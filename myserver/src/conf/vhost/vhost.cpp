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
#include <include/conf/vhost/vhost.h>
#include <include/base/file/file.h>
#include <include/base/file/files_utility.h>
#include <include/server/server.h>

#include <include/connection/connection.h>
#include <include/base/string/stringutils.h>
#include <include/base/string/securestr.h>
#include <include/conf/vhost/ip.h>

#ifdef HAVE_IDN
# include <stringprep.h>
# include <punycode.h>
# include <idna.h>
#endif

#include <cstdarg>

const string Vhost::accessLogType ("ACCESSLOG");
const string Vhost::warningLogType ("WARNINGLOG");


/*!
  Vhost costructor
 */
Vhost::Vhost (LogManager* lm)
{
  hostList.clear ();
  refMutex.init ();
  documentRoot.assign ("");
  systemRoot.assign ("");
  hashedData.clear ();
  protocolData = 0;

  /*
    By default use a non specified value for the throttling rate. -1 means
    that the throttling rate was not specified, while 0 means it was
    specified but there is not a limit.
   */
  throttlingRate = (u_long) -1;
  refCount = 0;
  nullReferenceCb = 0;
  defaultPriority = 0;
  logManager = lm;
  mimeHandler = NULL;
}


/*!
  Destroy the vhost.
 */
Vhost::~Vhost ()
{
  if (protocolData)
    delete protocolData;
  clearHostList ();
  clearIPList ();
  freeSSL ();
  freeHashedData ();

  HashMap<string, MimeRecord*>::Iterator it = locationsMime.begin ();
  while (it != locationsMime.end ())
    {
      delete *it;
      it++;
    }

  if (mimeHandler)
    delete mimeHandler;

  refMutex.destroy ();
  /*  mimeManager.clean ();*/
  logManager->remove (this);
}

/*!
  Clear the data dictionary.
  Returns zero on success.
 */
int Vhost::freeHashedData ()
{
  try
    {
      list<NodeTree<string>*>::iterator it = hashedDataTrees.begin ();
      while (it != hashedDataTrees.end ())
        {
          delete *it;
          it++;
        }

      hashedDataTrees.clear ();
      hashedData.clear ();
    }
  catch (...)
    {
      return 1;
    }
  return 0;
}

/*!
  Check if a MIME type file is defined for the virtual host.
 */
int Vhost::isMIME ()
{
  return mimeHandler ? 1 : 0;
}

/*!
  Get the MIME handler for the virtual host.
 */
MimeManagerHandler* Vhost::getMIME ()
{
  return mimeHandler;
}

/*!
  Clear the list of the hosts.
 */
void Vhost::clearHostList ()
{
  list<StringRegex*>::iterator i = hostList.begin ();
  while (i != hostList.end ())
    {
      StringRegex* sr = *i;
      delete sr;
      i++;
    }
  hostList.clear ();
}

/*!
  Clear the list of IPs.
 */
void Vhost::clearIPList ()
{
  list<IpRange *>::iterator it = ipListAllow.begin ();
  while (it != ipListAllow.end ())
    {
      delete (*it);
      it++;
    }
  ipListAllow.clear ();

  it = ipListDeny.begin ();
  while (it != ipListDeny.end ())
    {
      delete (*it);
      it++;
    }
  ipListDeny.clear ();
}

int
Vhost::openAccessLog (string location, list<string>& filters, u_long cycle)
{
  return logManager->add (this, accessLogType, location, filters, cycle);
}

int
Vhost::openWarningLog (string location, list<string>& filters, u_long cycle)
{
  return logManager->add (this, warningLogType, location, filters, cycle);
}

int
Vhost::openLogFiles ()
{
  return logManager->count (this, accessLogType) == 0
    || logManager->count (this, warningLogType) == 0;
}

/*!
  Add an IP address to the list.
  \param ip The ip to add.
  \param isRegex Specify if the ip is specified as a regex.
 */
void Vhost::addIP (const char *ip, int isRegex)
{
  std::string sTempIp (ip);
  IpRange *pNewRange = IpRange::RangeFactory (sTempIp);
  if ( pNewRange != NULL )
    ipListAllow.push_back (pNewRange);
}

/*!
  Remove the IP address to the list.
  \param ip The ip to remove.
 */
void Vhost::removeIP (const char *ip)
{
  std::string sTempIp (ip);
  ipListDeny.push_back (IpRange::RangeFactory (sTempIp));
}

/*!
  Remove the host address to the list.
  \param host The hostname to remove.
 */
void Vhost::removeHost (const char *host)
{
  list<StringRegex*>::iterator i = hostList.begin ();

  while (i != hostList.end ())
    {
      StringRegex* sr = *i;
      /*
        If this is the virtual host with the right IP.
       */
      if (!strcmp (sr->name, host))
        {
          hostList.erase (i);
          return;
        }
      i++;
    }
}
/*!
  Check if an host is allowed to the connection
  \param host hostname to check.
 */
int Vhost::isHostAllowed (const char* host)
{
  /* If no hosts are specified then every host is allowed to connect here.  */
  if (!hostList.size () || !host)
    return 1;

  list<StringRegex*>::iterator i = hostList.begin ();
  while (i != hostList.end ())
    {
      StringRegex* sr = *i;
      regmatch_t pm;
      if (sr->regex.isCompiled () &&!sr->regex.exec (host, 1, &pm, REG_NOTBOL))
        return 1;

      if (!strcmp (sr->name, host))
        return 1;

      i++;
    }
  return 0;
}

