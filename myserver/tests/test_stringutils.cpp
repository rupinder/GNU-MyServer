/*
  MyServer
  Copyright (C) 2008, 2009, 2010, 2011 Free Software Foundation, Inc.
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
#include "include/base/string/stringutils.h"
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <include/base/file/files_utility.h>
#include <string.h>

#include <string>

using namespace std;

class TestStringUtils : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE ( TestStringUtils );
  CPPUNIT_TEST (testTranslateEscapeString);
  CPPUNIT_TEST (testHexToInt);
  CPPUNIT_TEST (testHexVal);
  CPPUNIT_TEST (testTrim);
  CPPUNIT_TEST (testTrimLeft);
  CPPUNIT_TEST (testTrimRight);
  CPPUNIT_TEST (testGetTime);
  CPPUNIT_TEST (testGetRFC822GMTTime);
  CPPUNIT_TEST_SUITE_END ();

public:
  void setUp () {}
  void tearDown () {}

  void testTrim ()
  {
    char buffer[1024];
#define TEST_TRIM(A, B, C)                      \
    strcpy (buffer, A);                         \
    trim (buffer, B);                           \
    CPPUNIT_ASSERT (! strcmp (buffer, C));

    TEST_TRIM ("hello ", " ", "hello");
    TEST_TRIM ("hello ", "o ", "hell");
    TEST_TRIM (" hello ", " ", "hello");

#undef TEST_TRIM
  }

  void testTrimLeft ()
  {
    char buffer[1024];
#define TEST_TRIM(A, B, C)                          \
    strcpy (buffer, A);                             \
    CPPUNIT_ASSERT (! strcmp (trimLeft (buffer, B).c_str (), C));

    TEST_TRIM ("hello ", " ", "hello ");
    TEST_TRIM ("hello ", "o ", "hello ");
    TEST_TRIM (" hello ", " ", "hello ");

#undef TEST_TRIM
  }

  void testTrimRight ()
  {
    char buffer[1024];
#define TEST_TRIM(A, B, C)                                          \
    strcpy (buffer, A);                                             \
    CPPUNIT_ASSERT (! strcmp (trimRight (buffer, B).c_str (), C));

    TEST_TRIM ("hello ", " ", "hello");
    TEST_TRIM ("  hello ", "o ", "  hell");
    TEST_TRIM (" hello ", " ", " hello");

#undef TEST_TRIM
  }

  void testGetRFC822GMTTime ()
  {
    time_t epoch = 0;
    string out;
    const char* ret = getRFC822GMTTime (epoch, out);
    CPPUNIT_ASSERT (! strcasecmp (ret, "Thu, 01 Jan 1970 00:00:00 GMT"));
    CPPUNIT_ASSERT (! strcasecmp (out.c_str (), "Thu, 01 Jan 1970 00:00:00 GMT"));

    ret = getRFC822GMTTime (1009972800, out);
    CPPUNIT_ASSERT (! strcasecmp (ret, "Wed, 02 Jan 2002 12:00:00 GMT"));
    CPPUNIT_ASSERT (! strcasecmp (out.c_str (), "Wed, 02 Jan 2002 12:00:00 GMT"));
  }

  void testGetTime ()
  {
    string out;
    time_t epoch = 0;
    const char* ret = getRFC822LocalTime (epoch, out);

    CPPUNIT_ASSERT_EQUAL (getTime (ret), epoch);
  }

  void testHexVal ()
  {
    CPPUNIT_ASSERT_EQUAL (hexVal ('A'), 10);
    CPPUNIT_ASSERT_EQUAL (hexVal ('a'), 10);
    CPPUNIT_ASSERT_EQUAL (hexVal ('B'), 11);
    CPPUNIT_ASSERT_EQUAL (hexVal ('b'), 11);
    CPPUNIT_ASSERT_EQUAL (hexVal ('1'), 1);
    CPPUNIT_ASSERT_EQUAL (hexVal ('9'), 9);
    CPPUNIT_ASSERT_EQUAL (hexVal ('0'), 0);
  }

  void testHexToInt ()
  {
    CPPUNIT_ASSERT_EQUAL (hexToInt ("A"), 10);
    CPPUNIT_ASSERT_EQUAL (hexToInt ("a"), 10);
    CPPUNIT_ASSERT_EQUAL (hexToInt ("1"), 1);
    CPPUNIT_ASSERT_EQUAL (hexToInt ("10"), 1 << 4);
    CPPUNIT_ASSERT_EQUAL (hexToInt ("1A"), (1 << 4) + 10);
    CPPUNIT_ASSERT_EQUAL (hexToInt ("Aa"), (10 << 4) + 10);
  }

  void testTranslateEscapeString ()
  {
    char buffer[1024];
    strcpy (buffer, "%20");
    translateEscapeString (buffer);
    CPPUNIT_ASSERT (translateSingleEscapeString ("%20", " "));
    CPPUNIT_ASSERT (translateSingleEscapeString ("+", " "));
    CPPUNIT_ASSERT (translateSingleEscapeString ("foo%20", "foo "));
    CPPUNIT_ASSERT (translateSingleEscapeString ("foo+", "foo "));
    CPPUNIT_ASSERT (translateSingleEscapeString ("fooo+", "fooo "));
    CPPUNIT_ASSERT (translateSingleEscapeString ("%301%323%34", "01234"));
    CPPUNIT_ASSERT (translateSingleEscapeString ("%c3%a2%c2%9c%c2%88", "✈"));
    CPPUNIT_ASSERT (translateSingleEscapeString ("%c3%a2%c2%9c%c2%88%c3%a2%c2%9c%c2%88", "✈✈"));
  }

private:
  bool translateSingleEscapeString (char const *in, char const *desired)
  {
    char *buffer = gnulib::strdup (in);
    if (! buffer)
      return false;

    translateEscapeString (buffer);
    bool ret = ! strcmp (buffer, desired);
    free (buffer);

    if (! ret)
      return false;

    string strBuffer (in);
    translateEscapeString (strBuffer);
    ret = ! strcmp (strBuffer.c_str (), desired);

    return ret;
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION ( TestStringUtils );
