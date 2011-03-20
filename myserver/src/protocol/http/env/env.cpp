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

#include "myserver.h"
#include <include/protocol/http/env/env.h>
#include <include/protocol/http/http_headers.h>
#include <include/protocol/http/http.h>
#include <include/protocol/http/http_errors.h>
#include <include/server/server.h>

#include <string>
#include <sstream>
#include <algorithm>

#ifdef WIN32
# include <direct.h>
#endif
#include <string.h>


using namespace std;

/*!
  Write the string that contain the CGI environment to cgiEnvString.
  This function is used by other server side protocols too.
  \param td The HTTP thread context.
  \param cgiEnv The zero terminated list of environment string.
  \param processEnv Specify if add current process environment
  variables too.
*/
void Env::buildEnvironmentString (HttpThreadContext* td, char *cgiEnv,
                                  int processEnv)
{
  MemBuf memCgi;
  MemBuf portBuffer;
  MemBuf remotePortBuffer;
  char strTmp[32];
  HttpRequestHeader::Entry* reqEntry = NULL;

  memCgi.setExternalBuffer (cgiEnv, td->auxiliaryBuffer->getRealLength ());
  memCgi << "SERVER_SOFTWARE=GNU MyServer " << MYSERVER_VERSION;

#ifdef WIN32
  memCgi << " (WIN32)";
#else
# ifdef HOST_STR
  memCgi << " " << HOST_STR;
# else
  memCgi << " (Unknown)";
# endif
#endif
  /* Must use REDIRECT_STATUS for php and others.  */
  memCgi << end_str << "REDIRECT_STATUS=TRUE";

  memCgi << end_str << "SERVER_NAME=";
  memCgi << Server::getInstance ()->getServerName ();

  memCgi << end_str << "SERVER_SIGNATURE=";
  memCgi << "<address>GNU MyServer " << MYSERVER_VERSION << "</address>";

  memCgi << end_str << "SERVER_PROTOCOL=";
  memCgi << td->request.ver.c_str ();

  portBuffer.uintToStr ( td->connection->getLocalPort ());
  memCgi << end_str << "SERVER_PORT="<< portBuffer;

  memCgi << end_str << "SERVER_ADMIN=";
  memCgi << td->securityToken.getData ("server.admin",
                                       MYSERVER_VHOST_CONF |
                                       MYSERVER_SERVER_CONF, "");

  memCgi << end_str << "REQUEST_METHOD=";
  memCgi << td->request.cmd.c_str ();

  memCgi << end_str << "REQUEST_URI=";

  memCgi << td->request.uri.c_str ();

  memCgi << end_str << "QUERY_STRING=";
  memCgi << td->request.uriOpts.c_str ();

  memCgi << end_str << "GATEWAY_INTERFACE=CGI/1.1";

  if (td->request.contentLength.length ())
    {
      memCgi << end_str << "CONTENT_LENGTH=";
      memCgi << td->request.contentLength.c_str ();
    }
  else
    {
      u_long fs = 0;
      ostringstream stream;

      if (td->inputData.getHandle () >= 0)
        fs = td->inputData.getFileSize ();

      stream << fs;
      memCgi << end_str << "CONTENT_LENGTH=" << stream.str ().c_str ();
    }

  if (td->request.rangeByteBegin || td->request.rangeByteEnd)
    {
      ostringstream rangeBuffer;
      memCgi << end_str << "HTTP_RANGE=" << td->request.rangeType << "=" ;
      if (td->request.rangeByteBegin)
        {
          rangeBuffer << static_cast<int>(td->request.rangeByteBegin);
          memCgi << rangeBuffer.str ();
        }
      memCgi << "-";
      if (td->request.rangeByteEnd)
        {
          rangeBuffer << td->request.rangeByteEnd;
          memCgi << rangeBuffer.str ();
        }

    }

  memCgi << end_str << "CGI_ROOT=";
  memCgi << td->cgiRoot;

  memCgi << end_str << "REMOTE_ADDR=";
  memCgi << td->connection->getIpAddr ();

  remotePortBuffer.MemBuf::uintToStr (td->connection->getPort () );
  memCgi << end_str << "REMOTE_PORT=" << remotePortBuffer;

  memCgi << end_str << "REMOTE_USER=";
  memCgi << td->connection->getLogin ();

  if (td->http->getProtocolOptions () & Protocol::SSL)
    memCgi << end_str << "SSL=ON";
  else
    memCgi << end_str << "SSL=OFF";


  if (td->pathInfo.length ())
    {
      memCgi << end_str << "PATH_INFO=";
      memCgi << td->pathInfo;

      memCgi << end_str << "PATH_TRANSLATED=";
      memCgi << td->pathTranslated;
    }
  else
    {
      memCgi << end_str << "PATH_TRANSLATED=";
      memCgi << td->filenamePath;
    }

  memCgi << end_str << "SCRIPT_FILENAME=";
  memCgi << td->filenamePath;

  /*
    For the DOCUMENT_URI and SCRIPT_NAME copy the
    requested uri without the pathInfo.
  */
  memCgi << end_str << "SCRIPT_NAME=";
  memCgi << td->request.uri.c_str ();

  memCgi << end_str << "SCRIPT_URL=";
  memCgi << td->request.uri.c_str ();

  memCgi << end_str << "DATE_GMT=";
  getRFC822GMTTime (strTmp);
  memCgi << strTmp;

  memCgi << end_str << "DATE_LOCAL=";
  getRFC822LocalTime (strTmp);
  memCgi << strTmp;

  memCgi << end_str << "DOCUMENT_ROOT=";
  memCgi << td->connection->host->getDocumentRoot ();

  memCgi << end_str << "DOCUMENT_URI=";
  memCgi << td->request.uri.c_str ();

  memCgi << end_str << "DOCUMENT_NAME=";
  memCgi << td->filenamePath;

  memCgi << end_str << "REMOTE_IDENT=";
  memCgi << td->connection->getLogin ();

  memCgi << end_str << "AUTH_TYPE=";
  memCgi << td->request.auth.c_str ();


  reqEntry = td->request.other.get ("content-type");

  if (reqEntry)
    {
      memCgi << end_str << "CONTENT_TYPE=";
      memCgi << reqEntry->value.c_str ();
    }

  buildHttpHeaderEnvString (memCgi, td->request);

  if (processEnv)
    buildProcessEnvString (memCgi);


  memCgi << end_str << end_str  << end_str  << end_str  << end_str  ;
}

