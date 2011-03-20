/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2002, 2008, 2009, 2010, 2011 Free Software Foundation,
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

#ifndef FIND_DATA_H
# define FIND_DATA_H

# include "myserver.h"

# ifdef WIN32
#  include <io.h>
# endif
# include <sys/types.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# ifndef WIN32
#  include <dirent.h>
# endif
# include <limits.h>


# include <string>
# include <include/base/sync/mutex.h>

using namespace std;

# ifndef EACCES
#  define EACCES 1
# endif

# define MAX_NAME NAME_MAX

# ifndef FILE_ATTRIBUTE_DIRECTORY
#  define FILE_ATTRIBUTE_DIRECTORY 1
# endif

# ifndef intptr_t
#  define intptr_t int
# endif

class ReadDirectory
{
public:
  string name;
  int attrib;
  time_t time_write;
  off_t size;
# ifdef WIN32
  u_long st_nlink;
# else
  nlink_t st_nlink;
# endif

  int findfirst (const char *directory);
  int findfirst (string &directory) {return findfirst (directory.c_str ());};
  int findnext ();
  int findclose ();
  ReadDirectory ();
  ~ReadDirectory ();

private:
  int find (const char *filename);
  string dirName;

# ifdef WIN32
  _finddata_t fd;
  intptr_t  ff;
# else
  DIR *dh;
  struct stat stats;
#  ifdef HAVE_READDIR_R
  struct dirent entry;
#  else
  static Mutex mutex;
#  endif

# endif
};

#endif
