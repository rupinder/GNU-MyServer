/*
  MyServer
  Copyright (C) 2002, 2003, 2004, 2006, 2007, 2008, 2009, 2010, 2011
  Free Software Foundation, Inc.
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

/*
  To get more info about the FastCGI protocol please visit the official
  FastCGI site at: http://www.fastcgi.com.
  On that site you can find samples and all the supported languages.
 */

#include "myserver.h"

#include <include/http_handler/fastcgi/fastcgi.h>
#include <include/protocol/http/env/env.h>
#include <include/protocol/http/http.h>
#include <include/base/string/stringutils.h>
#include <include/server/server.h>
#include <include/filter/filters_chain.h>
#include <include/base/file/files_utility.h>
#include <include/base/string/securestr.h>

#include <string>
#include <sstream>
using namespace std;

#define SERVERS_DOMAIN "fastcgi"

/*! Is the fastcgi initialized?  */
int FastCgi::initialized = 0;

/*! Process server manager.  */
ProcessServerManager *FastCgi::processServerManager = 0;

struct FourChar
{
  union
  {
    unsigned int i;
    unsigned char c[4];
  };
};

/*!
  Entry-Point to manage a FastCGI request.
 */
int FastCgi::send (HttpThreadContext* td, const char* scriptpath,
                   const char *cgipath, bool execute, bool onlyHeader)
{
  FcgiContext con;
  size_t nbr = 0;
  FcgiHeader header;
  FiltersChain chain;
  size_t nbw;

  int exit;
  int ret;

  clock_t initialTicks;

  FastCgiServer* server = 0;
  int id;
  ostringstream cmdLine;
  string moreArg;
  bool responseCompleted = false;

  con.useChunks = false;
  con.keepalive = false;

  con.td = td;
  con.headerSent = false;

  td->scriptPath.assign (scriptpath);


  if (!(td->permissions & MYSERVER_PERMISSION_EXECUTE))
    return td->http->sendAuth ();

  try
    {
      string tmp;
      tmp.assign (cgipath);
      FilesUtility::splitPath (tmp, td->cgiRoot, td->cgiFile);
      tmp.assign (scriptpath);
      FilesUtility::splitPath (tmp, td->scriptDir, td->scriptFile);
      chain.setStream (td->connection->socket);

      if (td->mime)
        Server::getInstance ()->getFiltersFactory ()->chain (&chain,
                                                             td->mime->filters,
                                                             td->connection->socket,
                                                             &nbw,
                                                             1);

      td->buffer->setLength (0);
      td->auxiliaryBuffer->getAt (0) = '\0';


      /* Do not modify the text between " and ".  */
      int i;
      int subString = cgipath[0] == '"';
      int len = strlen (cgipath);
      string tmpCgiPath;
      for (i = 1; i < len; i++)
        {
          if (!subString && cgipath[i]==' ')
            break;
          if (cgipath[i] == '"' && cgipath[i - 1] != '\\')
            subString = !subString;
        }

      /*
        Save the cgi path and the possible arguments.
        the (x < len) case is when additional arguments are specified.
        If the cgipath is enclosed between " and " do not consider them
        when splitting directory and file name.
      */
      if (len)
        {
          if (i < len)
            {
              string tmpString (cgipath);
              int begin = tmpString[0]=='"' ? 1: 0;
              int end = tmpString[i] == '"' ? i - 1: i;
              tmpCgiPath.assign (tmpString.substr (begin, end - begin));
              moreArg.assign (tmpString.substr (i, len - 1));
            }
          else
            {
              int begin = (cgipath[0] == '"') ? 1 : 0;
              int end   = (cgipath[len] == '"') ? len - 1 : len;
              tmpCgiPath.assign (&cgipath[begin], end - begin);
              moreArg.assign ("");
            }
          FilesUtility::splitPath (tmpCgiPath, td->cgiRoot, td->cgiFile);
        }
      tmpCgiPath.assign (scriptpath);
      FilesUtility::splitPath (tmpCgiPath, td->scriptDir, td->scriptFile);

      if (execute)
        {
          if (cgipath && strlen (cgipath))
            {
#ifdef WIN32
              {
                int x;
                string cgipathString (cgipath);
                int len = strlen (cgipath);
                int subString = cgipath[0] == '"';

                cmdLine << "\"" << td->cgiRoot << "/" << td->cgiFile << "\" "
                        << moreArg << " \"" <<  td->filenamePath << "\"";
              }
#else
              cmdLine << cgipath << " " << td->filenamePath;
#endif
            }/*if (execute).  */
          else
            cmdLine << scriptpath;
        }
      else
        {
#ifdef WIN32
          cmdLine << "\"" << td->cgiRoot << "/" << td->cgiFile
                  << "\" " << moreArg;
#else
          cmdLine << cgipath;
#endif
        }

      server = connect (&con, cmdLine.str ().c_str ());
      if (server == NULL)
        {
          td->buffer->setLength (0);
          td->connection->host->warningsLogWrite
              (_("FastCGI: cannot connect to the %s process"),
               cmdLine.str ().c_str ());
          chain.clearAllFilters ();
          return td->http->raiseHTTPError (500);
        }

      id = td->id + 1;
      if (fastCgiRequest (&con, id))
        return td->http->raiseHTTPError (500);

      /* Now read the output. This flag is used by the external loop.  */
      exit = 0;

      /* Return 1 if keep the connection. A nonzero value also mean no errors.  */
      ret = HttpDataHandler::RET_OK;

      initialTicks = getTicks ();

      td->buffer->setLength (0);
      checkDataChunks (td, &con.keepalive, &con.useChunks);

      do
        {
          u_long dim;
          u_long timeout = td->http->getTimeout ();

          readHeader (&con, &header, initialTicks, timeout, id);

          /*
            contentLengthB1 is the high word of the content length value
            while contentLengthB0 is the low one.
            To retrieve the value of content length push left contentLengthB1
            of eight byte then do an or with contentLengthB0.
          */
          dim = (header.contentLengthB1 << 8) | header.contentLengthB0;

          if (dim == 0)
            {
              exit = 1;
              ret = HttpDataHandler::RET_FAILURE;
            }
          else
            {
              bool headerCompleted = false;
              u_long timeout = td->http->getTimeout ();

              switch (header.type)
                {
                case FCGISTDERR:

                  /* Fall trough.  */

                case FCGISTDOUT:
                  headerCompleted = false;
                  ret = sendData (&con, dim, static_cast<u_long>(timeout),
                                  &chain, &headerCompleted, onlyHeader);

                  if (ret)
                    {
                      exit = 1;
                      if (ret == 1)
                        responseCompleted = true;
                      else
                        ret = HttpDataHandler::RET_FAILURE;

                      break;
                    }

                  if (headerCompleted)
                    exit = 1;

                  break;

                case FCGIEND_REQUEST:
                  exit = 1;
                  break;

                case FCGIGET_VALUES_RESULT:
                case FCGIUNKNOWN_TYPE:
                default:
                  break;
                }
            }

          if (header.paddingLength)
            {
              u_long toPad = header.paddingLength;
              u_long timeout = td->http->getTimeout ();
              while (toPad)
                {
                  if (td->buffer->getRealLength () < toPad)
                    toPad =  td->buffer->getRealLength ();

                  nbr = con.sock.recv (td->buffer->getBuffer (), toPad,
                                       0, static_cast<u_long> (timeout));
                  toPad -= nbr;
                }
            }
        }
      while (! exit);

      /* Send the last null chunk if needed.  */
      if (!responseCompleted && con.useChunks && !onlyHeader)
        chain.getStream ()->write ("0\r\n\r\n", 5, &nbw);

      chain.clearAllFilters ();
      con.sock.close ();
    }
  catch (exception & e)
    {
      td->connection->host->warningsLogWrite (_E ("FastCGI: internal error"),
                                              &e);
      chain.clearAllFilters ();
      return td->http->raiseHTTPError (500);
    }

  return ret;
}

