/*
*MyServer
*Copyright (C) 2002,2003,2004 The MyServer Team
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "../include/myserver_regex.h"

regex_t compiled_regex;
regmatch_t match;

/*!
 *Compile the regex pattern.
 */
int Regex::compile(const char *pattern, int flags)
{
  int ret = regcomp(&compiled_regex, pattern, flags);
  if(!ret)
    compiled = 1;
  return ret;
}

/*!
 *Match the pattern against strings.
 */
int Regex::exec(const char *text, size_t nmatch, regmatch_t matchptr [], 
                int eflags)
{
  if(!compiled)
    return 1;
  int ret = regexec (&compiled_regex, text, nmatch, matchptr, eflags);
  return ret;
}

/*!
 *free the used memory.
 */
void Regex::free()
{
  if(compiled)
    regfree(&compiled_regex);
  compiled = 0;
}

/*!
 *Constructor for the class.
 */
Regex::Regex()
{
  compiled = 0;
}

/*!
 *Destructor for the class
 */
Regex::~Regex()
{
  free();
}

/*!
 *Constructor for the class.
 */
Regex::Regex(const char *pattern, int flags)
{
  compile(pattern, flags);
}
/*!
 *Return a nonzero value if the regex was compiled.
 */
int Regex::isCompiled()
{
  return compiled;
}
