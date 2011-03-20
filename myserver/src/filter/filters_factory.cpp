/*
  MyServer
  Copyright (C) 2002, 2003, 2004, 2007, 2008, 2009, 2010, 2011 Free
  Software Foundation, Inc.
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
#include <include/filter/filters_factory.h>

#include <string>
#include <sstream>

using namespace std;


/*!
  Initialize the object.
*/
FiltersFactory::FiltersFactory ()
{
  staticFilters.clear ();
}

/*!
  Destroy the object.
*/
FiltersFactory::~FiltersFactory ()
{

}

/*!
  Insert a filter by name and factory object. Returns 0 if the entry
  was added correctly.
*/
int FiltersFactory::insert (const char* name, FiltersSource* ptr)
{
  string nameStr (name);
  dynamicFilters.put (nameStr, ptr);
  return 0;
}

/*!
  Insert a filter by name and factory routine. Returns 0 if the entry
  was added correctly.
*/
int FiltersFactory::insert (const char* name, FILTERCREATE fnc)
{
  string nameStr (name);
  staticFilters.put (nameStr, fnc);
  return 0;
}

/*!
  Get a new filter by its name.
  The object have to be freed after its use to avoid memory leaks.
  Returns the new created object on success.
  Returns 0 on errors.
*/
Filter *FiltersFactory::getFilter (const char* name)
{
  FILTERCREATE staticFactory = staticFilters.get (name);
  FiltersSource* dynamicFactory;

  /* If the filter exists create a new object and return it. */
  if (staticFactory)
    return staticFactory (name);

  dynamicFactory = dynamicFilters.get (name);

  if (dynamicFactory)
    return dynamicFactory->createFilter (name);

  return 0;
}

/*!
  Create a FiltersChain starting from a list of strings.
  On success returns the new object.
  If specified [onlyNotModifiers] the method wil check that all the filters
  will not modify the data.
  On errors returns 0.
*/
FiltersChain* FiltersFactory::chain (list<string> &l, Stream* out, size_t *nbw,
                                     int onlyNotModifiers)
{
  FiltersChain *ret = new FiltersChain ();
  if (!ret)
    return 0;

  if (chain (ret, l, out, nbw, onlyNotModifiers))
    {
      ret->clearAllFilters ();
      delete ret;
      return 0;
    }
  return ret;
}

/*!
  Add new filters to an existent chain.
  \param c Output object.  It will receive the chain.
  \param l List of filters to build.
  \param out Output stream.
  \param nbw Number of written bytes, some filters may need to send a header.
  \param onlyNotModifiers if non-zero the method wil check that all the filters
  will not modify the data.
  \param accepted If specified, include only filters present in this string.
  On errors returns nonzero.
*/
int FiltersFactory::chain (FiltersChain* c, list<string> &l, Stream* out,
                           size_t *nbw, int onlyNotModifiers, string *accepted)
{

  list<string>::iterator  i = l.begin ();
  if (!c)
    return 1;

  c->setStream (out);
  *nbw = 0;
  for ( ; i != l.end (); i++)
    {
      size_t tmp;

      if (accepted && accepted->find (*i) == string::npos)
        continue;

      Filter *n = getFilter ((*i).c_str ());
      if (!n)
        {
          c->clearAllFilters ();
          return 1;
        }

      if (onlyNotModifiers && n->modifyData ())
        {
          delete n;
          c->clearAllFilters ();
          return 1;
        }

      c->addFilter (n, &tmp);
      *nbw += tmp;
    }

  return 0;
}

/*!
  Free the object.
*/
void FiltersFactory::free ()
{
  staticFilters.clear ();
  dynamicFilters.clear ();
}