/*!
  Append to the environment string variables from the HTTP request header.
*/
void Env::buildHttpHeaderEnvString (MemBuf& memCgi, HttpRequestHeader & req)
{

  HashMap<string, HttpRequestHeader::Entry*>::Iterator it = req.begin ();
  for (; it != req.end (); it++)
    {
      HttpRequestHeader::Entry* en = *it;
      string name;

      name.assign ("HTTP_");
      name.append (en->name.c_str ());
      transform (name.begin ()+5, name.end (), name.begin ()+5, ::toupper);
      for (int i = name.length (); i > 5; i--)
        if (name[i] == '-')
          name[i] = '_';

      memCgi  << end_str << name.c_str () << "=" << en->value.c_str ();
    }
}

/*!
  Append to the environment string process env variables.
*/
void Env::buildProcessEnvString (MemBuf& memCgi)
{
#ifdef WIN32
  LPTSTR lpszVariable;
  LPVOID lpvEnv;
  lpvEnv = Server::getInstance ()->getEnvString ();
  memCgi << end_str;
  if (lpvEnv)
    for (lpszVariable = (LPTSTR) lpvEnv; *lpszVariable; lpszVariable++)
      {
        if (((char*)lpszVariable)[0]  != '=' )
          {
            memCgi << (char*)lpszVariable << end_str;
          }
        while (*lpszVariable)
          *lpszVariable++;
      }
#endif
}