/*!
  Send the buffer content over the FastCGI connection
  Return non-zero on errors.
 */
int FastCgi::sendFcgiBody (FcgiContext* con, char* buffer, int len, int type,
                          int id)
{
  FcgiHeader header;
  generateFcgiHeader ( header, type, id, len );

  con->sock.send ((char*) &header, sizeof (header), 0);
  con->sock.send ((char*) buffer, len, 0);
  return 0;
}

/*!
  Trasform from a standard environment string to the FastCGI environment
  string.
 */
int FastCgi::buildFASTCGIEnvironmentString (HttpThreadContext*, char* src,
                                           char* dest)
{
  char *ptr = dest;
  char *sptr = src;
  char varName[100];
  char varValue[2500];
  for (;;)
  {
    int max = 100;
    u_long i;
    FourChar varNameLen;
    FourChar varValueLen;

    varNameLen.i = varValueLen.i = 0;
    varName[0] = '\0';
    varValue[0] = '\0';

    while (*sptr == '\0')
      sptr++;

    while ((--max) && *sptr != '=')
    {
      varName[varNameLen.i++] = *sptr++;
      varName[varNameLen.i] = '\0';
    }
    if (max == 0)
      return -1;
    sptr++;
    max = 2500;
    while ((--max) && *sptr != '\0')
    {
      varValue[varValueLen.i++] = *sptr++;
      varValue[varValueLen.i] = '\0';
    }
    if (max == 0)
      return -1;
    if (varNameLen.i > 127)
    {
      unsigned char fb = varValueLen.c[3] | 0x80;
      *ptr++ = fb;
      *ptr++ = varNameLen.c[2];
      *ptr++ = varNameLen.c[1];
      *ptr++ = varNameLen.c[0];
    }
    else
    {
      *ptr++ = varNameLen.c[0];
    }

    if (varValueLen.i > 127)
    {
      unsigned char fb = varValueLen.c[3] | 0x80;
      *ptr++ = fb;
      *ptr++ = varValueLen.c[2];
      *ptr++ = varValueLen.c[1];
      *ptr++ = varValueLen.c[0];
    }
    else
    {
      *ptr++ = varValueLen.c[0];
    }
    for (i = 0; i < varNameLen.i; i++)
      *ptr++ = varName[i];
    for (i = 0; i < varValueLen.i; i++)
      *ptr++ = varValue[i];
    if (*(++sptr) == '\0')
      break;
  }
  return static_cast<int> (ptr - dest);
}

