/*
*MyServer
*Copyright (C) 2002 The MyServer Team
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef CLIENTSTHREAD_H
#define CLIENTSTHREAD_H
#include "../stdafx.h"
#include "../include/utility.h"
#include "../include/connectionstruct.h"
#include "../include/security.h"
#include "../include/http.h"
#include "../include/MemBuf.h"
#include "../include/https.h"

class  ClientsTHREAD
{
	friend class cserver;
#ifdef WIN32
	friend  unsigned int __stdcall startClientsTHREAD(void* pParam);
#endif
#ifdef HAVE_PTHREAD
	friend  void* startClientsTHREAD(void* pParam);
#endif
#ifdef WIN32
	friend LRESULT CALLBACK MainWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
#endif
private:
	int working;
	int initialized;
	u_long id;
	int err;
	int threadIsRunning;
	int threadIsStopped;
	u_long buffersize;
	u_long buffersize2;
	http *http_parser;
	https *https_parser;
	int isRunning();
	int isStopped();
	CMemBuf buffer;
	CMemBuf buffer2;
	void controlConnections();
	u_long nBytesToRead;
public:
	CMemBuf *GetBuffer();
	CMemBuf *GetBuffer2();
	const static u_long ID_OFFSET = 200;
	ClientsTHREAD();
	~ClientsTHREAD();
	void stop();
	void clean();	
};
#ifdef WIN32
unsigned int __stdcall startClientsTHREAD(void* pParam); 
#endif
#ifdef HAVE_PTHREAD
void* startClientsTHREAD(void* pParam);
#endif

#endif
