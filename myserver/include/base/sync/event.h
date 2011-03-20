/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2006, 2009, 2010, 2011 Free Software Foundation, Inc.
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

#ifndef EVENT_H
# define EVENT_H

# include "myserver.h"
# include <include/base/sync/mutex.h>

# ifdef HAVE_PTHREAD
typedef pthread_cond_t EventHandle;
# else
typedef HANDLE EventHandle;
# endif

class Event
{
private:
  bool initialized;
  bool broadcast;
# ifdef HAVE_PTHREAD
  /*!
   *Under pthread a condition is associated with a mutex.
   */
  pthread_mutex_t mutex;
# endif
  EventHandle event;
public:
  Event (bool broadcast);
  ~Event ();
  int init (bool broadcast);
  int destroy ();
  int wait (u_long id = 0, u_long timeout = 0);
  int signal (u_long id = 0);
};
#endif
