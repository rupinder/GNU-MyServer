/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2007, 2009, 2010, 2011 Free Software Foundation, Inc.
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

#ifndef VHOST_MANAGER_H
# define VHOST_MANAGER_H

# include "myserver.h"
# include <include/conf/vhost/vhost.h>
# include <include/log/log_manager.h>

class VhostManagerHandler
{
public:
  VhostManagerHandler ();
  virtual ~VhostManagerHandler ();
  virtual Vhost* getVHost (const char*, const char*, u_short);
  virtual Vhost* getVHost (u_long n);
  virtual int load (const char *resource){return 0;}
};

/*!
  Proxy class to a VhostManagerHandler object.
 */
class VhostManager
{
public:
  typedef VhostManagerHandler* (*MAKE_HANDLER)(ListenThreads* lt,
                                               LogManager* lm);

  VhostManager ();
  void setHandler (VhostManagerHandler *handler);

  /*! Get a pointer to a vhost.  */
  Vhost* getVHost (const char*,const char*, u_short);
  Vhost* getVHost (int n);
  void registerBuilder (const string &name, MAKE_HANDLER builder);
  VhostManagerHandler *buildHandler (const string &name, ListenThreads *lt,
                                     LogManager *lm);

protected:
  HashMap<string, MAKE_HANDLER> builders;
  VhostManagerHandler *handler;
};

#endif
