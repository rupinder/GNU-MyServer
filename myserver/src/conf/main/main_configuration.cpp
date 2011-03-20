/*
  MyServer
  Copyright (C) 2009, 2010, 2011 Free Software Foundation, Inc.
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

#include <include/conf/main/main_configuration.h>


MainConfiguration::MainConfiguration ()
{

}

MainConfiguration::~MainConfiguration ()
{

}

const char *MainConfiguration::getValue (string const &field)
{
  return getValue (field.c_str ());
};


void MainConfiguration::readData (list<NodeTree<string>*> *hashedDataTrees,
                         HashMap<string, NodeTree<string>*> *hashedData)
{

}