/*!
  Check if all the host are allowed to the connection.
 */
int Vhost::areAllHostAllowed ()
{
  if (hostList.size () == 0)
    return 1;
  return 0;
}

/*!
  Check if all the IPs are allowed to the connection.
 */
int Vhost::areAllIPAllowed ()
{
  return ipListDeny.empty () && ipListAllow.empty () ? 1 : 0;
}

/*!
  Check if the network is allowed to the connection (control the network
  by the local IP).
  \param ip The IP to check.
 */
int Vhost::isIPAllowed (const char* ip)
{
  if (areAllIPAllowed ())
    return 1;

  std::string sTempIp (ip);
  IpRange *pTempIp = IpRange::RangeFactory (sTempIp);
  list<IpRange *>::const_iterator it = ipListDeny.begin ();
  while (it != ipListDeny.end ())
    {
      if ((*it)->InRange (pTempIp) )
        {
          delete pTempIp;
          return 0;
        }
      it++;
    }
  it = ipListAllow.begin ();
  while (it != ipListAllow.end () )
    {
      if ((*it)->InRange (pTempIp))
        {
          delete pTempIp;
          return 1;
        }
      it++;
    }
  delete pTempIp;
  return 0;
}

/*!
  Add an host to the allowed host list.
  \param host hostname to add.
  \param isRegex Is the host a regex?
 */
void Vhost::addHost (const char *host, int isRegex)
{
  StringRegex* hl = new StringRegex ();
  if (hl == 0)
    return;

#ifdef HAVE_IDN
  size_t len;
  int ret;
  char* ascii = 0;
  uint32_t* ucs4 = stringprep_utf8_to_ucs4 (host, -1, &len);

  if (!ucs4)
    {
      delete hl;
      return;
    }

  ret = idna_to_ascii_4z (ucs4, &ascii, 0);

  free (ucs4);

  if (ret != IDNA_SUCCESS)
    {
      delete hl;
      return;
    }

  host = ascii;
#endif

  hl->name.assign (host);

  if (isRegex)
    hl->regex.compile (host, REG_EXTENDED);
  hostList.push_back (hl);

#ifdef HAVE_IDN
  free (ascii);
#endif

}

/*!
  Write to the accesses log.
  \param fmt The log format string.
  \param ... Arguments.
 */
int
Vhost::accessesLogWrite (const char* fmt, ...)
{
  va_list argptr;
  va_start (argptr, fmt);
  int ret = logManager->log (this, accessLogType, MYSERVER_LOG_MSG_INFO, false,
                             true, fmt, argptr);
  va_end (argptr);
  return ret;
}

/*!
  Write a line to the warnings log.
  \param fmt The log format string.
  \param ... Arguments.
 */
int
Vhost::warningsLogWrite (const char* fmt, ...)
{
  va_list argptr;
  va_start (argptr, fmt);
  int ret = logManager->log (this, warningLogType, MYSERVER_LOG_MSG_INFO, true,
                             true, fmt, argptr);
  va_end (argptr);
  return ret;
}

/*!
  Set the null reference callback function. It is called when the reference
  counter for the virtual host is zero.
  \param cb The null references callback function.
 */
void Vhost::setNullRefCB (NULL_REFERENCECB cb)
{
  nullReferenceCb = cb;
}

/*!
  Get the null reference callback function used by the virtual host.
 */
NULL_REFERENCECB Vhost::getNullRefCB ()
{
  return nullReferenceCb;
}

/*!
  Increment current references counter by 1.
 */
void Vhost::addRef ()
{
  refMutex.lock (0);
  refCount++;
  refMutex.unlock (0);
}

/*!
  Decrement current references counter by 1.
 */
void Vhost::removeRef ()
{
  refMutex.lock (0);
  refCount--;
  /*!
    If the reference count reaches zero and a callback
    function is defined call it.
   */
  if (refCount == 0 && nullReferenceCb)
    nullReferenceCb (this);
  refMutex.unlock (0);
}

/*!
  Get the references counter.
 */
int Vhost::getRef ()
{
  return refCount;
}

/*!
  Set the reference counter for the virtual host.
  \param n The new reference counter.
 */
void Vhost::setRef (int n)
{
  refCount = n;
}

/*!
  Get the value for name in the hash dictionary.
  \param name The hashed entry key.
 */
const char* Vhost::getData (const char* name)
{
  NodeTree<string> *s = hashedData.get (name);

  return s ? s->getValue ()->c_str () : 0;
}

/*!
  Initialize SSL on the virtual host.
 */
int Vhost::initializeSSL ()
{
  Protocol* protocol;

  protocol = Server::getInstance ()->getProtocol (protocolName.c_str ());
  if (!protocol)
    return 0;

  if (!(protocol->getProtocolOptions () & Protocol::SSL))
    return 0;

  return sslContext.initialize ();
}

/*!
  Clean the memory used by the SSL context.
 */
int Vhost::freeSSL ()
{
  return sslContext.free ();
}

/*!
  Add a new location->mime association.

  \param location Location where use the specified mime type.
  \param record The MIME type to use.
  \return The MIME record previously registered on the location, if any.
 */
MimeRecord *Vhost::addLocationMime (string &location, MimeRecord *record)
{
  return locationsMime.put (location, record);
}
