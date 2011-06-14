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

#define TESTSERVERPEM "server.pem"
#define TESTSERVERKEY "server.key"

#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <include/base/socket/socket.h>
#include <include/base/socket/ssl_socket.h>
#include <include/base/file/file.h>
#include <include/base/file/files_utility.h>
#include <include/base/thread/thread.h>
#include <include/base/utility.h>

#include <string.h>
#ifndef WIN32
# include <errno.h>
# include <arpa/inet.h>
#endif


#include <string>
#include <iostream>
using namespace std;


static const char *serverKey = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEA5EaaswqVe0pk+4nuhwp4S+P5Xw6f8Ba/g1kXFGP3CPhRKzEQ\n\
q1JmkMl6t/AWVbr+wRz/JZ71JjHg+EDzaNNyHjtQQZL5A/DGXdgyRmSsgqJInAao\n\
WB4EIbfZ+7/UalwRNyyto2sG5T2kIrFfyHDQ7oX/fuw0JjTbR8vLCPw4pQkuWm1o\n\
JYwuh2HV2w+IEtXE1yBNDDZcy4DPRixqB8z9wN8iOHOrW9pgCaim+EB0QdrWlgae\n\
jVsrwSZNpivN6hxXupPv+I/LBrf7xDirDRxBvgZjaQgaiMtZ2SHLV2m17x6rIKXw\n\
wBV8xUAxTwcrCnvqXusZAx58Sow//9GCKpHsJQIDAQABAoIBAAJqIaEz56vfHKZ6\n\
e87q3Vy9SDoyY56PAngC2OBK1q909hiME2LZGjXq2oTzs/VZMDld/Kbq3QTTVhjH\n\
2xj6r5aLj6OwyKY7PEFYAZZq4p9ki58SThln8HH2ks+gG3eV+CYTnR7cm70iliNe\n\
8srGwrb/vyK+xLhx4WXm8d8P0RAkGRrFJ9LlFOTFqtLcptJPmvRMzIsbA3MrLJqM\n\
wbn/0g1bDWpfimKBbg+03++aIZbIlNDne/M1qNfWc8hcGuIhYRn+YShg7cP7PVpm\n\
t4qVSBDLnayBQ3gqy5A+Nn1aTk9wgOJ1DEMSvCxiiujJu8V0i9HU5kjb6CbPkIqw\n\
gve70CsCgYEA7Acm2RxMpdJBLhht4KfeVhG1CTfNkNCrCPuth/72gomrXPZ6iEQH\n\
S6AS3bO6lI+mYqZ/QX6wy/fGToyWHb/Z4EWTxb8C4gs3gcEgF40xvMCwE+e3F1cO\n\
zwSkYBNCjAlsFRsGMUB1onjeiax15LiCNSPhNwXNvzIVA1T9Bg/MfkMCgYEA95eG\n\
fbaj7aGw1E1dAMZDEQFjdkzN/UQe2NTLipNVBlJcB2pLlD0BzA88tRIQKBv4neic\n\
wdsVBygzbPtKlDJJgMn/gJjZYT9Ksyydn1aRW/Fz5E/8bV2VtSci2yNcfin69Bna\n\
x9DRRgJCGngLwMAr75kMbMZnjUVO08ynSzAjqXcCgYEA4fkC55BhjczcHJ7E6/as\n\
Nmkjk1y60fAGIEadYHvpVRcrWoiczcOb3Hq4NUxy0PMrUH2pjnIWIGHrmZ6dxoO8\n\
vCzBCu0SfIpSrUbSS9ohiV5N9fC96XmvMuJ14YnvB5WpxhcmeZcGSPRB0zurb7x6\n\
5Reh8wT6dBgl6BfBW8FafnMCgYApfxPjbgD7iuYALxcvUgtbhkgNQ8IieJBS9xdG\n\
Gntdm+Agvgf8yFzMvqEGGAjtMALzNFFRjE5x7KLx2Qwe2m7HTxOrkfKdAeabMRcZ\n\
5YBnUIiD84PzMIQu4GhPIhYWbyLIVqz8YFsyyJ4boZMWsx6qAZS7hmVEoCEeDmbb\n\
iy3u8QKBgHkzr3NcKH0mu6tyQFOkf1yKVhUoEyAE2Ew9DzYkjUCj9sEayYjS/SkR\n\
BF48eWMuYZoEFBizxmKZjiiAvvoOzCzW4oponSXR07iQWf2zwKuJvCtoWTIQ9ihP\n\
z6v2cd8NAg3tSqmpDMXS5uNan2uaRAQQbuoUfbPuEPjrLl/qjdey\n\
-----END RSA PRIVATE KEY-----\n";

