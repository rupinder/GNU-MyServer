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


#include "myserver.h"
#include <include/base/utility.h>
#include "include/base/socket/ssl_socket.h"

#include <include/base/exceptions/checked.h>

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#ifdef WIN32
# include <w32sock.h>
#else
# include <netdb.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
#endif

#include <sstream>

using namespace std;

class SslException : public exception
{
public:
  SslException (int error)
  {
    this->error = error;
  }

  virtual const char *what () const throw ()
  {
    return gnutls_strerror (error);
  }

protected:
  int error;
};

/*!
  Constructor of the class.
 */
SslSocket::SslSocket (Socket* sock) : Socket (sock)
{
  session = NULL;
  this->sock = sock;
  this->fd = sock->getHandle ();
}

SslSocket::SslSocket ()
{
  session = NULL;
  this->sock = NULL;
  this->fd = -1;
}

SslSocket::~SslSocket ()
{
  freeSSL ();
}

/*!
  Close the socket.
 */
int SslSocket::close ()
{
  int ret;
  if (sock)
    ret = sock->close ();
  else
    ret = Socket::close ();

  this->fd = -1;
  return ret;
}

/*!
  Shutdown the socket.
 */
int SslSocket::shutdown (int how)
{
  int ret;

  ret = gnutls_bye (session, GNUTLS_SHUT_WR);
  if (ret < 0)
    throw SslException (ret);

  checked::shutdown (fd, how);

  return ret;
}

/*!
  Send data over the socket.
  Return -1 on error.
  This routine is accessible only from the Socket class.
 */
int SslSocket::rawSend (const char* buffer, int len, int flags)
{
  int sent = 0;
  int ret;
  do
    {
      ret = gnutls_record_send (session, buffer + sent, len - sent);
      if (ret > 0)
        sent += ret;
    }
  while (ret == GNUTLS_E_INTERRUPTED || ret == GNUTLS_E_AGAIN
         || (ret > 0 && sent < len));

  if (ret < 0)
    throw SslException (ret);

  return sent;
}

/*!
  Connect the socket.
 */
int SslSocket::connect (MYSERVER_SOCKADDR* sa, int na)
{
  int ret;

  if ( sa == NULL || (sa->ss_family != AF_INET && sa->ss_family != AF_INET6) )
    return 1;
  if ( (sa->ss_family == AF_INET && na != sizeof (sockaddr_in))
#if HAVE_IPV6
       || (sa->ss_family == AF_INET6 && na != sizeof (sockaddr_in6))
#endif
       )
    return 1;

  gnutls_init (&session, GNUTLS_CLIENT);

  gnutls_priority_set_direct (session, "PERFORMANCE:+ANON-DH:!ARCFOUR-128",
                              NULL);

  gnutls_certificate_allocate_credentials (&cred);
  gnutls_credentials_set (session, GNUTLS_CRD_CERTIFICATE, cred);

  /*! Do the TCP connection.  */
  checked::connect (fd, (sockaddr *) sa, na);

#ifndef FD_TO_SOCKET
# define FD_TO_SOCKET(x) (x)
#endif

  gnutls_transport_set_ptr (session, (gnutls_transport_ptr_t) FD_TO_SOCKET (fd));

#undef FD_TO_SOCKET

  do
    {
      ret = gnutls_handshake (session);
    }
  while (ret == GNUTLS_E_AGAIN || ret == GNUTLS_E_INTERRUPTED);

  if (ret < 0)
    throw SslException (ret);

  return ret;
}

/*!
  Set the SSL context.
 */
int SslSocket::setSSLContext (gnutls_certificate_credentials_t cred,
                              gnutls_priority_t priority)
{
  this->cred = cred;
  this->priority = priority;
}

/*!
  Free the SSL connection.
 */
int SslSocket::freeSSL ()
{
  if (session != NULL)
    gnutls_deinit (session);
  session = NULL;
  return 0;
}

/*!
  SSL handshake procedure.
  Return nonzero on errors.
 */
int SslSocket::sslAccept ()
{
  int ret;
  gnutls_init (&session, GNUTLS_SERVER);
  gnutls_credentials_set (session, GNUTLS_CRD_CERTIFICATE, cred);
  gnutls_certificate_server_set_request (session, GNUTLS_CERT_REQUEST);
  gnutls_session_enable_compatibility_mode (session);
  gnutls_priority_set (session, priority);
  gnutls_credentials_set (session, GNUTLS_CRD_CERTIFICATE, cred);

#ifndef FD_TO_SOCKET
# define FD_TO_SOCKET(x) (x)
#endif

  gnutls_transport_set_ptr (session, (gnutls_transport_ptr_t) FD_TO_SOCKET (fd));

#undef FD_TO_SOCKET

  do
    {
      ret = gnutls_handshake (session);
    }
  while (ret == GNUTLS_E_AGAIN || ret == GNUTLS_E_INTERRUPTED);

  if (ret < 0)
    throw SslException (ret);

  return ret;
}


/*!
  Receive data from the socket.
  Returns -1 on errors.
 */
int SslSocket::recv (char* buffer, int len, int flags)
{
  int ret = 0;

  do
    {
      ret = gnutls_record_recv (session, buffer, len);
    }
  while (ret == GNUTLS_E_INTERRUPTED || ret == GNUTLS_E_AGAIN);

  if (ret < 0)
    throw SslException (ret);

  return ret;
}

/*!
  Returns the number of bytes waiting to be read.
 */
u_long SslSocket::bytesToRead ()
{
  size_t nBytesToRead = 0;

  nBytesToRead = gnutls_record_check_pending (session);
  if (nBytesToRead)
    return nBytesToRead;

  return Socket::bytesToRead ();
}

/*!
  Check if there is data ready to be read.
 */
int SslSocket::dataAvailable (int sec, int usec)
{
  if (bytesToRead ())
    return 1;

  return Socket::dataAvailable (sec, usec);
}
