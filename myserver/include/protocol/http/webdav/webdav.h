/*
  MyServer
  Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
  2011 Free Software Foundation, Inc.
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
#include <include/base/crypt/sha1.h>
#include <include/base/file/files_utility.h>
#include <include/base/xml/xml_parser.h>
#include <include/conf/vhost/vhost.h>

#include <string>

using namespace std;

class WebDAV
{
public:
  WebDAV ();
  int mkcol (HttpThreadContext*);
  xmlNodePtr generate (const char *, struct stat *m,
                       vector <const char*> *propReq);
  xmlDocPtr generateResponse (const char *, unsigned int,
                              vector <const char*> *propReq);
  xmlDocPtr generateLockResponse (string &, const char *urn);
  void getElements (xmlNode *, vector <const char*> *propReq);
  void getPropValue (const char *, struct stat *, char *);
  bool isLocked (HttpThreadContext *, string &);
  int propfind (HttpThreadContext *);
  int copy (HttpThreadContext *);
  int davdelete (HttpThreadContext *);
  int move (HttpThreadContext *);
  int lock (HttpThreadContext *);
  int unlock (HttpThreadContext *);

private:
  vector <const char*> available;
};