static const char *serverPem = "-----BEGIN CERTIFICATE-----\n\
MIICuzCCAaWgAwIBAgIESfxd9DALBgkqhkiG9w0BAQUwADAeFw0wOTA1MDIxNDUx\n\
MzRaFw0yNTAzMTMwNTI2MjJaMAAwggEfMAsGCSqGSIb3DQEBAQOCAQ4AMIIBCQKC\n\
AQDkRpqzCpV7SmT7ie6HCnhL4/lfDp/wFr+DWRcUY/cI+FErMRCrUmaQyXq38BZV\n\
uv7BHP8lnvUmMeD4QPNo03IeO1BBkvkD8MZd2DJGZKyCokicBqhYHgQht9n7v9Rq\n\
XBE3LK2jawblPaQisV/IcNDuhf9+7DQmNNtHy8sI/DilCS5abWgljC6HYdXbD4gS\n\
1cTXIE0MNlzLgM9GLGoHzP3A3yI4c6tb2mAJqKb4QHRB2taWBp6NWyvBJk2mK83q\n\
HFe6k+/4j8sGt/vEOKsNHEG+BmNpCBqIy1nZIctXabXvHqsgpfDAFXzFQDFPBysK\n\
e+pe6xkDHnxKjD//0YIqkewlAgMBAAGjRDBCMAwGA1UdEwEB/wQCMAAwEwYDVR0l\n\
BAwwCgYIKwYBBQUHAwEwHQYDVR0OBBYEFMpRt0kdzKEb9G/ofZRz3TZmy7idMAsG\n\
CSqGSIb3DQEBBQOCAQEAkKZ9kJInOz81k16vA2/Uj+ZQNrOLWJ9eoZXrmh2J+E2E\n\
vUEnl1ZZm41PHfLYvlFS+nkE81233D2NMGUowOlu17/WZifBg2DmnXWSvMV+R1Y6\n\
uDztAuNxEVog7QS7hXshO9jmeKJIDYwH1qNhupOyQZk3TAZouaAJ5GMZ0EzxuaqM\n\
sUaM198sIaGDgLtSxO/FxP1xDkLPx8jqy0A+Y2IWloW7r+XoohvxSk+oS6shNmF8\n\
FX3J22wtVUi4Ve/XftYt6RJKd764o5WTdh/Z+RUbtusXnj3ygpI/G7fTzuPUj9uF\n\
3K2VTrZpJnbMs7+i3w/ziC/cqWRVK6Rcq3bLzTXrig==\n\
-----END CERTIFICATE-----\n";

static DEFINE_THREAD (testSslRecvClient, pParam);

struct SslThreadArg
{
  int port;
  bool success;

  /* This is valid only if !success.  */
  exception reason;
};

class TestSslSocket : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE (TestSslSocket);
  CPPUNIT_TEST (testRecv);
  CPPUNIT_TEST_SUITE_END ();

