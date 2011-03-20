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

#ifndef PIPE_H
# define PIPE_H

# include "myserver.h"
# include <include/filter/stream.h>
# include <string>

using namespace std;


class Pipe : public Stream
{
public:
  Pipe ();
  virtual ~Pipe ();
  int create (bool readPipe = true);
  long getReadHandle ();
  long getWriteHandle ();
  void inverted (Pipe&);
  virtual int read (char* buffer, size_t len, size_t *nbr);
  virtual int write (const char* buffer, size_t len, size_t *nbw);
  virtual int close ();
  void closeRead ();
  void closeWrite ();
  bool pipeTerminated (){return terminated;}
  int waitForData (int sec, int usec);
private:
  bool terminated;
  int handles[2];
};
#endif
