# -*- coding: utf-8 -*-
'''
MyServer
Copyright (C) 2009, 2011 Free Software Foundation, Inc.
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
'''

from lxml import etree
from log import Log

class VHost():
    def __init__(self, name = None, port = None, protocol = None,
                 doc_root = None, sys_root = None, logs = [],
                 ip = [], host = {}, private_key = None, certificate = None):
        '''Create new instance of VHost. logs and ip are expected to be a
        collection and host is expected to be a dict {name: useRegex} where None
        means not set.'''
        self.set_name(name)
        self.set_port(port)
        self.set_protocol(protocol)
        self.set_doc_root(doc_root)
        self.set_sys_root(sys_root)
        self.logs = []
        for log in logs:
            self.add_log(log)
        self.ip = set()
        for ip_address in ip:
            self.add_ip(ip_address)
        self.host = {}
        for single_host in host.iteritems():
            self.add_host(single_host[0], single_host[1])
        self.set_private_key(private_key)
        self.set_certificate(certificate)
        self.custom = []
        self.custom_attrib = {}

    def __eq__(self, other):
        return isinstance(other, VHost) and self.name == other.name and \
            self.port == other.port and self.protocol == other.protocol and \
            self.doc_root == other.doc_root and \
            self.sys_root == other.sys_root and \
            self.logs == other.logs and self.ip == other.ip and \
            self.host == other.host and \
            self.private_key == other.private_key and \
            self.certificate == other.certificate

    def get_name(self):
        '''Get VHost name.'''
        return self.name

    def set_name(self, name):
        '''Set VHost name.'''
        self.name = name

    def get_doc_root(self):
        '''Get VHost doc root.'''
        return self.doc_root

    def set_doc_root(self, doc_root):
        '''Set VHost doc root.'''
        self.doc_root = doc_root

    def get_sys_root(self):
        '''Get VHost sys root.'''
        return self.sys_root

    def set_sys_root(self, sys_root):
        '''Set VHost sys root.'''
        self.sys_root = sys_root

    def get_protocol(self):
        '''Get VHost protocol.'''
        return self.protocol

    def set_protocol(self, protocol):
        '''Set VHost protocol.'''
        self.protocol = protocol

    def get_port(self):
        '''Get VHost port.'''
        return self.port

    def set_port(self, port):
        '''Set VHost port.'''
        if port is None:
            self.port = None
        else:
            self.port = int(port)

    def get_logs(self):
        '''Get list of logs.'''
        return self.logs

    def get_log(self, index):
        '''Get index-th log.'''
        return self.logs[index]

    def remove_log(self, index):
        '''Remove index-th log.'''
        self.logs.pop(index)

    def add_log(self, log, index = None):
        '''Add log to VHost's logs, if index is None append it, otherwise insert
        at index position.'''
        if index is None:
            self.logs.append(log)
        else:
            self.logs.insert(index, log)

    def get_ip(self):
        '''Get VHost ip set.'''
        return self.ip

    def add_ip(self, ip):
        '''Add ip to VHost ip set.'''
        self.ip.add(ip)

    def remove_ip(self, ip):
        '''Remove ip from VHost ip set.'''
        self.ip.remove(ip)

    def get_host(self):
        '''Get VHost host dict.'''
        return self.host

    def add_host(self, host, use_regex = None):
        '''Add host to VHost host dict.'''
        self.host[host] = use_regex

    def remove_host(self, host):
        '''Remove host from VHost host dict.'''
        self.host.pop(host)

    def get_private_key(self):
        '''Get vhost private ssl key.'''
        return self.private_key

    def set_private_key(self, private_key):
        '''Set vhost private ssl key.'''
        self.private_key = private_key

    def get_certificate(self):
        '''Get vhost ssl certificate.'''
        return self.certificate

    def set_certificate(self, certificate):
        '''Set vhost ssl certificate.'''
        self.certificate = certificate

    def __str__(self):
        return etree.tostring(self.to_lxml_element(), pretty_print = True)

    def to_lxml_element(self):
        '''Convert to instance of lxml.etree.Element.'''
        def make_element(tag, text):
            element = etree.Element(tag)
            element.text = text
            return element
        root = etree.Element('VHOST')
        if self.name is not None:
            root.append(make_element('NAME', self.name))
        if self.port is not None:
            root.append(make_element('PORT', str(self.port)))
        if self.protocol is not None:
            root.append(make_element('PROTOCOL', self.protocol))
        if self.doc_root is not None:
            root.append(make_element('DOCROOT', self.doc_root))
        if self.sys_root is not None:
            root.append(make_element('SYSROOT', self.sys_root))
        if self.private_key is not None:
            root.append(make_element('SSL_PRIVATEKEY', self.private_key))
        if self.certificate is not None:
            root.append(make_element('SSL_CERTIFICATE', self.certificate))
        for ip_address in self.ip:
            root.append(make_element('IP', ip_address))
        for host, use_regex in self.host.iteritems():
            element = make_element('HOST', host)
            if use_regex is not None:
                element.set('useRegex', 'YES' if use_regex else 'NO')
            root.append(element)
        for log in self.logs:
            root.append(log.to_lxml_element())

        for element in self.custom:
            root.append(element)
        for key, value in self.custom_attrib.iteritems():
            root.set(key, value)

        return root

    @staticmethod
    def from_lxml_element(root):
        '''Factory to produce VHost from lxml.etree.Element object.'''
        if root.tag != 'VHOST':
            raise AttributeError('Expected VHOST tag.')
        custom_attrib = root.attrib
        name = None
        port = None
        protocol = None
        doc_root = None
        sys_root = None
        private_key = None
        certificate = None
        ip = []
        host = {}
        logs = []
        custom = []
        for child in list(root):
            if child.tag == 'NAME':
                name = child.text
            elif child.tag == 'PORT':
                port = child.text
            elif child.tag == 'PROTOCOL':
                protocol = child.text
            elif child.tag == 'DOCROOT':
                doc_root = child.text
            elif child.tag == 'SYSROOT':
                sys_root = child.text
            elif child.tag == 'IP':
                ip.append(child.text)
            elif child.tag == 'HOST':
                use_regex = child.get('useRegex', None)
                if use_regex is not None:
                    use_regex = use_regex.upper() == 'YES'
                host[child.text] = use_regex
            elif child.tag == 'SSL_PRIVATEKEY':
                private_key = child.text
            elif child.tag == 'SSL_CERTIFICATE':
                certificate = child.text
            else:
                try:
                    logs.append(Log.from_lxml_element(child))
                except AttributeError:
                    custom.append(child)
        vhost = VHost(name, port, protocol, doc_root, sys_root, logs, ip, host,
                      private_key, certificate)
        vhost.custom = custom
        vhost.custom_attrib = custom_attrib
        return vhost

    @staticmethod
    def from_string(text):
        '''Factory to produce VHost by parsing a string.'''
        return VHost.from_lxml_element(etree.XML(
                text, parser = etree.XMLParser(remove_comments = True)))

