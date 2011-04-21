/*
  MyServer
  Copyright (C) 2005, 2007, 2008, 2009, 2010, 2011 Free Software
  Foundation, Inc.
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "myserver.h"

#include <include/protocol/http/http_headers.h>
#include <include/protocol/http/http.h>
#include <include/protocol/http/http_data_handler.h>

/*!
  Send a file to the client using the HTTP protocol.
 */
int
HttpDataHandler::send (HttpThreadContext* td,
                       const char* /*filenamePath*/, const char* /*exec*/,
                       bool /*execute*/, bool /*onlyHeader*/)
{
  td->connection->host->warningsLogWrite (_("HttpDataHandler: using the base interface!"));
  return td->http->raiseHTTPError (500);
}

/*!
  Constructor for the class.
 */
HttpDataHandler::HttpDataHandler () { }

/*!
  Destroy the object.
 */
HttpDataHandler::~HttpDataHandler () { }

/*!
  Load the static elements.
 */
int HttpDataHandler::load ()
{
  return 0;
}

/*!
  Unload the static elements.
 */
int HttpDataHandler::unLoad ()
{
  return 0;
}

/*!
  Send data over the HTTP channel.  This method considers modifier filters.
  in the filters chain.
  \param td The HTTP thread context.
  \param buffer Data to send.
  \param size Size of the buffer.
  \param chain Where send data if not append.
  \param useChunks Can we use HTTP chunks to send data?
  \param realBufferSize The real dimension of the buffer that can be
  used by this method.
  \param tmpStream A support on memory read/write stream used
  internally by the function.
 */
size_t HttpDataHandler::appendDataToHTTPChannel (HttpThreadContext *td,
                                                 char *buffer,
                                                 size_t size,
                                                 FiltersChain *chain,
                                                 bool useChunks,
                                                 size_t realBufferSize,
                                                 MemoryStream *tmpStream)
{
  size_t nbr, nbw;
  Stream *oldStream = chain->getStream ();

  if (!chain->hasModifiersFilters ())
    return appendDataToHTTPChannel (td, buffer, size, chain, useChunks);

  /*
    This function can't append directly to the chain because we can't
    know in advance the data chunk size.  Therefore we replace the
    final stream with a memory buffer and write there the final data
    chunk content, finally we read from it and send directly to the
    original stream.
   */
  chain->setStream (tmpStream);

  chain->write (buffer, size, &nbw);

  tmpStream->read (buffer, realBufferSize, &nbr);

  chain->setStream (oldStream);

  return appendDataToHTTPChannel (td, buffer, nbr, chain, useChunks);
}

/*!
  Send raw data over the HTTP channel.  It doesn't consider modifier filters.
  Return zero on success.
  \param td The HTTP thread context.
  \param buffer Data to send.
  \param size Size of the buffer.
  \param chain Where send data if not append.
  \param useChunks Can we use HTTP chunks to send data?
 */
size_t
HttpDataHandler::appendDataToHTTPChannel (HttpThreadContext *td,
                                          char *buffer,
                                          size_t size,
                                          FiltersChain *chain,
                                          bool useChunks)
{
  size_t tmp, nbw = 0;
  if (chain->hasModifiersFilters ())
    {
      td->connection->host->warningsLogWrite (_("Http: internal error"));
      return 0;
    }

  if (useChunks)
    {
      ostringstream chunkHeader;
      size_t flushNbw = 0;
      chunkHeader << hex << size << "\r\n";

      chain->flush (&flushNbw);

      chain->getStream ()->write (chunkHeader.str ().c_str (),
                                  chunkHeader.str ().length (), &tmp);
    }

  if (size)
    chain->write (buffer, size, &nbw);

  if (useChunks)
    chain->getStream ()->write ("\r\n", 2, &tmp);

  return nbw;
}

/*!
  Check if the server can use the chunked transfer encoding and if the client
  supports keep-alive connections.
 */
void
HttpDataHandler::checkDataChunks (HttpThreadContext* td, bool* keepalive,
                                  bool* useChunks)
{
  *keepalive = td->request.isKeepAlive ();
  *useChunks = false;

  *keepalive &= !td->request.ver.compare ("HTTP/1.1");

  if (*keepalive)
    {
      HttpResponseHeader::Entry *e;
      e = td->response.other.get ("transfer-encoding");
      if (e)
        e->value.assign ("chunked");
      else
        {
          e = new HttpResponseHeader::Entry ();
          e->name.assign ("transfer-encoding");
          e->value.assign ("chunked");
          td->response.other.put (e->name, e);
        }
      *useChunks = true;
    }
}
