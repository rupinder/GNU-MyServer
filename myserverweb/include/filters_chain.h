/*
*MyServer
*Copyright (C) 2002,2003,2004 The MyServer Team
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef FILTERS_CHAIN_H
#define FILTERS_CHAIN_H
#include "../stdafx.h"
#include "../include/stream.h"
#include "../include/filter.h"
#include <list>

using namespace std;

class FiltersChain 
{
protected:
  list <Filter*> filters;
  Filter* firstFilter;
  Stream *stream;
  int acceptDuplicates;
public:
  void setAcceptDuplicates(int);
  int getAcceptDuplicates();
  void setStream(Stream*);
  Stream* getStream();
  Filter* getFirstFilter();
  int isEmpty();
  int addFilter(Filter*,u_long *nbw, int sendData=1);
  void clearAllFilters();
  int isFilterPresent(Filter*);
  int isFilterPresent(const char*);
  int removeFilter(Filter*);
  int clear();
  void getName(string& out);
  int hasModifiersFilters();
  int read(char* buffer, u_long len, u_long*);
  int write(const char* buffer, u_long len, u_long*);
	int flush(u_long*);
  FiltersChain();
  ~FiltersChain();
};


#endif
