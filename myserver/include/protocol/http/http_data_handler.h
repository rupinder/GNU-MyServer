/* -*- mode: c++ -*- */
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

#ifndef HTTP_DATA_HANDLER_H
# define HTTP_DATA_HANDLER_H

# include "myserver.h"
# include <include/protocol/protocol.h>
# include <include/protocol/http/http_headers.h>
# include <include/protocol/http/http_thread_context.h>
# include <include/filter/filters_chain.h>
# include <include/filter/filters_factory.h>
# include <include/filter/memory_stream.h>
# include <include/conf/mime/mime_manager.h>

/*!
  Base class to handle HTTP data.
 */
class HttpDataHandler
{
public:

  enum
    {
      RET_OK = 0,
      RET_FAILURE
    };

  virtual int load ();
  virtual int unLoad ();

  virtual int send (HttpThreadContext *, const char *exec,
                    const char *cmdLine = 0, bool execute = false,
                    bool onlyHeader = false);

  HttpDataHandler ();
  virtual ~HttpDataHandler ();

  static void chooseEncoding (HttpThreadContext *td,
                               bool disableEncoding = false);

  static size_t appendDataToHTTPChannel (HttpThreadContext *td,
                                         const char *buffer,
                                         size_t size);

  static size_t beginHTTPResponse (HttpThreadContext *td,
                                   MemoryStream &memStream);

  static size_t completeHTTPResponse (HttpThreadContext *td,
                                      MemoryStream &memStream);

  static size_t generateFiltersChain (HttpThreadContext *td,
                                      FiltersFactory *factory,
                                      MimeRecord *mime,
                                      MemoryStream &memStream);

private:
  static size_t appendDataToHTTPChannel (HttpThreadContext* td,
                                         const char *buffer,
                                         size_t size,
                                         FiltersChain &chain);
};

#endif
