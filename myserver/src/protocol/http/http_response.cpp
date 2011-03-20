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
#include <include/protocol/http/http_response.h>
#include <include/base/string/stringutils.h>
#include <iostream>
#include <sstream>
using namespace std;

/*!
  Create the object.
 */
HttpResponseHeader::HttpResponseHeader ()
{
  free ();
}


/*!
  Destroy the object.
 */
HttpResponseHeader::~HttpResponseHeader ()
{
  HashMap<string, HttpResponseHeader::Entry*>::Iterator it = other.begin ();
  for (;it != other.end (); it++)
    delete (*it);
}

/*!
  Reset the object.
 */
void HttpResponseHeader::free ()
{
  ver.clear ();
  contentLength.clear ();
  errorType.clear ();

  HashMap<string, HttpResponseHeader::Entry*>::Iterator it = other.begin ();
  for (;it != other.end (); it++)
    delete (*it);

  other.clear ();
}

/*!
  Get the value of the [name] field.
 */
string* HttpResponseHeader::getValue (const char* name, string* out)
{
  if (! strcasecmp (name, "Ver"))
    {
      if (out)
        out->assign (ver.c_str ());
      return &ver;
    }

  if (! strcasecmp (name, "Content-length"))
    {
      if (out)
        out->assign (contentLength.c_str ());
      return &contentLength;
    }

  return HttpHeader::getValue (name, out);
}

/*!
  Set the value of the [name] field to [in].
 */
string* HttpResponseHeader::setValue (const char* name, const char* in)
{
  if (! strcasecmp (name, "Ver"))
    {
      ver.assign (in);
      return &ver;
    }

  if (! strcasecmp (name, "Content-length"))
    {
      contentLength.assign (in);
      return &contentLength;
    }

  return HttpHeader::setValue (name, in);
}

/*!
  Get the kind of HTTP status code specified by the httpStatus variable.
  \return the HTTP status kind.
 */
int HttpResponseHeader::getStatusType ()
{
  if (httpStatus < 200)
    return HttpResponseHeader::INFORMATIONAL;

  if (httpStatus < 300)
    return HttpResponseHeader::SUCCESSFUL;

  if (httpStatus < 400)
    return HttpResponseHeader::REDIRECTION;

  if (httpStatus < 500)
    return HttpResponseHeader::CLIENT_ERROR;

  return HttpResponseHeader::SERVER_ERROR;
}
