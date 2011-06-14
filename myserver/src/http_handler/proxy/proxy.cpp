/*
  MyServer
  Copyright (C) 2009, 2010, 2011 Free Software Foundation, Inc.
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
#include <include/http_handler/proxy/proxy.h>

#include <include/base/string/stringutils.h>
#include <include/protocol/http/http_thread_context.h>
#include <include/protocol/http/http.h>
#include <include/protocol/url.h>
#include <include/base/socket/socket.h>
#include <include/protocol/http/http_request.h>
#include <include/protocol/http/http_response.h>
#include <include/protocol/http/http_data_read.h>
#include <include/protocol/http/http_data_handler.h>
#include <include/protocol/http/http_headers.h>
#include <include/filter/filters_chain.h>
#include <include/server/server.h>

#include <sstream>

/*
  Forward the HTTP requesto to another server.

  \param td The HTTP thread context.
  \param scriptpath Not used.
  \param exec The remote server Url.
  \param execute Not used.
  \param onlyHeader Specify if send only the HTTP header.
 */
int Proxy::send (HttpThreadContext *td, const char* scriptpath,
                 const char* exec, bool execute, bool onlyHeader)
{
  Url destUrl (exec, 80);
  ConnectionPtr con = NULL;
  Socket *sock;
  FiltersChain &chain = td->outputChain;
  HttpRequestHeader req;
  size_t nbw;
  bool keepalive = false;

  for (HashMap<string, HttpRequestHeader::Entry*>::Iterator it =
         td->request.begin (); it != td->request.end (); it++)
    {
      HttpRequestHeader::Entry *e = *it;
      req.setValue (e->name.c_str (), e->value.c_str ());
    }

  if (strcasecmp (destUrl.getProtocol (), "http"))
    {
      td->connection->host->warningsLogWrite
        ("Proxy: %s is not a supported protocol",
         destUrl.getProtocol ().c_str ());
      return td->http->raiseHTTPError (500);
    }

  try
    {
      req.ver.assign ("HTTP/1.1");
      req.cmd.assign (td->request.cmd);

      if (destUrl.getResource ()[0] == '\0' && td->pathInfo[0] == '\0')
        req.uri = "/";
      else
        {
          req.uri = destUrl.getResource ();
          req.uri.append (td->pathInfo);
        }
      if (td->request.uriOpts.length ())
        {
          req.uri.append ("?");
          req.uri.append (td->request.uriOpts);
        }

      req.setValue ("Connection", "keep-alive");
      if (td->request.uriOptsPtr)
        {
          char buffer[32];
          size_t size = td->inputData.getFileSize ();
          sprintf (buffer, "%u", size);
          req.setValue ("Content-Length", buffer);
        }

      ostringstream host;
      host << destUrl.getHost ();
      if (destUrl.getPort () != 80 )
        host << ":" << destUrl.getPort ();

      req.setValue ("Host", host.str ().c_str ());

      string xForwardedFor;
      td->request.getValue ("X-Forwarded-For", &xForwardedFor);
      if (xForwardedFor.size ())
        xForwardedFor.append (", ");
      xForwardedFor.append (td->connection->getIpAddr ());
      req.setValue ("X-Forwarded-For", xForwardedFor.c_str ());

      con = getConnection (destUrl.getHost ().c_str (), destUrl.getPort ());
      if (! con)
        return td->http->raiseHTTPError (500);

      sock = con->socket;

      req.clearValue ("accept-encoding");

      u_long hdrLen =
        HttpHeaders::buildHTTPRequestHeader (td->auxiliaryBuffer->getBuffer (),
                                             &req);


      sock->write (td->auxiliaryBuffer->getBuffer (), hdrLen, &nbw);

      if (td->request.uriOptsPtr)
        td->inputData.fastCopyToSocket (sock, 0, td->auxiliaryBuffer, &nbw);


      flushToClient (td, *sock, chain, onlyHeader, &keepalive);

      addConnection (con, destUrl.getHost ().c_str (), destUrl.getPort (),
                     keepalive);

      req.free ();
    }
  catch (exception & e)
    {
      if (con)
        addConnection (con, destUrl.getHost ().c_str (), destUrl.getPort (),
                       false);
      return HttpDataHandler::RET_FAILURE;
    }

  return HttpDataHandler::RET_OK;
}

/*!
  Flush the server reply to the client.
 */
