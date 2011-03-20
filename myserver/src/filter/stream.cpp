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

#include "myserver.h"

#include <include/filter/stream.h>

#include <string>
#include <sstream>
using namespace std;

/*!
  Read [len] characters from the stream. Returns -1 on errors.
 */
int Stream::read (char* buffer,size_t len, size_t *nbr)
{
  *nbr = 0;
  return 0;
}

/*!
  Close the stream.
  \return -1 on errors.
 */
int Stream::close ()
{
  return 0;
}

/*!
  Write [len] characters to the stream. Returns -1 on errors.
 */
int Stream::write (const char* buffer, size_t len, size_t *nbw)
{
  *nbw = 0;
  return 0;
}

/*!
  Write remaining data to the stream.
 */
int Stream::flush (size_t* nbw)
{
  *nbw = 0;
  return 0;
}

Stream::Stream ()
{

}

Stream::~Stream ()
{

}

/*!
  Get the file handle.
 */
Handle Stream::getHandle ()
{
  return (Handle)-1;
}
