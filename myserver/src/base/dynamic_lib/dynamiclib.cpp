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
#include <include/base/dynamic_lib/dynamiclib.h>
#include <include/base/exceptions/exceptions.h>


class DynError : public AbstractServerException
{
public:
  DynError (const char *err)
  {
    this->err = err;
  }

  virtual const char *what () const throw ()
  {
    return err;
  }
private:
  const char *err;
};


/*!
  Initialize class internal data.
 */
DynamicLibrary::DynamicLibrary ()
{
  handle = 0;
}

/*!
  Destroy the object.
 */
DynamicLibrary::~DynamicLibrary ()
{
  close ();
}

/*!
  Load the specified dynamic library. It returns 0 on success.
  \param filename Name of the file to load.
  \param globally Set if the library is loaded globally.
 */
int DynamicLibrary::loadLibrary (const char *filename, int globally)
{
  fileName.assign (filename);
#ifdef WIN32
  handle = LoadLibrary (filename);
  /* FIXME: use a real error message.  */
  if (handle == NULL)
    throw DynError ("");
#endif

#ifdef HAVE_DL
  int flag = 0;
  if (globally)
    flag = RTLD_NOW | RTLD_GLOBAL;
  else
    flag = RTLD_LOCAL | RTLD_LAZY;

# ifdef RTLD_DEEPBIND
  flag |= RTLD_DEEPBIND;
# endif

  handle = dlopen (filename, flag);
  if (handle == NULL)
    throw DynError (dlerror ());

#endif
  return handle ? 0 : 1;
}

/*!
  Get a pointer to the specified symbol. Returns 0 on errors or
  the symbol address.
  \param fnName Symbol to find.
 */
void *DynamicLibrary::getProc (const char *sym)
{
  if (!handle)
    return 0;
#ifdef WIN32
    return (void *) GetProcAddress ((HMODULE) handle, sym);
#endif
#ifdef HAVE_DL
    return (void *) dlsym (handle, sym);
#endif
}

/*!
  Close the library. Returns 0 on success.
 */
int DynamicLibrary::close ()
{
  int ret = 1;
  if (!handle)
    return 1;
#ifdef WIN32
    ret = FreeLibrary ((HMODULE) handle) ? 0 : 1;
    /* FIXME: use a real error message.  */
    if (! ret)
      throw DynError ("");
#endif
#ifdef HAVE_DL
    ret = dlclose (handle);
    if (ret)
      throw DynError (dlerror ());
#endif
    handle = 0;
    fileName.assign ("");
    return ret;
}

/*!
  Return if the object has a valid handle.
 */
int DynamicLibrary::validHandle ()
{
  return handle ? 1 : 0;
}
