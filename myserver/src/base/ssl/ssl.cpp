/*
  MyServer
  Copyright (C) 2007, 2008, 2009, 2010, 2011 Free Software Foundation,
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

#include "myserver.h"

#include <include/base/ssl/ssl.h>
#include <include/base/file/files_utility.h>
#include <include/base/sync/mutex.h>

#include <string.h>

#include <include/base/exceptions/checked.h>

#if HAVE_PTHREAD
# include <pthread.h>
#endif

#if HAVE_LIBGCRYPT
# include <errno.h>
# ifdef WIN32
#  undef socklen_t
# endif
# include <gcrypt.h>
# if HAVE_PTHREAD
  /* Hack.  */
# undef malloc
# define malloc gnulib::malloc
GCRY_THREAD_OPTION_PTHREAD_IMPL;
# undef malloc
# endif
#endif


SslContext::SslContext ()
{
  cred = NULL;
  priority_cache = NULL;
  certificateFile.assign ("");
  privateKeyFile.assign ("");
}

/*!
  Initialize SSL on the virtual host.
 */
int SslContext::initialize ()
{
  /*
    The specified file doesn't exist.
   */
  if (FilesUtility::nodeExists (certificateFile.c_str ()) == 0
      || FilesUtility::nodeExists (privateKeyFile) == 0)
    return -1;

  gnutls_priority_init (&priority_cache, "NORMAL", NULL);

  gnutls_certificate_allocate_credentials (&cred);

  gnutls_certificate_set_x509_trust_file (cred, certificateFile.c_str (),
                                          SSL_FILETYPE_PEM);

  /*TODO*/
  gnutls_certificate_set_x509_crl_file (cred, "", GNUTLS_X509_FMT_PEM);

  gnutls_certificate_set_x509_key_file (cred, certificateFile.c_str (),
                                        privateKeyFile.c_str (),
                                        GNUTLS_X509_FMT_PEM);


  gnutls_dh_params_init (&dh_params);
  gnutls_dh_params_generate2 (dh_params, 1024);
  gnutls_certificate_set_dh_params (cred, dh_params);

  return 1;
}

int SslContext::free ()
{
  if (cred)
    gnutls_certificate_free_credentials (cred);
  cred = NULL;

  if (priority_cache)
    gnutls_priority_deinit (priority_cache);
  priority_cache = NULL;
  return 0;
}

#if !HAVE_LIBGCRYPT || !HAVE_PTHREAD

static int gcry_lock (void **mutex)
{
  return ((Mutex *) *mutex)->lock ();
}

static int gcry_unlock (void **mutex)
{
  return ((Mutex *) *mutex)->unlock ();
}

static int gcry_init (void **mutex)
{
  *mutex = new Mutex ();
  return 0;
}

static int gcry_destroy (void **mutex)
{
  delete (Mutex *) *mutex;
  return 0;
}

static struct gcry_thread_cbs myserver_gcry_cbs =
  {
    GCRY_THREAD_OPTION_USER,
    NULL,
    gcry_init,
    gcry_destroy,
    gcry_lock,
    gcry_unlock
  };

#endif

void initializeSSL ()
{
  static bool initialized = false;

  if (!initialized)
    {
#if HAVE_LIBGCRYPT && HAVE_PTHREAD
      gcry_control (GCRYCTL_SET_THREAD_CBS, &gcry_threads_pthread);
      gcry_control (GCRYCTL_INITIALIZATION_FINISHED);
#else
      gcry_control (GCRYCTL_SET_THREAD_CBS, &myserver_gcry_cbs);
      gcry_control (GCRYCTL_INITIALIZATION_FINISHED);
#endif
      gnutls_global_init ();
      initialized = true;
    }
}

void cleanupSSL ()
{

}
