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
#include <include/filter/filters_chain.h>

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
  \param realBufferSize The real dimension of the buffer that can be
  used by this method.
  \param tmpStream A support on memory read/write stream used
  internally by the function.
 */
size_t HttpDataHandler::appendDataToHTTPChannel (HttpThreadContext *td,
                                                 const char *buffer,
                                                 size_t size,
                                                 FiltersChain &chain,
                                                 size_t realBufferSize,
                                                 MemoryStream &tmpStream)
{
  char tmpBuf[BUFSIZ];
  size_t nbr, nbw, written = 0, totalNbw = 0;
  Stream *oldStream = chain.getStream ();

  if (! chain.hasModifiersFilters () || size == 0)
    return appendDataToHTTPChannel (td, buffer, size, chain);

  /*
    This function can't append directly to the chain because we can't
    know in advance the data chunk size.  Therefore we replace the
    final stream with a memory buffer and write there the final data
    chunk content, finally we read from it and send directly to the
    original stream.
   */
  try
    {
      FiltersChain directStream (oldStream);
      do
        {
          if (size - written)
            {
              chain.setStream (&tmpStream);
              chain.write (buffer + written, size - written, &nbw);
              chain.setStream (oldStream);
              written += size;
            }

          tmpStream.read (tmpBuf, BUFSIZ, &nbr);
          if (nbr)
            totalNbw += appendDataToHTTPChannel (td, tmpBuf, nbr, directStream);
        }
      while (size - written || nbr);
      tmpStream.refresh ();
    }
  catch (...)
    {
      chain.setStream (oldStream);
      throw;
    }

  return totalNbw;
}

/*!
  Send raw data over the HTTP channel.  It doesn't consider modifier filters.
  Return zero on success.
  \param td The HTTP thread context.
  \param buffer Data to send.
  \param size Size of the buffer.
  \param chain Where send data if not append.
 */
size_t
HttpDataHandler::appendDataToHTTPChannel (HttpThreadContext *td,
                                          const char *buffer,
                                          size_t size,
                                          FiltersChain &chain)
{
  size_t tmp, nbw = 0;

  if (td->useChunks)
    {
      ostringstream chunkHeader;
      chunkHeader << hex << size << "\r\n";
      chain.getStream ()->write (chunkHeader.str ().c_str (),
                                 chunkHeader.str ().length (), &tmp);
    }

  if (size)
    chain.write (buffer, size, &nbw);

  if (td->useChunks)
    chain.getStream ()->write ("\r\n", 2, &tmp);

  return nbw;
}

/*!
  Check if the server can use the chunked transfer encoding and if the client
  supports keep-alive connections.
 */
void
HttpDataHandler::checkDataChunks (HttpThreadContext* td, bool disableEncoding)
{
  td->keepalive = td->request.isKeepAlive ();
  td->useChunks = false;

  td->keepalive &= !td->request.ver.compare ("HTTP/1.1");

  if (!disableEncoding && td->keepalive)
    {
      td->response.setValue ("transfer-encoding", "chunked");
      td->useChunks = true;
    }
}

/*!
  Begin a HTTP response.
*/
size_t
HttpDataHandler::beginHTTPResponse (HttpThreadContext *td,
                                    MemoryStream &memStream,
                                    FiltersChain &chain,
                                    bool useChunks)
{
  size_t ret = 0;
  /*
    Flush initial data.  This is data that filters could have added
    and we have to send before the file itself, for example the gzip
    filter add a header to file.
  */
  if (memStream.availableToRead ())
    {
      size_t nbr;
      memStream.read (td->buffer->getBuffer (),
                      td->buffer->getRealLength (), &nbr);

      memStream.refresh ();
      if (nbr)
        {
          FiltersChain directChain (chain.getStream ());
          ret += appendDataToHTTPChannel (td, td->buffer->getBuffer (), nbr,
                                          directChain);
        }
    }

  return ret;
}

/*!
  Complete a HTTP response.
 */
size_t
HttpDataHandler::completeHTTPResponse (HttpThreadContext *td,
                                       MemoryStream &memStream,
                                       FiltersChain &chain,
                                       bool useChunks)
{
  size_t ret = 0;
  /* If we don't use chunks we can flush directly.  */
  if (! useChunks)
    chain.flush (&ret);
  else
    {
      /*
        Replace the final stream before the flush and write to a
        memory buffer, after all the data is flushed from the
        chain we can replace the stream with the original one and
        write there the HTTP data chunk.
      */
      size_t nbr, nbw;
      Stream* tmpStream = chain.getStream ();
      FiltersChain directChain (tmpStream);
      memStream.refresh ();
      chain.setStream (&memStream);
      chain.flush (&nbw);
      chain.setStream (tmpStream);
      memStream.read (td->buffer->getBuffer (),
                      td->buffer->getRealLength (), &nbr);

      ret += appendDataToHTTPChannel (td, td->buffer->getBuffer (),
                                      nbr, directChain);
      chain.getStream ()->write ("0\r\n\r\n", 5, &nbw);
    }

  return ret;
}

/*!
  Populate a FiltersChain object ensuring desired filters are supported by the
  client.
 */
size_t
HttpDataHandler::generateFiltersChain (HttpThreadContext *td,
                                       FiltersFactory *factory,
                                       FiltersChain &fc,
                                       MimeRecord *mime,
                                       MemoryStream &memStream)
{
  size_t nbw = 0;
  HttpRequestHeader::Entry* e = td->request.other.get ("accept-encoding");
  memStream.refresh ();

  if (mime)
    factory->chain (&fc, mime->filters, &memStream, &nbw, 0, false,
                    e ? e->value : "");

  if (fc.hasModifiersFilters ())
    {
      string filters;
      fc.getName (filters);
      td->response.setValue ("content-encoding", filters.c_str ());
    }

  fc.setStream (td->connection->socket);
  return nbw;
}