int Proxy::flushToClient (HttpThreadContext* td, Socket &client,
                          FiltersChain &out, bool onlyHeader, bool *kaClient)
{
  size_t read = 0;
  size_t headerLength;
  int ret;
  size_t nbw;
  bool validResponse = false;
  size_t contentLength = (size_t) - 1;


  td->response.free ();

  for (;;)
    {
      ret = client.recv (td->auxiliaryBuffer->getBuffer () + read,
                         td->auxiliaryBuffer->getRealLength () - read - 1,
                         0,
                         td->http->getTimeout ());

      if (ret == 0)
        break;

      read += ret;

      td->auxiliaryBuffer->getBuffer ()[read] = '\0';

      validResponse = HttpHeaders::buildHTTPResponseHeaderStruct
        (td->auxiliaryBuffer->getBuffer (), &td->response, &headerLength);

      if (validResponse)
        break;
    }

  if (read == 0)
    return td->http->raiseHTTPError (500);

  if (td->response.contentLength.length ())
    contentLength = atoll (td->response.contentLength.c_str ());

  string *tmp = td->request.getValue ("Host", NULL);
  const char *via = tmp ? tmp->c_str () : td->connection->getLocalIpAddr ();

  tmp = td->response.getValue ("Via", NULL);
  if (tmp)
    {
      tmp->append (", ");
      tmp->append (via);
      td->response.setValue ("Via", tmp->c_str ());
    }
  else
    td->response.setValue ("Via", via);

  tmp = td->response.getValue ("Connection", NULL);
  *kaClient = tmp && !strcasecmp (tmp->c_str (), "keep-alive");

  string transferEncoding;
  bool hasTransferEncoding = false;
  tmp = td->response.getValue ("Transfer-Encoding", NULL);
  if (tmp)
    {
      hasTransferEncoding = true;
      transferEncoding.assign (*tmp);
    }

  char tmpBuf[1024];
  MemBuf memBuf;
  MemoryStream memStream (&memBuf);
  memBuf.setExternalBuffer (tmpBuf, sizeof (tmpBuf));
  generateFiltersChain (td, Server::getInstance ()->getFiltersFactory (),
                        td->mime, memStream);

  /* At this point we can modify the response struct before send it to the
     client.  */
  chooseEncoding (td, td->response.contentLength.length ());
  HttpHeaders::sendHeader (td->response, *out.getStream (), *td->buffer, td);

  if (onlyHeader)
    return HttpDataHandler::RET_OK;

  td->sentData += HttpDataHandler::beginHTTPResponse (td, memStream);

  readPayLoad (td, &td->response, &client,
               td->auxiliaryBuffer->getBuffer () + headerLength,
               read - headerLength, td->http->getTimeout (), contentLength,
               hasTransferEncoding ? &transferEncoding : NULL);

  return HttpDataHandler::RET_OK;
}

/*!
  Forward the message payload to the client.

  \param td The current HTTP thread context.
  \param res Response obtained by the server.
  \param initBuffer Initial read data.
  \param initBufferSize Size of initial data.
  \param timeout Connection timeout.
  with the client.
  \param contentLength The Content-Length specified by
  the client, if any.
  \param serverTransferEncoding Transfer-encoding
  used by the server.

  \return -1 on error.
  \return Otherwise the number of bytes transmitted.
 */
int Proxy::readPayLoad (HttpThreadContext* td,
                        HttpResponseHeader* res,
                        Socket *client,
                        const char *initBuffer,
                        u_long initBufferSize,
                        int timeout,
                        size_t contentLength,
                        string *serverTransferEncoding)
{
  size_t nbr = 0, nbw = 0, inPos = 0;

  /* Only the chunked transfer encoding is supported.  */
  if (serverTransferEncoding && serverTransferEncoding->compare ("chunked"))
    return HttpDataHandler::RET_FAILURE;

  /* If it is specified a transfer encoding read data using it.  */
  if (serverTransferEncoding)
    {
      if (!serverTransferEncoding->compare ("chunked"))
        {
          size_t remainingChunkSize = 0;
          for (;;)
            {
              if (HttpDataRead::readChunkedPostData (initBuffer, &inPos, initBufferSize,
                                                     client, td->buffer->getBuffer (),
                                                     td->buffer->getRealLength () - 1,
                                                     &nbr, timeout, NULL, 1,
                                                     &remainingChunkSize) < 0)
                return HttpDataHandler::RET_FAILURE;

              if (!nbr)
                break;

              td->sentData +=
                HttpDataHandler::appendDataToHTTPChannel (td,
                                                      td->buffer->getBuffer (),
                                                          nbr);
            }
        }
    }
  else
    {
      size_t length = contentLength;

      /* If it is not specified an encoding, read the data as it is.  */
      for (;;)
        {
          u_long len = td->buffer->getRealLength () - 1;

          if (contentLength != ((size_t) -1) && length < len)
            len = length;

          if (len == 0)
            break;

          int timedOut =
            HttpDataRead::readContiguousPrimitivePostData (initBuffer, &inPos,
                                                       initBufferSize, client,
                                                     td->buffer->getBuffer (),
                                                           len, &nbr, timeout);

          if (contentLength == 0 && nbr == 0)
            break;

          if (length)
            length -= nbr;

          td->sentData +=
            HttpDataHandler::appendDataToHTTPChannel (td,
                                             td->buffer->getBuffer (), nbr);
          if (timedOut || contentLength != ((size_t) -1) && length == 0)
            break;
        }
    }

  MemoryStream memStream (td->auxiliaryBuffer);
  td->sentData += completeHTTPResponse (td, memStream);

  return HttpDataHandler::RET_OK;
}

