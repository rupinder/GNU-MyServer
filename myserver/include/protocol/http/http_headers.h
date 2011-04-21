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

#ifndef HTTP_HEADERS_H
# define HTTP_HEADERS_H

# include "myserver.h"
# include <include/connection/connection.h>

# ifdef WIN32
#  include <direct.h>
#  include <errno.h>
# else
#  include <string.h>
#  include <errno.h>
# endif


# include <string>
using namespace std;

enum
  {
    HTTP_AUTH_SCHEME_BASIC,
    HTTP_AUTH_SCHEME_DIGEST
  };

struct HttpThreadContext;
struct HttpRequestHeader;
struct HttpResponseHeader;

class HttpHeaders
{
public:
  static int buildHTTPRequestHeaderStruct (const char* input,
                                           size_t inputSize,
                                           size_t* headerSize,
                                           HttpRequestHeader *request,
                                           Connection* connection);

  static int buildHTTPResponseHeaderStruct (const char *input,
                                            HttpResponseHeader *response,
                                            size_t* nbtr);

  static int sendHeader (HttpResponseHeader &response, Stream &stream,
                         MemBuf &memBuf, HttpThreadContext *ctx);

  static int validHTTPRequest (const char *, size_t, size_t *, size_t *);
  static int validHTTPResponse (const char *, size_t *, size_t *);

  static void resetHTTPRequest (HttpRequestHeader *request);
  static void resetHTTPResponse (HttpResponseHeader *response);

  static void buildDefaultHTTPResponseHeader (HttpResponseHeader*);
  static void buildDefaultHTTPRequestHeader (HttpRequestHeader*);

  static size_t buildHTTPResponseHeader (char *, HttpResponseHeader*);
  static size_t buildHTTPRequestHeader (char *, HttpRequestHeader*);

protected:
  static int readReqAuthLine (HttpRequestHeader *request,
                              Connection *connection,
                              const char *token,
                              int *lenOut);

  static int readReqRangeLine (HttpRequestHeader *request,
                               Connection *connection,
                               const char *token,
                               int *lenOut);
};
#endif
