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

#ifndef STRINGUTILS_H
# define STRINGUTILS_H

# include "myserver.h"
# include <string>

# include <string.h>

using namespace std;

const char *getRFC822GMTTime (char* out);
const char *getRFC822GMTTime (const time_t,char* out);
const char *getRFC822LocalTime (char* out);
const char *getRFC822LocalTime (const time_t,char* out);

const char *getRFC822GMTTime (string& out);
const char *getRFC822GMTTime (const time_t, string& out);
const char *getRFC822LocalTime (string& out);
const char *getRFC822LocalTime (const time_t, string &out);

int getCharInString (const char*,const char*,int max);

const char* getLocalLogFormatDate (const time_t t, char* out);
const char* getLocalLogFormatDate (char* out);

const char* getLocalLogFormatDate (const time_t t, string& out);
const char* getLocalLogFormatDate (string& out);

time_t getTime (const char* str);
inline time_t getTime (string const& str){ return getTime (str.c_str ()); }

void trim (char* str, char const* trimChars);

void gotoNextLine (char** cmd);

int hexVal (char c);

void translateEscapeString (char *TargetStr);
void translateEscapeString (string& TargetStr);

int hexToInt (const char *str);
inline int hexToInt (string const& str){ return hexToInt (str.c_str ()); }

int getEndLine (const char* str, int max);
inline int getEndLine (string const& str, int max)
{return getEndLine (str.c_str (), max); }

string trim (string const& s, string const&t = " ");
string trimLeft ( string const &s , string const &t = " " );
string trimRight (string const &s , string const &t = " " );

int strcasecmp (string const &a, string const &b);
int strcmp (string const &a, string const &b);

int strcasecmp (string const &a, const char* b);
int strcmp (string const &a, const char* b);

#endif
