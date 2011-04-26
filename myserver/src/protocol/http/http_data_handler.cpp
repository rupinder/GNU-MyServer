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
 */
size_t HttpDataHandler::appendDataToHTTPChannel (HttpThreadContext *td,
                                                 const char *buffer,
                                                 size_t size)
{
  if (! td->outputChain.hasModifiersFilters () || size == 0)
    return appendDataToHTTPChannel (td, buffer, size, td->outputChain);

  /*
    This function can't append directly to the chain because we can't
    know in advance the data chunk size.  Therefore we replace the
    final stream with a memory buffer and write there the final data
    chunk content, finally we read from it and send directly to the
    original stream.
   */
  Stream *oldStream = td->outputChain.getStream ();
  size_t totalNbw = 0;
  try
    {
      char tmpBuf[BUFSIZ];
      size_t nbw;
      MemBuf memBuf;
      MemoryStream tmpStream (&memBuf);
      FiltersChain directStream (oldStream);
      size_t consumed = 0;

      memBuf.setExternalBuffer (tmpBuf, BUFSIZ);
      while (consumed < size)
        {
          size_t available;
          size_t written = std::min ((size_t) BUFSIZ / 2, size - consumed);
          td->outputChain.setStream (&tmpStream);
          td->outputChain.write (buffer + consumed, written, &available);
          td->outputChain.setStream (oldStream);

          totalNbw += appendDataToHTTPChannel (td, tmpBuf,
                                               available,
                                               directStream);
          tmpStream.refresh ();
          consumed += written;
        }
    }
  catch (...)
    {
      td->outputChain.setStream (oldStream);
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

  /* In the chunked transfer encoding, a chunk of size 0 has a special meaning,
     just skip it.  */
  if (td->transferEncoding == HttpThreadContext::TRANSFER_ENCODING_CHUNKED
      && size == 0)
    return 0;

  if (td->transferEncoding == HttpThreadContext::TRANSFER_ENCODING_CHUNKED)
    {
      ostringstream chunkHeader;
      chunkHeader << hex << size << "\r\n";
      chain.getStream ()->write (chunkHeader.str ().c_str (),
                                 chunkHeader.str ().length (), &tmp);
    }

  if (size)
    chain.write (buffer, size, &nbw);

  if (td->transferEncoding == HttpThreadContext::TRANSFER_ENCODING_CHUNKED)
    chain.getStream ()->write ("\r\n", 2, &tmp);

  return nbw;
}

/*!
  Choose the transfer encoding to use with the client.
  \param td The HTTP thread context.
  \param disableEncoding If specified don't use any transfer encoding.
  If it is specified, then the Content-Length must be specified.
 */
void
HttpDataHandler::chooseEncoding (HttpThreadContext *td, bool disableEncoding)
{
  td->keepalive = td->request.isKeepAlive ();
  td->transferEncoding = HttpThreadContext::TRANSFER_ENCODING_NONE;

  td->keepalive &= !td->request.ver.compare ("HTTP/1.1");

  if (!disableEncoding && td->keepalive)
    {
      td->response.setValue ("transfer-encoding", "chunked");
      td->transferEncoding = HttpThreadContext::TRANSFER_ENCODING_CHUNKED;
      td->response.contentLength.assign ("");
    }
}

/*!
  Begin a HTTP response.
*/
size_t
HttpDataHandler::beginHTTPResponse (HttpThreadContext *td,
                                    MemoryStream &memStream)
{
  char tmpBuf[BUFSIZ];
  size_t ret = 0;
  /*
    Flush initial data.  This is data that filters could have added
    and we have to send before the file itself, for example the gzip
    filter add a header to file.
  */
  if (memStream.availableToRead ())
    {
      FiltersChain directChain (td->outputChain.getStream ());
      for (;;)
        {
          size_t nbr;
          memStream.read (tmpBuf, BUFSIZ, &nbr);
          if (nbr == 0)
            break;

          ret += appendDataToHTTPChannel (td, tmpBuf, nbr, directChain);
        }
    }

  memStream.refresh ();

  return ret;
}

/*!
  Complete a HTTP response.
 */
size_t
HttpDataHandler::completeHTTPResponse (HttpThreadContext *td,
                                       MemoryStream &memStream)
{
  size_t ret = 0;

  /* If we don't use chunks we can flush directly.  */
  if (td->transferEncoding == HttpThreadContext::TRANSFER_ENCODING_NONE)
    {
      td->outputChain.flush (&ret);
    }
  else if (td->transferEncoding == HttpThreadContext::TRANSFER_ENCODING_CHUNKED)
    {
      /*
        Replace the final stream before the flush and write to a
        memory buffer, after all the data is flushed from the
        chain we can replace the stream with the original one and
        write there the HTTP data chunk.
      */
      size_t nbr, nbw;
      Stream* tmpStream = td->outputChain.getStream ();
      FiltersChain directChain (tmpStream);
      memStream.refresh ();
      td->outputChain.setStream (&memStream);
      td->outputChain.flush (&nbw);
      td->outputChain.setStream (tmpStream);

      memStream.read (td->buffer->getBuffer (),
                      td->buffer->getRealLength (), &nbr);

      ret += appendDataToHTTPChannel (td, td->buffer->getBuffer (),
                                      nbr, directChain);
      td->outputChain.getStream ()->write ("0\r\n\r\n", 5, &nbw);
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
                                       MimeRecord *mime,
                                       MemoryStream &memStream)
{
  size_t nbw = 0;
  HttpRequestHeader::Entry* e = td->request.other.get ("accept-encoding");
  memStream.refresh ();

  if (mime)
    factory->chain (&td->outputChain, mime->filters, &memStream, &nbw, 0,
                    false, e ? e->value : "");

  if (td->outputChain.hasModifiersFilters ())
    {
      string filters;
      td->outputChain.getName (filters);
      td->response.setValue ("content-encoding", filters.c_str ());
      td->response.contentLength.assign ("");
    }

  td->outputChain.setStream (td->connection->socket);
  return nbw;
}
