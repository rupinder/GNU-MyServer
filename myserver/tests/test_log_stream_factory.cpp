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
#include <list>
#include <string>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <include/base/socket/socket.h>
#include <include/log/stream/log_stream_factory.h>
#include <include/filter/filters_factory.h>

class TestLogStreamFactory : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE (TestLogStreamFactory);
  CPPUNIT_TEST (testGetProtocol);
  CPPUNIT_TEST (testGetPath);
  CPPUNIT_TEST (testCreation);
  CPPUNIT_TEST_SUITE_END ();
public:
  void setUp ()
  {
    lsf = new LogStreamFactory ();
  }

  void testGetProtocol ()
  {
    CPPUNIT_ASSERT (!lsf->protocolCheck (lsf->getProtocol ("foo")));
    CPPUNIT_ASSERT (!lsf->protocolCheck (lsf->getProtocol ("foo:bar")));
    CPPUNIT_ASSERT (!lsf->protocolCheck (lsf->getProtocol ("foo://")));
    CPPUNIT_ASSERT (!lsf->protocolCheck (lsf->getProtocol ("://")));
    CPPUNIT_ASSERT (!lsf->protocolCheck (lsf->getProtocol ("://foo")));
    CPPUNIT_ASSERT (lsf->protocolCheck (lsf->getProtocol ("file://")));
  }

  void testGetPath ()
  {
    CPPUNIT_ASSERT (!lsf->getPath ("foo:bla").size ());
    CPPUNIT_ASSERT (!lsf->getPath ("foo://bar").size ());
    CPPUNIT_ASSERT (lsf->getPath ("socket://" LOCALHOST_ADDRESS ":0").size ());
  }

  void testCreation ()
  {
    list<string> filters;
    FiltersFactory ff;
    CPPUNIT_ASSERT (!lsf->create (&ff, "foo", filters, 0));
    LogStream* ls = lsf->create (&ff, "console://", filters, 0);
    CPPUNIT_ASSERT (!ls);
    ls = lsf->create (&ff, "console://foo", filters, 0);
    CPPUNIT_ASSERT (!ls);
    ls = lsf->create (&ff, "console://stdout", filters, 0);
    CPPUNIT_ASSERT (ls);

    bool success = false;
    try
      {
        lsf->create (&ff, "socket://" LOCALHOST_ADDRESS ":0",
                     filters, 0);
      }
    catch (...)
      {
        success = true;
      }

    CPPUNIT_ASSERT (success);

    delete ls;
  }

  void tearDown ()
  {
    delete lsf;
  }
private:
  LogStreamFactory* lsf;
};

CPPUNIT_TEST_SUITE_REGISTRATION (TestLogStreamFactory);
