/* -*- mode: c++ -*- */
/*
  MyServer
  Copyright (C) 2002, 2003, 2004, 2006, 2007, 2008, 2009, 2010, 2011
  Free Software Foundation, Inc.
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

#ifndef SSL_SOCKET_H
# define SSL_SOCKET_H

# include "myserver.h"

# include <sys/types.h>
# include <sys/socket.h>


# include <include/base/socket/socket.h>
# include <include/base/ssl/ssl.h>

# include <string>
using namespace std;

# include <gnutls/openssl.h>

class SslSocket : public Socket
{
public:

  int setSSLContext (gnutls_certificate_credentials_t, gnutls_priority_t);
  int sslAccept ();

  int freeSSL ();

  virtual int close ();
  virtual int shutdown (int how);
  virtual int connect (MYSERVER_SOCKADDR* sa, int na);

  using Socket::recv;
  virtual int recv (char* buffer, int len, int flags);

  virtual int rawSend (const char* buffer, int len, int flags);
  virtual u_long bytesToRead ();

# ifdef __HURD__
  virtual int dataAvailable (int sec = 1, int usec = 500);
# else
  virtual int dataAvailable (int sec = 0, int usec = 500);
# endif

  SslSocket ();
  SslSocket (Socket*);
  virtual ~SslSocket ();

protected:
  gnutls_certificate_credentials_t cred;
  gnutls_session_t session;
  gnutls_priority_t priority;
  Socket *sock;
};


#endif
