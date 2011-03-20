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

#include <include/conf/security/xml_validator.h>
#include <include/conf/security/auth_domain.h>
#include <include/conf/security/security_cache.h>
#include <include/base/regex/myserver_regex.h>

XmlValidator::XmlValidator ()
{
  secCache = NULL;
}

XmlValidator::~XmlValidator ()
{
  if (secCache != NULL)
    {
      secCache->free ();
      delete secCache;
    }
}

/*!
  Get the security files cache.
 */
SecurityCache* XmlValidator::getCache (SecurityToken *st)
{
  if (!secCache)
    {
      const char *data = st->getData ("SECURITY_CACHE_NODES",
                                      MYSERVER_SERVER_CONF, NULL);

      secCache = new SecurityCache ();

      if (data)
        {
          int nodes = atoi (data);
          secCache->setMaxNodes (nodes);
        }
    }

  return secCache;
}

/*!
  Get the XML parser to use.
*/
SecurityCache::CacheNode* XmlValidator::getParser (SecurityToken* st)
{
  const char *secName;
  SecurityCache *cache = getCache (st);

  if (!cache)
    return NULL;

  secName = st->getData ("security.filename", MYSERVER_VHOST_CONF
                         | MYSERVER_SERVER_CONF, ".security.xml");

  /*TODO: check that file size is not bigger than "security.max_size".  */
  return cache->getParser (*(st->getDirectory ()), *(st->getSysDirectory ()),
                           false, secName);
}

/*!
  \see AuthMethod#getPermissionMask.
*/
int XmlValidator::getPermissionMask (SecurityToken* st)
{
  xmlNodePtr root;
  SecurityCache::CacheNode *node = getParser (st);

  if (! node)
    return 0;

  XmlParser* xmlFile = &node->parser;

  for (xmlNodePtr cur = xmlFile->getDoc ()->children; cur; cur = cur->next)
    if (cur->type == XML_ELEMENT_NODE)
      {
        for (xmlNodePtr curChild = cur->children; curChild;
             curChild = curChild->next)
          if (curChild->type == XML_ELEMENT_NODE)
            {
              root = curChild;
              break;
            }
      }

  for (xmlNodePtr cur = root; cur; cur = cur->next)
    {
      if (xmlStrcmp (cur->name, (const xmlChar *) "USER"))
        continue;

      xmlAttr *attrs = cur->properties;

      xmlChar *name = NULL;
      xmlChar *password = NULL;
      xmlChar *algorithm = NULL;
      int permissions =  getPermissions (attrs, &name, &password, &algorithm);

      if (!name || !password
          || xmlStrcmp (name, (const xmlChar *)st->getUser ().c_str ()))
        continue;

      st->setProvidedMask (permissions);

      if (algorithm)
        st->setAlgorithm ((const char*)algorithm);

      if (password)
        st->setNeededPassword ((const char*)password);

      if (AuthMethod::comparePassword ((const char *)password,
                                       st->getPassword ().c_str (),
                                       (const char *)algorithm))
        {
          st->setAuthenticated (true);
          st->setMask (permissions);
        }
      else
        {
          st->setAuthenticated (false);
          st->setMask (0);
        }

      node->decRef ();
      return st->getMask ();
    }

  node->decRef ();
  return 0;
}

/*!
  Get a permission mask from the attributes.
  \param attrs Attributes list.
  \param user The found user name.
  \param password The found password.
  \param the crypt algorithm used on password.
  \return the permissions mask.
 */
