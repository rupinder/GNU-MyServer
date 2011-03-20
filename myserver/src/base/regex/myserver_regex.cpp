/*
  MyServer
  Copyright (C) 2002, 2003, 2004, 2007, 2008, 2009, 2010, 2011 Free
  Software Foundation, Inc.
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

#include <include/base/regex/myserver_regex.h>

/*!
  Compile the regex pattern.
 */
int Regex::compile (const char *p, int f)
{
  int ret = regcomp (&compiledRegex, p, f);
  pattern.assign (p);
  flags = f;
  if (!ret)
    compiled = 1;

  return ret;
}

/*!
  Match the pattern against strings.
 */
int Regex::exec (const char *text, size_t nmatch, regmatch_t matchptr [],
                 int eflags)
{
  if (!compiled)
    return 1;

  return regexec (&compiledRegex, text, nmatch, matchptr, eflags);
}

/*!
  Free the used memory.
 */
void Regex::free ()
{
  if (compiled)
    regfree (&compiledRegex);
  compiled = 0;
}

/*!
  Destructor for the class
 */
Regex::~Regex ()
{
  free ();
}

/*!
  Constructor for the class.
 */
Regex::Regex (const char *pattern, int flags)
{
  compiled = 0;
  compile (pattern, flags);
}
/*!
  Return a nonzero value if the regex was compiled.
 */
int Regex::isCompiled ()
{
  return compiled;
}

/*!
  Construct by copy.
 */
Regex::Regex (Regex& r)
{
  clone (r);
}

/*!
  Create a clone.
 */
void Regex::clone (Regex& r)
{
  r.compiled = compiled;
  if (!compiled)
    return;

  compile (r.pattern.c_str (), r.flags);
}