/*!
  Fill the FcgiHeader structure.
 */
void FastCgi::generateFcgiHeader ( FcgiHeader &header, int iType,
                                  int iRequestId, int iContentLength )
{
  header.version = FCGIVERSION;
  header.type = (u_char)iType;
  header.requestIdB1 = (u_char)((iRequestId >> 8 ) & 0xff);
  header.requestIdB0 = (u_char)((iRequestId ) & 0xff);
  header.contentLengthB1 = (u_char)((iContentLength >> 8 ) & 0xff);
  header.contentLengthB0 = (u_char)((iContentLength ) & 0xff);
  header.paddingLength = 0;
  header.reserved = 0;
}

/*!
  Constructor for the FASTCGI class
 */
FastCgi::FastCgi ()
{
  initialized = 0;
}

/*!
  Initialize the FastCGI protocol implementation
 */
int FastCgi::load ()
{
  if (initialized)
    return 1;
  initialized = 1;
  processServerManager = Server::getInstance ()->getProcessServerManager ();
  processServerManager->createDomain (SERVERS_DOMAIN);
  return 0;
}

/*!
  Clean the memory and the processes occuped by the FastCGI servers
 */
int FastCgi::unLoad ()
{
  initialized = 0;
  return 0;
}

/*!
  Return the the running server specified by path.
  If the server is not running returns 0.
 */
FastCgiServer* FastCgi::isFcgiServerRunning (const char* path)
{
  return processServerManager->getServer (SERVERS_DOMAIN, path);
}


/*!
  Get a connection to the FastCGI server.
 */
FastCgiServer* FastCgi::connect (FcgiContext* con, const char* path)
{
  FastCgiServer* server = runFcgiServer (con, path);
  /* If we find a valid server try the connection to it.  */
  if (server)
  {
    int ret = processServerManager->connect (&(con->sock), server);
    if (ret == -1)
      return 0;
  }
  return server;
}

/*!
  Run the FastCGI server.
  If the path starts with a @ character, the path is handled as a
  remote server.
 */
FastCgiServer* FastCgi::runFcgiServer (FcgiContext* context,
                                      const char* path)
{
  /* This method needs a better home (and maybe better code).
    Compute a simple hash from the IP address.  */
  const char *ip = context->td->connection->getIpAddr ();
  int seed = 13;
  for (const char *c = ip; *c; c++)
    seed = *c * 21 + seed;

  FastCgiServer* server = processServerManager->getServer (SERVERS_DOMAIN,
                                                          path,
                                                          seed);

  if (server)
    return server;

  /* If the path starts with @ then the server is remote.  */
  if (path[0] == '@')
  {
    int i = 1;
    char host[128];
    char port[6];

    while (path[i] && path[i] != ':')
      i++;

    myserver_strlcpy (host, &path[1], min (128, i));

    myserver_strlcpy (port, &path[i + 1], 6);

    return processServerManager->addRemoteServer (SERVERS_DOMAIN, path,
                                                 host, atoi (port));
  }

  return processServerManager->runAndAddServer (SERVERS_DOMAIN, path);
}