int
XmlValidator::getPermissions (xmlAttr* attrs, xmlChar** user,
                              xmlChar** password, xmlChar **algorithm)
{
  int permissions = 0;

  while (attrs)
    {
      if (user && !xmlStrcmp (attrs->name, (const xmlChar *)"name")
          && attrs->children && attrs->children->content)
        *user = attrs->children->content;

      else if (password
               && !xmlStrcmp (attrs->name, (const xmlChar *)"password")
               && attrs->children && attrs->children->content)
        *password = attrs->children->content;

      else if (algorithm
               && !xmlStrcmp (attrs->name, (const xmlChar *)"algorithm")
               && attrs->children && attrs->children->content)
        *algorithm = attrs->children->content;

      else if (!xmlStrcmp (attrs->name, (const xmlChar *)"READ")
               && attrs->children && attrs->children->content
               && !xmlStrcmp (attrs->children->content,
                              (const xmlChar *) "YES"))
        permissions |= MYSERVER_PERMISSION_READ;

      else if (!xmlStrcmp (attrs->name, (const xmlChar *)"WRITE")
               && attrs->children && attrs->children->content &&
               !xmlStrcmp (attrs->children->content, (const xmlChar *) "YES"))
        permissions |= MYSERVER_PERMISSION_WRITE;

      else if (!xmlStrcmp (attrs->name, (const xmlChar *)"EXECUTE")
               && attrs->children && attrs->children->content &&
               !xmlStrcmp (attrs->children->content, (const xmlChar *) "YES"))
        permissions |= MYSERVER_PERMISSION_EXECUTE;

      else if (!xmlStrcmp (attrs->name, (const xmlChar *)"BROWSE")
               && attrs->children && attrs->children->content
               && !xmlStrcmp (attrs->children->content,
                              (const xmlChar *) "YES"))
        permissions |= MYSERVER_PERMISSION_BROWSE;

      attrs = attrs->next;
    }

  return permissions;
}


/*!
 \see XmlValidator#getPermissionMaskImpl.
*/
int
XmlValidator::getPermissionMaskImpl (SecurityToken* st,
                               HashMap<string, SecurityDomain*> *hashedDomains,
                                     AuthMethod* authMethod)
{
  SecurityCache::CacheNode *node = getParser (st);

  if (! node)
    return 0;

  XmlParser* xmlFile = &node->parser;

  if (! xmlFile->getDoc ())
    return 0;

  for (xmlNodePtr cur = xmlFile->getDoc ()->children; cur; cur = cur->next)
    if (cur->type == XML_ELEMENT_NODE)
      {
        int cmd = -1;

        computeXmlNode (cur, st, &cmd, hashedDomains);

        /* By default return ALLOW.  */
        if (cmd == -1)
          {
            node->decRef ();
            return 1;
          }

        if (cmd == 0)
          {
            node->decRef ();
            return 0;
          }

        if (cmd == 1)
          {
            node->decRef ();
            st->setMask (MYSERVER_PERMISSION_ALL);
            return 1;
          }

      }

  node->decRef ();
  return 0;
}

/*!
  Compute the current XML node.
 */
int
XmlValidator::computeXmlNode (xmlNodePtr node,
                              SecurityToken *st,
                              int *cmd,
                              HashMap<string, SecurityDomain*> *hashedDomains)
{
  if (!node)
    return 0;

  for (xmlNodePtr cur = node->children; cur; cur = cur->next)
    {
      if (cur->type != XML_ELEMENT_NODE)
        continue;

      if (!xmlStrcmp (cur->name, (const xmlChar *) "CONDITION"))
        {
          if (doCondition (cur, hashedDomains))
            computeXmlNode (cur, st, cmd, hashedDomains);
        }
      else if (!xmlStrcmp (cur->name, (const xmlChar *) "RETURN"))
        {
          doReturn (cur, cmd, hashedDomains);
          return 1;
        }
      else if (!xmlStrcmp (cur->name, (const xmlChar *) "DEFINE"))
        doDefine (cur, st, hashedDomains);
      else if (!xmlStrcmp (cur->name, (const xmlChar *) "SET_HEADER"))
        doSetHeader (cur, st);
      else if (!xmlStrcmp (cur->name, (const xmlChar *) "PERMISSION"))
        doPermission (cur, st, hashedDomains);
    }

  return 0;
}

