/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2002, 2003, 2004, 2007, 2009, 2010, 2011 Free Software
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

#ifndef HTTP_REQUEST_H
# define HTTP_REQUEST_H

# include "myserver.h"
# include <string>
# include <include/base/hash_map/hash_map.h>
# include <include/protocol/http/http_header.h>

using namespace std;


/*! Max length for a HTTP request fields. */
# define HTTP_REQUEST_CMD_DIM 16
# define HTTP_REQUEST_VER_DIM 10
# define HTTP_REQUEST_AUTH_DIM 16
# define HTTP_REQUEST_CONTENT_LENGTH_DIM 12
# define HTTP_REQUEST_URI_DIM 4096
# define HTTP_REQUEST_URI_OPTS_DIM 4096
# define HTTP_REQUEST_OTHER_DIM 4096
# define HTTP_REQUEST_RANGE_TYPE_DIM 16


/*!
  Structure to describe an HTTP request.
 */
struct HttpRequestHeader : public HttpHeader
{
  string cmd;
  string ver;
  string auth;
  string contentLength;
  string uri;
  string uriOpts;
  char *uriOptsPtr;
  string rangeType;
  off_t rangeByteBegin;
  off_t rangeByteEnd;
  int uriEndsWithSlash;

  /*! Digest authorization scheme stuff.  */
  char digestRealm[48+1];
  char digestOpaque[48+1];
  char digestNonce[48+1];
  char digestCnonce[48+1];
  char digestUri[1024+1];
  char digestMethod[16+1];
  char digestUsername[48+1];
  char digestResponse[48+1];
  char digestQop[16+1];
  char digestNc[10+1];

  virtual string* getValue (const char* name, string* out);
  virtual string* setValue (const char* name, const char* in);

  HashMap<string, HttpRequestHeader::Entry*>::Iterator begin (){return other.begin ();}
  HashMap<string, HttpRequestHeader::Entry*>::Iterator back (){return other.back ();}
  HashMap<string, HttpRequestHeader::Entry*>::Iterator end (){return other.end ();}


  bool isKeepAlive ();

  HttpRequestHeader ();
  ~HttpRequestHeader ();
  void free ();

};

#endif
