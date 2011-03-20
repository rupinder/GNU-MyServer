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

#ifndef MUTEX_H
# define MUTEX_H

# include "myserver.h"
# include <include/base/file/file.h>
# include <include/base/string/stringutils.h>

# ifdef HAVE_PTHREAD
typedef pthread_mutex_t MutexHandle;
# else
typedef HANDLE MutexHandle;
# endif

class Mutex
{
public:
  Mutex ();
  virtual ~Mutex ();
  virtual int init ();
  int destroy ();
  int lock (u_long id = 0);
  int unlock (u_long id = 0);
  bool isLocked ();
  MutexHandle getHandle ();
protected:
  bool locked;
  int initialized;
  MutexHandle mutex;
};
#endif
