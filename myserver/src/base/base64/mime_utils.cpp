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
#include <include/base/base64/mime_utils.h>
#include <include/base/exceptions/checked.h>

#ifndef WIN32
# include <ctype.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <include/base/string/stringutils.h>
#include <include/base/string/securestr.h>

static char* myserver_strupr (char * s)
{
  unsigned int len = strlen (s);
  for (register unsigned int i = 0; i < len; i++)
    s[i] = toupper (s[i]);
  return s;
}

const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#define SKIP '\202'
#define NOSKIP 'A'
#define MaxLineLength 76
const char base64map[] =
{
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,  SKIP,  SKIP,   SKIP,  SKIP,  SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,    62,   SKIP,   SKIP,   SKIP,    63,
  52,    53,    54,    55,    56,    57,    58,    59,
  60,    61,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,    0 ,    1 ,    2 ,    3 ,    4 ,    5 ,    6 ,
  7 ,    8 ,    9 ,    10,    11,    12,    13,    14,
  15,    16,    17,    18,    19,    20,    21,    22,
  23,    24,    25,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,    26,    27,    28,    29,    30,    31,    32,
  33,    34,    35,    36,    37,    38,    39,    40,
  41,    42,    43,    44,    45,    46,    47,    48,
  49,    50,    51,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP
};



const char hexmap[] =
{
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  0 ,    1 ,    2 ,    3 ,    4 ,    5 ,    6 ,    7 ,
  8 ,    9 ,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,    10,    11,    12,    13,    14,    15,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP
};



const char QpEncodeMap[] =
{
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   NOSKIP,   SKIP,   SKIP,   NOSKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
  NOSKIP,   SKIP,   SKIP,   SKIP,   SKIP,   NOSKIP,   NOSKIP,   NOSKIP,
  NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
  NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
  NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   SKIP,   NOSKIP,   NOSKIP,
  SKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
  NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
  NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
  NOSKIP,   NOSKIP,   NOSKIP,   SKIP,   SKIP,   SKIP,   SKIP,   NOSKIP,
  SKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
  NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
  NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,   NOSKIP,
    NOSKIP,   NOSKIP,   NOSKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
    SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP
};



CBase64Utils::CBase64Utils ()
{
  ErrorCode = 0;
}

CBase64Utils::~CBase64Utils ()
{

}
/*!
  Decode a string using the Base64 codification
 */
char* CBase64Utils::encode (const char *input, int bufsize)
{
  int alsize = ((bufsize * 4) / 3);
  char *finalresult = (char*)calloc (alsize + ((alsize / 76) * 2) +
                                    (10 * sizeof (char)), sizeof (char));
  int count = 0;
  int LineLen = 0;
  char* fresult = finalresult;
  const char *s = input;
  int tmp = 0;
  while (count <= bufsize)
  {
    if (count % 3 == 0 && count != 0)
    {
      tmp >>= 8;
      tmp &= 0xFFFFFF;
      int mid = tmp;
      mid >>= 18;
      mid &= 0x3F;
      *(fresult++) = base64chars[mid];
      LineLen++;
      mid = tmp;
      mid >>= 12;
      mid &= 0x3F;
      *(fresult++) = base64chars[mid];
      LineLen++;
      mid = tmp;
      mid >>= 6;
      mid &= 0x3F;
      *(fresult++) = base64chars[mid];
      LineLen++;
      mid = tmp;
      mid &= 0x3F;
      *(fresult++) = base64chars[mid];
      LineLen++;
      tmp = 0;

      if (LineLen >= MaxLineLength)
      {
        *(fresult++) = '\r';
        *(fresult++) = '\n';
        LineLen = 0;
      }
      if (bufsize - count < 3)
      break;
    }
    unsigned char mid = (256 - (0 - *s));
    tmp |= mid;
    tmp <<= 8;
    count++;
    s++;
  }
  int rest = (bufsize - count) % 3;
  if (rest != 0)
  {
    tmp = 0;
    int i;
    for (i = 0; i < 3; i++)
    {
      if (i < rest)
      {
        unsigned char mid = (256 - (0 - *s));
        tmp |= mid;
        tmp |= *s;
        tmp <<= 8;
        count++;
        s++;
      }
      else
      {
        tmp |= 0;
        tmp <<= 8;
      }
    }
    tmp >>= 8;
    tmp &= 0xFFFFFF;
    int mid = tmp;
    if (rest >= 1)
    {
      mid >>= 18;
      mid &= 0x3F;
      *(fresult++) = base64chars[mid];
      mid = tmp;
      mid >>= 12;
      mid &= 0x3F;
      *(fresult++) = base64chars[mid];
    }
    if (rest >= 2)
    {
      mid = tmp;
      mid >>= 6;
      mid &= 0x3F;
      *(fresult++) = base64chars[mid];
    }
    if (rest >= 3)
    {
      mid = tmp;
      mid &= 0x3F;
      *(fresult++) = base64chars[mid];
    }
    for (int c = 3; c > rest; c--)
    {
      *(fresult++) = '=';
    }
  }
  return finalresult;
}

/*!
  Decode a Base64 coded string.
 */
