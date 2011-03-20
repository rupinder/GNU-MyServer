/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2005, 2009, 2010, 2011 Free Software Foundation, Inc.
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

#ifndef DYN_HTTP_MANAGER_H
# define DYN_HTTP_MANAGER_H

# include "myserver.h"
# include <include/base/xml/xml_parser.h>
# include <include/protocol/protocol.h>
# include <include/connection/connection.h>
# include <include/base/dynamic_lib/dynamiclib.h>
# include <include/protocol/http/http_headers.h>
# include <include/base/hash_map/hash_map.h>
# include <string>
using namespace std;


class DynamicHttpManager
{
public:
  DynamicHttpManager ();
  virtual ~DynamicHttpManager ();
  virtual int send (HttpThreadContext*, ConnectionPtr s, const char *filenamePath,
                    const char* cgi, int selfExecuted, bool onlyHeader = false) = 0;

};

#endif
