/*
*MyServer
*Copyright (C) 2002,2003,2004 The MyServer Team
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef CONTROLHEADER_H
#define CONTROLHEADER_H
#include "../stdafx.h"
#include "../include/protocol.h"

class ControlHeader
{
  char connection[32];
  char command[32];
  char cmdOptions[64];
  char authLogin[64];
  char authPassword[64];
  char version[12];
  int length;
public:
  void reset();
  char *getOptions();
  char *getVersion();
  char *getConnection();
  int getLength();
  char *getAuthLogin();
  char *getAuthPassword();
  char *getCommand();
  ControlHeader();
  virtual ~ControlHeader();
  int parse_header(char *buffer, int bufferlen, int*);
};

#endif