/*!
  Do a request to the FastCGI server.
  \param con The current FastCGI context.
  \param id The request ID.
 */
int FastCgi::fastCgiRequest (FcgiContext* con, int id)
{
  FcgiBeginRequestBody tBody;
  HttpThreadContext *td = con->td;
  /* Size of data chunks to use with STDIN.  */
  const size_t maxStdinChunk = 8192;
  size_t nbr;
  FcgiHeader header;
  int sizeEnvString;

  Env::buildEnvironmentString (td, td->buffer->getBuffer ());
  sizeEnvString = buildFASTCGIEnvironmentString (td,td->buffer->getBuffer (),
                                                td->auxiliaryBuffer->getBuffer ());
  if (sizeEnvString == -1)
  {
    td->buffer->setLength (0);
    td->connection->host->warningsLogWrite (_("FastCGI: internal error"));
    return 1;
  }


  tBody.roleB1 = ( FCGIRESPONDER >> 8 ) & 0xff;
  tBody.roleB0 = ( FCGIRESPONDER ) & 0xff;
  tBody.flags = 0;
  memset ( tBody.reserved, 0, sizeof ( tBody.reserved ) );

  sendFcgiBody (con, (char*)&tBody, sizeof (tBody), FCGIBEGIN_REQUEST, id);
  sendFcgiBody (con, td->auxiliaryBuffer->getBuffer (), sizeEnvString,
                FCGIPARAMS, id);
  sendFcgiBody (con, 0, 0, FCGIPARAMS, id);

  if (atoi (td->request.contentLength.c_str ()))
  {
    td->buffer->setLength (0);
    td->inputData.seek (0);

    /* Send the STDIN data.  */
    do
    {
      td->inputData.read (td->buffer->getBuffer (), maxStdinChunk, &nbr);

      if (!nbr)
        break;

      generateFcgiHeader (header, FCGISTDIN, id, nbr);
      con->sock.send ((char*) &header, sizeof (header), 0);
      con->sock.send (td->buffer->getBuffer (), nbr, 0);
    }
    while (nbr == maxStdinChunk);
  }

  /* Final stdin chunk.  */
  sendFcgiBody (con, 0, 0, FCGISTDIN, id);

  return 0;
}


/*!
  Handle the STDOUT data received from the FastCGI server.
  The FastCGI header must be read previously, only the payload is processed
  here.
  \param con The current FastCGI context.
  \param dim Size of the payload.
  \param timeout Connection timeout to use.
  \param chain Output chain where to send data.
  \param responseCompleted Output value.  It is true when the response is
  completed.
  \param onlyHeader Send only the HTTP header.
  \return 0 on success.
  \return 1 if a full response is sent to the client.
  \return -1 on error.
 */
int FastCgi::sendData (FcgiContext* con, u_long dim, u_long timeout,
                       FiltersChain* chain, bool *responseCompleted,
                       bool onlyHeader)
{
  u_long dataRead = 0;
  while (dataRead < dim)
    {
      HttpThreadContext* td = con->td;
      u_long timeout = td->http->getTimeout ();

      if (con->headerSent)
        td->buffer->setLength (0);

      if (dim - td->buffer->getLength () == 0)
        return -1;

      size_t nbr = con->sock.recv (td->buffer->getBuffer ()
                                   + td->buffer->getLength (),
                                std::min ((u_long) td->buffer->getRealLength (),
                                             dim - td->buffer->getLength ()),
                                   0, timeout);

      td->buffer->setLength (td->buffer->getLength () + nbr);

      dataRead += nbr;

      if (! con->headerSent)
        return handleHeader (con, chain, responseCompleted, onlyHeader);
    }

  if (onlyHeader)
    return 1;

  HttpDataHandler::appendDataToHTTPChannel (con->td,
                                            con->td->buffer->getBuffer (),
                                            con->td->buffer->getLength (),
                                            &(con->td->outputData),
                                            chain,
                                            con->td->appendOutputs,
                                            con->useChunks);

  con->td->sentData += con->td->buffer->getLength ();

  return 0;
}