public:

  void setUp ()
  {
    initializeSSL ();
    size_t nbw;
    File f;

    f.openFile (TESTSERVERKEY,  File::WRITE | File::FILE_OPEN_ALWAYS);
    f.writeToFile (serverKey, strlen (serverKey), &nbw);
    f.close ();

    f.openFile (TESTSERVERPEM,  File::WRITE | File::FILE_OPEN_ALWAYS);
    f.writeToFile (serverPem, strlen (serverPem), &nbw);
    f.close ();
  }

  void tearDown ()
  {
    FilesUtility::deleteFile (TESTSERVERPEM);
    FilesUtility::deleteFile (TESTSERVERKEY);
  }

  void testRecv ()
  {
    gnutls_priority_t priority_cache;
    gnutls_dh_params_t dh_params;
    gnutls_certificate_credentials_t cred;

    Socket *obj = new Socket;
    SslSocket *sslObj = NULL;
    ThreadID tid;

    int optvalReuseAddr = 1;
    int port = 6543;
    MYSERVER_SOCKADDRIN sockIn = { 0 };
    int status;

    ((sockaddr_in*) (&sockIn))->sin_family = AF_INET;
    ((sockaddr_in*) (&sockIn))->sin_addr.s_addr = inet_addr (LOCALHOST_ADDRESS);
    ((sockaddr_in*) (&sockIn))->sin_port = htons (port);

    socklen_t sockInLen = sizeof (sockaddr_in);

    CPPUNIT_ASSERT (obj->socket (AF_INET, SOCK_STREAM, 0) != -1);

    CPPUNIT_ASSERT (obj->setsockopt (SOL_SOCKET, SO_REUSEADDR,
                                      (const char*) &optvalReuseAddr,
                                      sizeof (optvalReuseAddr)) != -1);


    gnutls_certificate_allocate_credentials (&cred);
    gnutls_priority_init (&priority_cache, "NORMAL", NULL);

    gnutls_dh_params_init (&dh_params);
    gnutls_dh_params_generate2 (dh_params, 1024);
    gnutls_certificate_set_dh_params (cred, dh_params);

    gnutls_certificate_set_x509_trust_file (cred, TESTSERVERPEM,
                                            SSL_FILETYPE_PEM);

    gnutls_certificate_set_x509_key_file (cred, TESTSERVERPEM,
                                          TESTSERVERKEY,
                                          GNUTLS_X509_FMT_PEM);


    /* If the port is used by another program, try a few others.  */
    do
      {
        port++;
        ((sockaddr_in*) (&sockIn))->sin_port = htons (port);
      }
    while ((status = obj->bind (&sockIn, sockInLen)) != 0 && port < 28000);

    CPPUNIT_ASSERT (status != -1);

    CPPUNIT_ASSERT (obj->listen (1) != -1);

    SslThreadArg arg;
    arg.port = port;
    CPPUNIT_ASSERT_EQUAL (Thread::create (&tid, testSslRecvClient, &arg), 0);

    Thread::wait (MYSERVER_SEC (1));

    Socket s = obj->accept (&sockIn, &sockInLen);

    sslObj = new SslSocket (&s);
    sslObj->setSSLContext (cred, priority_cache);

    int ret = sslObj->sslAccept ();
    if (ret < 0)
      {
        delete obj;
        delete sslObj;
        CPPUNIT_ASSERT (false);
      }

    char buf[32] = {0};

    ret = sslObj->recv (buf, sizeof (buf), 0);

    sslObj->send ("a", 1, 0);

    CPPUNIT_ASSERT (ret != -1);

    CPPUNIT_ASSERT (sslObj->close () != -1);

    CPPUNIT_ASSERT (obj->close () != -1);

    Thread::join (tid);
    if (!arg.success)
      throw arg.reason;

    gnutls_certificate_free_credentials (cred);
    gnutls_priority_deinit (priority_cache);

    delete obj;
    delete sslObj;
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION (TestSslSocket);

static DEFINE_THREAD (testSslRecvClient, pParam)
{
  SslSocket *sslClient = NULL;
  int err;
  SslThreadArg *arg = (SslThreadArg *) pParam;
  MYSERVER_SOCKADDRIN sockIn = { 0 };

  ((sockaddr_in*) (&sockIn))->sin_family = AF_INET;
  ((sockaddr_in*) (&sockIn))->sin_addr.s_addr = inet_addr (LOCALHOST_ADDRESS);
  ((sockaddr_in*) (&sockIn))->sin_port = htons (arg->port);

  int sockInLen = sizeof (struct sockaddr_in);

  arg->success = true;

  sslClient = new SslSocket ();

  try
    {
      err = sslClient->socket (AF_INET, SOCK_STREAM, 0);
      CPPUNIT_ASSERT (err != -1);

      err = sslClient->connect (&sockIn, sockInLen);
      CPPUNIT_ASSERT (err != -1);

      char buf[] = "Works?\n";

      err = sslClient->send (buf, strlen (buf), 0);
      sslClient->recv (buf, 1, 0);

      CPPUNIT_ASSERT (err != -1);
      CPPUNIT_ASSERT (sslClient->close () != -1);
      delete sslClient;
    }
  catch (exception& e)
    {
      arg->success = false;
      arg->reason = e;
    }
  catch (...)
    {
    }

  return 0;
}
