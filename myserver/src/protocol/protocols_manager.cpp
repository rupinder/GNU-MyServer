/*
MyServer
Copyright (C) 2002, 2003, 2004, 2007, 2008, 2009 Free Software Foundation, Inc.
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


#include <include/protocol/protocols_manager.h>
#include <string>
#include <algorithm>
#include <cctype>

/*!
 * Class constructor.
 */
ProtocolsManager::ProtocolsManager ()
{

}

/*!
 * Class destructor.
 */
ProtocolsManager::~ProtocolsManager ()
{

}

/*!
 * Return a protocol by its name.
 */
Protocol* ProtocolsManager::getProtocol (string& name)
{
  return staticProtocols.get (name);
}

/*!
 * Add a static protocol to the list.
 */
void ProtocolsManager::addProtocol (string& name, Protocol* protocol)
{
  std::transform (name.begin (), name.end (), name.begin (),
                  static_cast < int(*)(int) > (tolower));

  staticProtocols.put (name, protocol);
  staticProtocolsList.push_back (protocol);
}


