/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2005, 2007, 2009, 2010, 2011 Free Software Foundation,
  Inc.
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

#ifndef FILTER_H
# define FILTER_H
# include "myserver.h"
# include <include/filter/stream.h>
# include <include/protocol/protocol.h>

/*!
  Abstract class to handle virtual data filters.
 */
class Filter : public Stream
{
public:
  Protocol* getProtocol ()
  {
    return protocol;
  }
  void setProtocol (Protocol* pr)
  {
    protocol = pr;
  }
  virtual int getHeader (char* buffer, size_t len, size_t* nbw);
  virtual int getFooter (char* buffer, size_t len, size_t* nbw);
  virtual int read (char* buffer, size_t len, size_t *);
  virtual int write (const char* buffer, size_t len, size_t *);
  virtual int flush (size_t *);
  virtual int modifyData ();
  virtual const char* getName ();
  void setParent (Stream*);
  Stream* getParent ();
  Filter ();
  /*! Avoid direct instances of this class. */
  virtual ~Filter () = 0;

protected:
  Protocol *protocol;
  Stream *parent;
};

#endif
