# GNU MyServer
#
# Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
# 2011 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AM_CXXFLAGS=-I"$(top_builddir)/lib" -I"$(top_srcdir)/lib" -I"$(top_srcdir)"

SUBDIRS = base conf connection connections_scheduler filter http_handler \
	log plugin protocol server GUI

bin_PROGRAMS = myserver
myserver_SOURCES = myserver.cpp

noinst_LTLIBRARIES = libmyserver.la
libmyserver_la_SOURCES =
libmyserver_la_LIBADD =	base/base64/libbase64.la \
		base/bitvec/libbitvec.la \
		base/crypt/libcrypt.la \
		base/dynamic_lib/libdynamic_lib.la \
		base/exceptions/libexceptions.la \
		base/file/libfile.la \
		base/files_cache/libfiles_cache.la \
		base/hash_map/libhash_map.la \
		base/home_dir/libhome_dir.la \
		base/libbase.la \
		base/mem_buff/libmem_buff.la \
		base/multicast/libmulticast.la \
		base/pipe/libpipe.la \
		base/process/libprocess.la \
		base/read_directory/libread_directory.la \
		base/regex/libregex.la \
		base/safetime/libsafetime.la \
		base/slab/libslab.la \
		base/socket/libsocket.la \
		base/socket_pair/libsocket_pair.la \
		base/ssl/libssl.la \
		base/string/libstring.la \
		base/sync/libsync.la \
		base/thread/libthread.la \
		base/unix_socket/libunix_socket.la \
		base/xml/libxml.la \
		conf/libconf.la \
		conf/main/libmain.la \
		conf/mime/libmime.la \
		conf/security/libsecurity.la \
		conf/vhost/libvhost.la \
		connection/libconnection.la \
		connections_scheduler/libconnections_scheduler.la \
		filter/gzip/libgzip.la \
		filter/libfilter.la \
		http_handler/cgi/libcgi.la \
		http_handler/fastcgi/libfastcgi.la \
		http_handler/http_dir/libhttp_dir.la \
		http_handler/http_file/libhttp_file.la \
		http_handler/isapi/libisapi.la \
		http_handler/libhttp_handler.la \
		http_handler/mscgi/libmscgi.la \
		http_handler/proxy/libproxy.la \
		http_handler/scgi/libscgi.la \
		http_handler/wincgi/libwincgi.la \
		log/liblog.la \
		log/stream/libstream.la \
		plugin/libplugin.la \
		protocol/control/libcontrol.la \
		protocol/ftp/libftp.la \
		protocol/gopher/libgopher.la \
		protocol/http/env/libenv.la \
		protocol/http/libhttp.la \
		protocol/https/libhttps.la \
		protocol/http/webdav/libwebdav.la \
		protocol/libprotocol.la \
		server/libserver.la \
		../lib/libgnu.la  \
		$(PTHREAD_LIB) $(IDN_LIB) $(XNET_LIB) $(DL_LIB) \
		$(ZLIB_LIB) $(XML_LIBS) \
		$(INTLLIBS) $(LDFLAGS)

myserver_CXXFLAGS = $(AM_CXXFLAGS) $(CXXFLAGS) -DHOST_STR=\"$(build)\" -DPREFIX=\"$(prefix)\"

myserver_LDADD = libmyserver.la $(PTHREAD_LIB) $(IDN_LIB) \
	$(XNET_LIB) $(DL_LIB) $(ZLIB_LIB) \
	$(XML_LIBS) $(INTLLIBS) $(LDFLAGS)

all-local: libmyserver.la myserver${EXEEXT}
	$(LIBTOOL) --mode=install cp myserver$(EXEEXT) $(abs_top_builddir)/binaries/

clean-local:
	$(LIBTOOL) --mode=uninstall rm -f $(abs_top_builddir)/binaries/myserver$(EXEEXT)