class VHosts():
    def __init__(self, VHosts):
        '''Create a new instance of VHosts. VHosts attribute is expected to be a
        list.'''
        self.VHosts = VHosts
        self.custom = []
        self.custom_attrib = {}

    def __eq__(self, other):
        return isinstance(other, VHosts) and self.VHosts == other.VHosts

    def to_lxml_element(self):
        '''Convert to lxml.etree.Element.'''
        root = etree.Element('VHOSTS')
        for vhost in self.VHosts:
            root.append(vhost.to_lxml_element())

        for element in self.custom:
            root.append(element)
        for key, value in self.custom_attrib.iteritems():
            root.set(key, value)

        return root

    def __str__(self):
        return etree.tostring(self.to_lxml_element(), pretty_print = True)

    @staticmethod
    def from_lxml_element(root):
        '''Factory to produce VHosts from lxml.etree.Element object.'''
        if root.tag != 'VHOSTS':
            raise AttributeError('Expected VHOSTS tag.')
        vhosts = []
        custom = []
        for element in list(root):
            try:
                vhosts.append(VHost.from_lxml_element(element))
            except AttributeError:
                custom.append(element)
        vhosts = VHosts(vhosts)
        vhosts.custom = custom
        vhosts.custom_attrib = root.attrib
        return vhosts

    @staticmethod
    def from_string(text):
        '''Factory to produce VHosts from parsing a string.'''
        return VHosts.from_lxml_element(etree.XML(
                text, parser = etree.XMLParser(remove_comments = True)))