char* CBase64Utils::decode (const char *input, int *bufsize)
{
  int std = 0, count = 1, resultlen = 0;
  char *finalresult = new char[*bufsize + sizeof (char)];

  const char *s = input;
  char *result = finalresult;

  while (*s != '=' && count <= *bufsize)
  {
    while (base64map[static_cast<int>(static_cast<char>(*s))] == SKIP)
    {
      if (*s != '\r' && *s != '\n')
      {
        ErrorCode = 1;
      }
      s++;
      (*bufsize)--;
      if (count >= *bufsize)
      {
        break;
      }
    }
    std |= base64map[*(s++) & 0xFF];
    std <<= 6;
    if (count % 4 == 0)
    {
      int tmp;
      std >>= 6;
      tmp = std;
      tmp >>= 16;
      tmp &= 0xFF;
      *(result++) = static_cast<char>(tmp);
      tmp = std;
      tmp >>= 8;
      tmp &= 0xFF;
      *(result++) = static_cast<char>(tmp);
      tmp = std;
      tmp &= 0xFF;
      *(result++) = static_cast<char>(tmp);
      std = 0;
      resultlen += 3;
    }
    count++;
  }
  count--;
  if (count % 4 != 0)
  {
    int tmp;
    for (int i = 0; i < 4 - (count % 4); i++)
    {
      std <<= 6;
      resultlen++;
    }
    std >>= 6;
    tmp = std;
    tmp >>= 16;
    tmp &= 0xFF;
    *(result++) = static_cast<char>(tmp);
    tmp = std;
    tmp >>= 8;
    tmp &= 0xFF;
    *(result++) = static_cast<char>(tmp);
    tmp = std;
    tmp &= 0xFF;
    *(result++) = static_cast<char>(tmp);
  }
  *bufsize = resultlen;
  return finalresult;
}
CQPUtils::CQPUtils ()
{
  ErrorCode = 0;
}

CQPUtils::~CQPUtils ()
{

}
char* CQPUtils::decode (char *input)
{
  char *s = input;
  char *finalresult = new char [strlen (input) + sizeof (char)];
  char *result = finalresult;
  while (*s != '\0')
  {
    if (*s == '=')
    {
      int i;
      for (i = 0; i < 3; i++)
      {
        if (s[i] == '\0')
        {
          ErrorCode = 1;
          return finalresult;
        }
      }
      char mid[3];
      int ok = 1;
      s++;
      for (i = 0; i < 2; i++)
      {
        if (hexmap[static_cast<int>(static_cast<char>(s[i])) ] == SKIP)
        {
          ok = 0;
          if (s[i] == '\r' && s[i + 1] == '\n')
          {
            s += 2;
            break;
          }
          else
          {
            ErrorCode = 1;
          }
        }
        mid[i] = s[i];
      }
      if (ok)
      {
        int m;
        s += 2;
        m = hexmap[static_cast<int>(static_cast<char>(mid[0]))];
        m <<= 4;
        m |= hexmap[static_cast<int>(static_cast<char>(mid[1]))];
        *(result++) = static_cast<char>(m);
      }
    }
    else
    {
      if (*s != '\0') *(result++) = *(s++);
    }
  }
  return finalresult;
}


#define BufAdd 10

char* CQPUtils::expandBuffer (char *buffer, int UsedSize, int *BufSize,
                             int Singlechar)
{
  int AddVal;
  if (Singlechar) AddVal = 3;
  else AddVal = 5;
  if (UsedSize >= *BufSize - AddVal)
  {
    *BufSize += BufAdd;
    return (char*) checked::realloc (buffer, *BufSize * sizeof (char));
  }
  return buffer;
}


char* CQPUtils::encode (char *input)
{
  int BufSize = static_cast<int>(strlen (input) + BufAdd);
  int UsedSize = 0;
  int LineLen = 0;
  char *finalresult = (char*)calloc (BufSize, sizeof (char));
  char *fresult = finalresult;
  char *s = input;
  while (*s != '\0')
  {
    unsigned char mid = (256 - (0 - *s));
    if (*s == '\n')
      LineLen = 0;
    if (QpEncodeMap[mid] == SKIP)
    {
      char mids[3];
      if (LineLen >= MaxLineLength - 4)
      {
        finalresult = expandBuffer (finalresult, UsedSize, &BufSize, 0);
        *(fresult++) = '=';
        *(fresult++) = '\r';
        *(fresult++) = '\n';
        UsedSize += 3;
        LineLen = 0;
      }
      finalresult = expandBuffer (finalresult, UsedSize, &BufSize, 0);

      gnulib::snprintf (mids, 3, "%X", mid);

      myserver_strupr (mids);
      *(fresult++) = '=';
      *(fresult++) = mids[0];
      *(fresult++) = mids[1];
      UsedSize += 3;
      LineLen += 2;
      s++;
    }
    else
    {
      if (LineLen >= MaxLineLength - 4)
      {
        finalresult = expandBuffer (finalresult, UsedSize, &BufSize, 0);
        *(fresult++) = '=';
        *(fresult++) = '\r';
        *(fresult++) = '\n';
        UsedSize += 3;
        LineLen = 0;
      }
      finalresult = expandBuffer (finalresult, UsedSize, &BufSize);
      UsedSize++;
      LineLen++;
      *(fresult++) = *(s++);
    }
  }
  *(fresult++) = '\0';
  return finalresult;
}