/*!
  Handle a CONDITION.
*/
bool XmlValidator::doCondition (xmlNodePtr node,
                                HashMap<string, SecurityDomain*> *hashedDomains)
{
  string name;
  const xmlChar *isNot = (const xmlChar*)"";
  const xmlChar *value = (const xmlChar*)"";
  const xmlChar *regex = (const xmlChar*)"";
  xmlAttr *attrs = node->properties;

  while (attrs)
    {
      if (!xmlStrcmp (attrs->name, (const xmlChar *)"name") &&
          attrs->children && attrs->children->content)
        name.assign ((const char*)attrs->children->content);

      if (!xmlStrcmp (attrs->name, (const xmlChar *)"value") &&
          attrs->children && attrs->children->content)
        value = attrs->children->content;

      if (!xmlStrcmp (attrs->name, (const xmlChar *)"not") &&
          attrs->children && attrs->children->content)
        isNot = attrs->children->content;

      if (!xmlStrcmp (attrs->name, (const xmlChar *)"regex") &&
          attrs->children && attrs->children->content)
        regex = attrs->children->content;

      attrs = attrs->next;
    }

  const string *storedValue = getValue (hashedDomains, name);
  if (!storedValue)
    return false;

  bool eq;

  if (!xmlStrcmp (regex, (const xmlChar *) "yes"))
    {
      Regex regex;

      if (regex.compile ((const char*)value, REG_EXTENDED))
        return false;

      regmatch_t pm;

      eq = regex.exec (storedValue->c_str (), 1, &pm, 0) == 0;
    }
  else
    eq = storedValue->compare ((const char*)value) == 0;

  if (!xmlStrcmp (isNot, (const xmlChar *) "yes"))
    return !eq;

  return eq;
}

/*!
  Handle a PERMISSION.
 */
void XmlValidator::doPermission (xmlNodePtr node, SecurityToken *st,
                               HashMap<string, SecurityDomain*> *hashedDomains)
{
  string name;
  xmlAttr *attrs = node->properties;

  st->setProvidedMask (getPermissions (attrs));

  if (st->isAuthenticated ())
    st->setMask (st->getProvidedMask ());
}


/*!
  Handle a DEFINE.
 */
void XmlValidator::doDefine (xmlNodePtr node, SecurityToken *st,
                             HashMap<string, SecurityDomain*> *hashedDomains)
{
  string name;
  const xmlChar *value = (const xmlChar*)"";
  xmlAttr *attrs = node->properties;

  while (attrs)
    {
      if (!xmlStrcmp (attrs->name, (const xmlChar *)"name") &&
          attrs->children && attrs->children->content)
        name.assign ((const char*)attrs->children->content);

      if (!xmlStrcmp (attrs->name, (const xmlChar *)"value") &&
          attrs->children && attrs->children->content)
        value = attrs->children->content;

      attrs = attrs->next;
    }

  if (!value)
    return;

  string valStr ((const char*)value);
  NodeTree<string> *nt = new NodeTree<string> (valStr);
  NodeTree<string> *old = st->getValues ()->put (name, nt);

  if (old)
    delete old;
}

/*!
  Handle a RETURN.
 */
void XmlValidator::doReturn (xmlNodePtr node, int *cmd,
                             HashMap<string, SecurityDomain*> *hashedDomains)
{
  xmlAttr *attrs = node->properties;
  xmlChar *value = NULL;
  while (attrs)
    {
      if (!xmlStrcmp (attrs->name, (const xmlChar *)"value") &&
          attrs->children && attrs->children->content)
        value = attrs->children->content;

      attrs = attrs->next;
    }

  if (value && !xmlStrcmp (value, (const xmlChar *) "ALLOW"))
    *cmd = 1;
  else
    *cmd = 0;
}

/*!
  Handle a SET_HEADER.
 */
void XmlValidator::doSetHeader (xmlNodePtr node, SecurityToken *st)
{
  xmlAttr *attrs = node->properties;
  const char *name = NULL;
  const char *value = NULL;
  SecurityHeaderData *headerData = st->getResponseHeaderData ();

  if (headerData == NULL)
    return;

  while (attrs)
    {
      if (!xmlStrcmp (attrs->name, (const xmlChar *) "name") &&
          attrs->children && attrs->children->content)
        name = (const char *) attrs->children->content;

      if (!xmlStrcmp (attrs->name, (const xmlChar *) "value") &&
          attrs->children && attrs->children->content)
        value = (const char *) attrs->children->content;

      attrs = attrs->next;
    }

  if (name == NULL || value == NULL)
    return;

  headerData->setValue (name, value);
}
