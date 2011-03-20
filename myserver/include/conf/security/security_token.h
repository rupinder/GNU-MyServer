/* -*- mode: c++ -*- */
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

#ifndef SECURITY_TOKEN_H
# define SECURITY_TOKEN_H

# include "myserver.h"
# include <include/base/hash_map/hash_map.h>
# include <include/conf/nodetree.h>
# include <string>

using namespace std;

class Server;
class Vhost;
struct MimeRecord;

enum SECURITY_RING
  {
    /*! Security file configuration.  */
    MYSERVER_SECURITY_CONF = (1 << 1),

    /*! Virtual host configuration.  */
    MYSERVER_VHOST_CONF = (1 << 2),

    /*! Mime type.  */
    MYSERVER_MIME_CONF = (1 << 3),

    /*! Global server configuration. */
    MYSERVER_SERVER_CONF = (1 << 4),
  };

class SecurityHeaderData
{
public:
  virtual string* setValue (const char *name, const char *in) = 0;
};

class SecurityToken
{
public:
  SecurityToken ();
  void reset ();

  const char* getData (const char *name, int domains,
                       const char *def = NULL);

  NodeTree<string>* getNodeTree (string &key, int domains,
                                 NodeTree<string> *def = NULL);

  string &getUser ()
  {
    return user;
  }

  string& getPassword ()
  {
    return password;
  }

  HashMap<string, NodeTree<string>*> *getValues ()
  {
    return &values;
  }

  const string *getDirectory ()
  {
    return directory;
  }

  const string *getSysDirectory ()
  {
    return sysdirectory;
  }

  const string *getResource ()
  {
    return resource;
  }

  int getMask ()
  {
    return mask;
  }

  int getProvidedMask ()
  {
    return providedMask;
  }

  string &getAlgorithm ()
  {
    return algorithm;
  }

  string &getNeededPassword ()
  {
    return neededPassword;
  }

  bool isDone ()
  {
    return done;
  }

  bool isAuthenticated ()
  {
    return authenticated;
  }


  Server *getServer ()
  {
    return server;
  }

  Vhost *getVhost ()
  {
    return vhost;
  }

  void setUser (string &u)
  {
    user.assign (u);
  }

  void setPassword (string &pw)
  {
    password.assign (pw);
  }

  void setDirectory (const string *d)
  {
    directory = d;
  }

  void setSysDirectory (const string *sd)
  {
    sysdirectory = sd;
  }

  void setResource (const string *r)
  {
    resource = r;
  }

  void setMask (int m)
  {
    mask = m;
  }

  void setProvidedMask (int p)
  {
    providedMask = p;
  }

  void setNeededPassword (const char *pw)
  {
    neededPassword.assign (pw);
  }

  void setNeededPassword (string &pw)
  {
    neededPassword = pw;
  }

  void setAlgorithm (string &a)
  {
    algorithm = a;
  }

  void setAlgorithm (const char *pw)
  {
    algorithm = pw;
  }

  void setDone (bool d)
  {
    done = d;
  }

  void setAuthenticated (bool a)
  {
    authenticated = a;
  }

  void setServer (Server *s)
  {
    server = s;
  }

  void  setVhost (Vhost *v)
  {
    vhost = v;
  }

  MimeRecord *getMime ()
  {
    return mimeRecord;
  }

  void setMime (MimeRecord *mr)
  {
    mimeRecord = mr;
  }

  void setResponseHeaderData (SecurityHeaderData *header)
  {
    this->responseHeader = header;
  }

  SecurityHeaderData *getResponseHeaderData ()
  {
    return responseHeader;
  }

private:
  /*! Protocol specific response header data.  */
  SecurityHeaderData *responseHeader;

  /*! User to check for.  */
  string user;

  /*! Password provided by the user.  */
  string password;

  /*! Directory that the user is in.  */
  const string *directory;

  /*! System directory for the host.  */
  const string *sysdirectory;

  /*! Resource that the user tried to access.  */
  const string *resource;

  /*! Permission mask.  */
  int mask;

  /*! Hash algorithm used to crypt the neededPassword.  */
  string algorithm;

  /*!
    Password that the user should provide to have access.
    This is used in authorization schemes like the HTTP digest,
    where the password is not sent in clear on the network.
  */
  string neededPassword;

  /*! The provided password is correct.  */
  bool authenticated;

  /*! The obtained mask if password were correct.  */
  int providedMask;

  /*! Stored values.  */
  HashMap<string, NodeTree<string>*> values;

  /*! If it is true the security manager will use the cached values.  */
  bool done;

  /*! Server instance.  */
  Server *server;

  /*! VHost.  */
  Vhost *vhost;

  MimeRecord *mimeRecord;

};

#endif
