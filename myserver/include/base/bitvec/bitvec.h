/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2009, 2010, 2011 Free Software Foundation, Inc.
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

#ifndef BITVEC_H
# define BITVEC_H

# include "myserver.h"

class BitVec
{
public:
  BitVec (int capacity, bool val);

  void init (int capacity, bool val);

  int ffs ();

  int find ();

  /*!
    Set the value of a bit to 1.
    \param i index of the bit to set.
   */
  void set (u_long i)
  {
    data[i / (sizeof (long int) * 8)] |= 1l << i % (sizeof (long int) * 8);
  }

  /*!
    Set the value of a bit to 1.
    \param i index of the bit to set.
   */
  void unset (u_long i)
  {
    data[i / (sizeof (long int) * 8)] &= ~(1l << i % (sizeof (long int) * 8));
  }

  /*!
    Get the value of the specified bit.
    \param i index of the bit to get.
   */
  bool get (u_long i)
  {
    return (data[i / (sizeof (long int) * 8)] >>
            i % (sizeof (long int) * 8)) & 1l ? true : false;
  }

  ~BitVec ()
  {
    delete [] data;
  }

  size_t getCapacity ()
  {
    return capacity;
  }

private:
  /* Used internally by find.  */
  int lastFound;

  long int *data;
  size_t dataSize;
  size_t capacity;
};

#endif