int Proxy::load ()
{
  const char *data
    = Server::getInstance ()->getData ("http.proxy.connections.keepalive",
                                       "64");
  maxKeepAliveConnections = atoi (data);

  return 0;
}

ConnectionPtr Proxy::getConnection (const char *host, u_short port)
{
  ConnectionPtr ret = NULL;

  connectionsLock.lock ();
  try
    {
      vector<ConnectionRecord>::iterator it = connections.begin ();
      for (; it != connections.end (); it++)
        {
          if ((*it).port == port && (*it).host.compare (host) == 0)
            {
              ret = (*it).connection;
              connections.erase (it);
              break;
            }
        }
      connectionsLock.unlock ();
      if (ret)
        return ret;
    }
  catch (...)
    {
      connectionsLock.unlock ();
      throw;
    }

  ret = new Connection ();
  ret->socket = new Socket ();
  try
    {
      ret->socket->socket (AF_INET, SOCK_STREAM, 0);
      ret->socket->connect (host, port);
    }
  catch (...)
    {
      delete ret;
      throw;
    }

  return ret;
}

void Proxy::proxySchedulerHandler (void *p, Connection *c, int event)
{
  Proxy *proxy = (Proxy *) p;
  proxy->removeConnection (c);
}

void Proxy::removeConnection (Connection *c)
{
  if (c->getToRemove ())
    {
      delete c;
      return;
    }

  connectionsLock.lock ();
  try
    {
      vector<ConnectionRecord>::iterator it = connections.begin ();
      for (; it != connections.end (); it++)
        {
          /* Remove only if it is found in the buffer.  Otherwise it is
             used at this time and we can't remove it.  */
          if ((*it).connection == c)
            {
              connections.erase (it);
              delete c;
              break;
            }
        }

      connectionsLock.unlock ();
    }
  catch (...)
    {
      connectionsLock.unlock ();
    }
}

void Proxy::addConnection (ConnectionPtr con, const char *host, u_short port,
                           bool keepalive)
{
  ConnectionsScheduler *cs  = Server::getInstance ()->getConnectionsScheduler ();

  if (!keepalive || connections.size () >= maxKeepAliveConnections)
    {
      /* Do not destroy the connection immediately, it can be used by the
         scheduler, so delay its destruction until we are sure it is not used
         by the scheduler anymore.  */
      con->setToRemove (1);
      cs->addNewWaitingConnection (con);
      return;
    }

  con->setSchedulerHandler (Proxy::proxySchedulerHandler, this);

  cs->addNewWaitingConnection (con);

  ConnectionRecord cr;
  cr.host = host;
  cr.port = port;
  cr.connection = con;

  connections.push_back (cr);
}

int Proxy::unLoad ()
{
  ConnectionsScheduler *cs  = Server::getInstance ()->getConnectionsScheduler ();

  connectionsLock.lock ();
  try
    {
      vector<ConnectionRecord>::iterator it = connections.begin ();
      for (; it != connections.end (); it++)
        {
          (*it).connection->setToRemove (1);
          cs->addNewWaitingConnection ((*it).connection);
        }

      connections.clear ();
      connectionsLock.unlock ();
    }
  catch (...)
    {
      connectionsLock.unlock ();
      throw;
    }

  return 0;
}
