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

#ifndef FILTERS_CHAIN_H
# define FILTERS_CHAIN_H
# include "myserver.h"
# include <include/filter/stream.h>
# include <include/filter/filter.h>
# include <list>

using namespace std;

class FiltersChain : public Stream
{
public:
  void setAcceptDuplicates (int);
  int getAcceptDuplicates ();
  void setStream (Stream*);
  Stream* getStream ();
  Filter* getFirstFilter ();
  int isEmpty ();
  int addFilter (Filter*,size_t *nbw, int sendData = 1);
  void clearAllFilters ();
  int isFilterPresent (Filter*);
  int isFilterPresent (const char*);
  int removeFilter (Filter*);
  int clear ();
  void getName (string& out);
  int hasModifiersFilters ();
  list<string> getFilters ();
  virtual int read (char *buffer, size_t len, size_t *);
  virtual int write (const char *buffer, size_t len, size_t *);
  virtual int flush (size_t *);
  FiltersChain ();
  FiltersChain (Stream *s);
  ~FiltersChain ();
protected:
  list <Filter*> filters;
  Filter* firstFilter;
  Stream *stream;
  int acceptDuplicates;
};

#endif