/*!
  Look for a HTTP header in the fastcgi server data.
  \param con The current FastCGI context.
  \param chain Output chain where to send data.
  \param responseCompleted Output value.  It is true when the response is completed.
  \param onlyHeader If non-zero, don't send any HTTP payload.
  \return 0 on success.
 */
int FastCgi::handleHeader (FcgiContext* con, FiltersChain* chain, bool* responseCompleted,
         bool onlyHeader)
{
  char* buffer = con->td->buffer->getBuffer ();
  u_long size = con->td->buffer->getLength ();
  u_long headerSize = 0;

  for (u_long i = 0; i < size - 3; i++)
  {
    if ((buffer[i] == '\r') && (buffer[i + 1] == '\n') &&
       (buffer[i + 2] == '\r') && (buffer[i + 3] == '\n'))
    {
      headerSize = i + 4;
      break;
    }
  }

  HttpHeaders::buildHTTPResponseHeaderStruct (con->td->buffer->getBuffer (),
                                              &con->td->response,
                                              &(con->td->nBytesToRead));

  bool allowSendfile =
    ! strcasecmp (con->td->securityToken.getData ("fastcgi.sendfile.allow",
                                              MYSERVER_VHOST_CONF
                                              | MYSERVER_SERVER_CONF, "NO"),
                  "YES");
  if (allowSendfile)
    {
      string *sendfile = con->td->response.getValue ("X-Sendfile", NULL);
      if (sendfile)
        {
          DynHttpManagerList *m =
            con->td->http->getStaticData ()->getDynManagerList ();
          HttpDataHandler *handler = m->getHttpManager ("SEND");
          if (!handler)
            {
              con->td->connection->host->warningsLogWrite
                (_("FastCGI: cannot find handler for SEND"));
              return con->td->http->raiseHTTPError (500);
            }

          string url (*sendfile);
          delete con->td->response.other.remove ("X-Sendfile");
          *responseCompleted = true;
          handler->send (con->td, url.c_str (), NULL, false, onlyHeader);
          return 1;
        }
    }

  if (!con->td->appendOutputs)
    {
      string *location = con->td->response.getValue ("Location", NULL);
      if (location)
        {
          *responseCompleted = true;
          return con->td->http->sendHTTPRedirect (location->c_str ());
        }
    }

  if (HttpHeaders::sendHeader (con->td->response, *con->td->connection->socket,
                               *con->td->auxiliaryBuffer, con->td))
    {
      *responseCompleted = true;
      return 1;
    }

  con->headerSent = true;

  /* Flush the buffer if remaining data is present.  */
  if (size - headerSize)
    {
      HttpDataHandler::appendDataToHTTPChannel (con->td,
                                                con->td->buffer->getBuffer () + headerSize,
                                                size - headerSize,
                                                &(con->td->outputData),
                                                chain,
                                                con->td->appendOutputs,
                                                con->useChunks);
      con->td->sentData += size - headerSize;
    }

  return 0;
}

/*!
  Read a FastCGI header.
  \param con The FastCGI context.
  \param header The header to read.
  \param started Start time for the request (it is used to compute the timeout).
  \param timeout The timeout to use for the connection.
  \param id Request ID.
  \return 0 on success.
 */
int FastCgi::readHeader (FcgiContext *con, FcgiHeader* header, u_long started,
                         u_long timeout, int id)
{
  size_t nbr;
  HttpThreadContext* td = con->td;
  char* buffer = (char*) header;
  u_long readData = 0;

  for (;;)
    {
      u_long ticks = getTicks ();

      if (ticks - started > timeout)
        {
          td->connection->host->warningsLogWrite (_("FastCGI: server timeout"));
          sendFcgiBody (con, 0, 0, FCGIABORT_REQUEST, id);
          return 1;
        }

      nbr = con->sock.recv (buffer + readData, sizeof (FcgiHeader) - readData,
                            0, timeout - (ticks - started));

      if (nbr == 0)
        return 1;

      readData += nbr;

      if (readData == sizeof (FcgiHeader))
        break;

      if (readData > sizeof (FcgiHeader))
        {
          td->connection->host->warningsLogWrite (_("FastCGI: internal error"));
          sendFcgiBody (con, 0, 0, FCGIABORT_REQUEST, id);
          return 1;
        }
    }

  return 0;
}
