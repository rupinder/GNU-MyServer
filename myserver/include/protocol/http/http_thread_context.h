/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2002, 2003, 2004, 2008, 2009, 2010, 2011 Free Software
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

#ifndef HTTP_THREAD_CONTEXT_H
# define HTTP_THREAD_CONTEXT_H

# include "myserver.h"
# include <include/protocol/http/http_data_handler.h>
# include <include/protocol/http/http_request.h>
# include <include/protocol/http/http_response.h>
# include <include/base/string/stringutils.h>
# include <include/base/file/file.h>
# include <include/base/mem_buff/mem_buff.h>
# include <include/connection/connection.h>
# include <include/conf/mime/mime_manager.h>
# include <include/conf/security/security_token.h>
# include <include/filter/filters_chain.h>

# ifdef WIN32
#  include <direct.h>
#  include <errno.h>
# else
#  include <string.h>
#  include <errno.h>
# endif

# include <string>
using namespace std;

class Http;
class MimeRecord;

/*!
  Store thread specific data.
 */
struct HttpThreadContext
{
  /*! Http::raiseHTTPError sets this.  */
  int lastError;

  /*! Is the client asking only for the header?  */
  bool onlyHeader;

  /*! Was the HTTP header already flushed to the client?  */
  bool headerSent;

  /*! Size of the request header in `buffer'. */
  size_t headerSize;

  enum
    {
      TRANSFER_ENCODING_NONE = 0,
      TRANSFER_ENCODING_CHUNKED
    }
  transferEncoding;

  bool keepalive;

  FiltersChain outputChain;

  ConnectionPtr connection;
  MemBuf *buffer;
  MemBuf *auxiliaryBuffer;
  u_long id;
  size_t nBytesToRead;
  HttpResponseHeader response;
  HttpRequestHeader request;
  string filenamePath;
  string pathInfo;
  string pathTranslated;
  string cgiRoot;
  string cgiFile;
  string scriptPath;
  string scriptDir;
  string scriptFile;
  string vhostDir;
  string vhostSys;
  HashMap<string,string*> other;
  char identity[32];
  File inputData;
  int authScheme;
  Http *http;
  MimeRecord *mime;
  int sentData;
  SecurityToken securityToken;
  int permissions;

  ~HttpThreadContext ();
  const char* getVhostDir ();
  const char* getVhostSys ();
  const char* getData (const char *name);
};

#endif
