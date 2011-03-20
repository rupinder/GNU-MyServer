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

from security import SecurityElement, User, Condition, Permission, Return, SecurityList
from definition import Definition
from lxml import etree
import unittest

class SecurityElementTest(unittest.TestCase):
    def test_creation(self):
        element = SecurityElement()

    def test_from_string(self):
        text = '<USER />'
        element = SecurityElement.from_string(text)
        self.assertTrue(isinstance(element, User))
        text = '<CONDITION />'
        element = SecurityElement.from_string(text)
        self.assertTrue(isinstance(element, Condition))
        text = '<PERMISSION />'
        element = SecurityElement.from_string(text)
        self.assertTrue(isinstance(element, Permission))
        text = '<RETURN />'
        element = SecurityElement.from_string(text)
        self.assertTrue(isinstance(element, Return))
        text = '<DEFINE />'
        element = SecurityElement.from_string(text)
        self.assertTrue(isinstance(element, Definition))

class ConditionTest(unittest.TestCase):
    def test_creation(self):
        condition = Condition()
        condition = Condition('name')
        condition = Condition('name', 'value')
        condition = Condition('name', 'value', False)
        condition = Condition('name', 'value', False, False)
        condition = Condition('name', 'value', False, False, [])

    def test_name(self):
        condition = Condition('name1')
        self.assertEqual('name1', condition.get_name())
        condition.set_name('name2')
        self.assertEqual('name2', condition.get_name())
        condition.set_name(None)
        self.assertEqual(None, condition.get_name())
        condition = Condition(name = 'name1')
        self.assertEqual('name1', condition.get_name())

    def test_value(self):
        condition = Condition('name1', 'value1')
        self.assertEqual('value1', condition.get_value())
        condition.set_value('value2')
        self.assertEqual('value2', condition.get_value())
        condition.set_value(None)
        self.assertEqual(None, condition.get_value())
        condition = Condition(value = 'value1')
        self.assertEqual('value1', condition.get_value())

    def test_reverse(self):
        condition = Condition('name1', 'value1', False)
        self.assertFalse(condition.get_reverse())
        condition.set_reverse(True)
        self.assertTrue(condition.get_reverse())
        condition.set_reverse(None)
        self.assertEqual(None, condition.get_reverse())
        condition = Condition(reverse = True)
        self.assertTrue(condition.get_reverse())

    def test_regex(self):
        condition = Condition('name1', 'value1', False, False)
        self.assertFalse(condition.get_regex())
        condition.set_regex(True)
        self.assertTrue(condition.get_regex())
        condition.set_regex(None)
        self.assertEqual(None, condition.get_regex())
        condition = Condition(regex = True)
        self.assertTrue(condition.get_regex())

    def test_sub_elements(self):
        element_0 = Condition()
        element_1 = Return()
        condition = Condition()
        self.assertEqual(0, len(condition.get_sub_elements()))
        condition.add_sub_element(element_0)
        self.assertEqual(1, len(condition.get_sub_elements()))
        self.assertEqual(element_0, condition.get_sub_element(0))
        condition.add_sub_element(element_1)
        self.assertEqual(2, len(condition.get_sub_elements()))
        self.assertEqual(element_0, condition.get_sub_element(0))
        self.assertEqual(element_1, condition.get_sub_element(1))
        condition.remove_sub_element(0)
        self.assertEqual(1, len(condition.get_sub_elements()))
        self.assertEqual(element_1, condition.get_sub_element(0))
        condition.add_sub_element(element_0, 0)
        self.assertEqual(2, len(condition.get_sub_elements()))
        self.assertEqual(element_0, condition.get_sub_element(0))
        self.assertEqual(element_1, condition.get_sub_element(1))
        self.assertRaises(IndexError, condition.get_sub_element, 2)
        condition = Condition(sub_elements = [element_0, element_1])
        self.assertEqual(2, len(condition.get_sub_elements()))
        self.assertEqual(element_0, condition.get_sub_element(0))
        self.assertEqual(element_1, condition.get_sub_element(1))

    def test_equality(self):
        self.assertEqual(
            Condition('name1', 'value1', False, False, []),
            Condition('name1', 'value1', False, False, []))
        self.assertNotEqual(
            Condition('name1', 'value1', False, False, []),
            Condition('name2', 'value1', False, False, []))
        self.assertNotEqual(
            Condition('name1', 'value1', False, False, []),
            Condition('name1', 'value2', False, False, []))
        self.assertNotEqual(
            Condition('name1', 'value1', False, False, []),
            Condition('name1', 'value1', True, False, []))
        self.assertNotEqual(
            Condition('name1', 'value1', False, False, []),
            Condition('name1', 'value1', False, True, []))
        self.assertNotEqual(
            Condition('name1', 'value1', False, False, []),
            Condition('name1', 'value1', False, False, [Condition()]))
        self.assertNotEqual(Condition(), 'another type')

    def test_from_string(self):
        text = '<CONDITION />'
        condition = Condition.from_string(text)
        right = Condition()
        self.assertEqual(condition, right)

    def test_from_string_name(self):
        text = '<CONDITION name="test" />'
        condition = Condition.from_string(text)
        right = Condition(name = 'test')
        self.assertEqual(condition, right)

    def test_from_string_value(self):
        text = '<CONDITION value="test" />'
        condition = Condition.from_string(text)
        right = Condition(value = 'test')
        self.assertEqual(condition, right)

    def test_from_string_reverse(self):
        text = '<CONDITION not="yes" />'
        condition = Condition.from_string(text)
        right = Condition(reverse = True)
        self.assertEqual(condition, right)

    def test_from_string_regex(self):
        text = '<CONDITION regex="yes" />'
        condition = Condition.from_string(text)
        right = Condition(regex = True)
        self.assertEqual(condition, right)

    def test_from_string_sub_elements(self):
        text = '<CONDITION><PERMISSION /><RETURN /></CONDITION>'
        condition = Condition.from_string(text)
        right = Condition(sub_elements = [Permission(), Return()])
        self.assertEqual(condition, right)

    def test_from_string_full(self):
        text = '''
<CONDITION name="name" value="value" not="yes" regex="yes">
  <PERMISSION />
  <RETURN />
</CONDITION>'''
        condition = Condition.from_string(text)
        right = Condition('name', 'value', True, True,
                          [Permission(), Return()])
        self.assertEqual(condition, right)

    def test_from_lxml(self):
        text = '<CONDITION />'
        condition = Condition.from_lxml_element(etree.XML(text))
        right = Condition()
        self.assertEqual(condition, right)

    def test_from_lxml_name(self):
        text = '<CONDITION name="test" />'
        condition = Condition.from_lxml_element(etree.XML(text))
        right = Condition(name = 'test')
        self.assertEqual(condition, right)

    def test_from_lxml_value(self):
        text = '<CONDITION value="test" />'
        condition = Condition.from_lxml_element(etree.XML(text))
        right = Condition(value = 'test')
        self.assertEqual(condition, right)

    def test_from_lxml_reverse(self):
        text = '<CONDITION not="yes" />'
        condition = Condition.from_lxml_element(etree.XML(text))
        right = Condition(reverse = True)
        self.assertEqual(condition, right)

    def test_from_lxml_regex(self):
        text = '<CONDITION regex="yes" />'
        condition = Condition.from_lxml_element(etree.XML(text))
        right = Condition(regex = True)
        self.assertEqual(condition, right)

    def test_from_lxml_sub_elements(self):
        text = '<CONDITION><PERMISSION /><RETURN /></CONDITION>'
        condition = Condition.from_lxml_element(etree.XML(text))
        right = Condition(sub_elements = [Permission(), Return()])
        self.assertEqual(condition, right)

    def test_from_lxml_full(self):
        text = '''
<CONDITION name="name" value="value" not="yes" regex="yes">
  <PERMISSION />
  <RETURN />
</CONDITION>'''
        condition = Condition.from_lxml_element(etree.XML(text))
        right = Condition('name', 'value', True, True,
                          [Permission(), Return()])
        self.assertEqual(condition, right)

    def test_to_string(self):
        condition = Condition()
        copy = Condition.from_string(str(condition))
        self.assertEqual(condition, copy)

    def test_to_string_name(self):
        condition = Condition(name = 'name')
        copy = Condition.from_string(str(condition))
        self.assertEqual(condition, copy)

    def test_to_string_value(self):
        condition = Condition(value = 'value')
        copy = Condition.from_string(str(condition))
        self.assertEqual(condition, copy)

    def test_to_string_reverse(self):
        condition = Condition(reverse = True)
        copy = Condition.from_string(str(condition))
        self.assertEqual(condition, copy)

    def test_to_string_regex(self):
        condition = Condition(regex = True)
        copy = Condition.from_string(str(condition))
        self.assertEqual(condition, copy)

    def test_to_string_sub_elements(self):
        condition = Condition(sub_elements = [Permission(), Return()])
        copy = Condition.from_string(str(condition))
        self.assertEqual(condition, copy)

    def test_to_string_full(self):
        condition = Condition('name', 'value', True, True,
                              [Permission(), Return()])
        copy = Condition.from_string(str(condition))
        self.assertEqual(condition, copy)

    def test_to_lxml(self):
        condition = Condition()
        copy = Condition.from_lxml_element(condition.to_lxml_element())
        self.assertEqual(condition, copy)

    def test_to_lxml_name(self):
        condition = Condition(name = 'name')
        copy = Condition.from_lxml_element(condition.to_lxml_element())
        self.assertEqual(condition, copy)

    def test_to_lxml_value(self):
        condition = Condition(value = 'value')
        copy = Condition.from_lxml_element(condition.to_lxml_element())
        self.assertEqual(condition, copy)

    def test_to_lxml_reverse(self):
        condition = Condition(reverse = True)
        copy = Condition.from_lxml_element(condition.to_lxml_element())
        self.assertEqual(condition, copy)

    def test_to_lxml_regex(self):
        condition = Condition(regex = True)
        copy = Condition.from_lxml_element(condition.to_lxml_element())
        self.assertEqual(condition, copy)

    def test_to_lxml_sub_elements(self):
        condition = Condition(sub_elements = [Permission(), Return()])
        copy = Condition.from_lxml_element(condition.to_lxml_element())
        self.assertEqual(condition, copy)

    def test_to_lxml_full(self):
        condition = Condition('name', 'value', True, True,
                              [Permission(), Return()])
        copy = Condition.from_lxml_element(condition.to_lxml_element())
        self.assertEqual(condition, copy)

    def test_bad_root_tag(self):
        text = '<ERROR />'
        self.assertRaises(AttributeError, Condition.from_string, text)
        self.assertRaises(AttributeError, Condition.from_lxml_element,
                          etree.XML(text))

class PermissionTest(unittest.TestCase):
    def test_creation(self):
        permission = Permission()
        permission = Permission(True)
        permission = Permission(True, True)
        permission = Permission(True, True, True)
        permission = Permission(True, True, True, True)
        permission = Permission(True, True, True, True, True)

    def test_read(self):
        permission = Permission(True)
        self.assertTrue(permission.get_read())
        permission.set_read(False)
        self.assertFalse(permission.get_read())
        permission.set_read(None)
        self.assertEqual(None, permission.get_read())
        permission = Permission(read = True)
        self.assertTrue(permission.get_read())

    def test_execute(self):
        permission = Permission(True, True)
        self.assertTrue(permission.get_execute())
        permission.set_execute(False)
        self.assertFalse(permission.get_execute())
        permission.set_execute(None)
        self.assertEqual(None, permission.get_execute())
        permission = Permission(execute = True)
        self.assertTrue(permission.get_execute())

    def test_browse(self):
        permission = Permission(True, True, False)
        self.assertFalse(permission.get_browse())
        permission.set_browse(True)
        self.assertTrue(permission.get_browse())
        permission.set_browse(None)
        self.assertEqual(None, permission.get_browse())
        permission = Permission(browse = True)
        self.assertTrue(permission.get_browse())

    def test_delete(self):
        permission = Permission(True, True, False, False)
        self.assertFalse(permission.get_delete())
        permission.set_delete(True)
        self.assertTrue(permission.get_delete())
        permission.set_delete(None)
        self.assertEqual(None, permission.get_delete())
        permission = Permission(delete = True)
        self.assertTrue(permission.get_delete())

    def test_write(self):
        permission = Permission(True, True, False, False, False)
        self.assertFalse(permission.get_write())
        permission.set_write(True)
        self.assertTrue(permission.get_write())
        permission.set_write(None)
        self.assertEqual(None, permission.get_write())
        permission = Permission(write = True)
        self.assertTrue(permission.get_write())

    def test_equality(self):
        self.assertEqual(
            Permission(False, False, False, False, False),
            Permission(False, False, False, False, False))
        self.assertNotEqual(
            Permission(False, False, False, False, False),
            Permission(True, False, False, False, False))
        self.assertNotEqual(
            Permission(False, False, False, False, False),
            Permission(False, True, False, False, False))
        self.assertNotEqual(
            Permission(False, False, False, False, False),
            Permission(False, False, True, False, False))
        self.assertNotEqual(
            Permission(False, False, False, False, False),
            Permission(False, False, False, True, False))
        self.assertNotEqual(
            Permission(False, False, False, False, False),
            Permission(False, False, False, False, True))
        self.assertNotEqual(Permission(), 'another type')

    def test_from_string(self):
        text = '<PERMISSION />'
        permission = Permission.from_string(text)
        right = Permission()
        self.assertEqual(permission, right)

    def test_from_string_read(self):
        text = '<PERMISSION READ="NO" />'
        permission = Permission.from_string(text)
        right = Permission(read = False)
        self.assertEqual(permission, right)

    def test_from_string_execute(self):
        text = '<PERMISSION EXECUTE="NO" />'
        permission = Permission.from_string(text)
        right = Permission(execute = False)
        self.assertEqual(permission, right)

    def test_from_string_browse(self):
        text = '<PERMISSION BROWSE="NO" />'
        permission = Permission.from_string(text)
        right = Permission(browse = False)
        self.assertEqual(permission, right)

    def test_from_string_delete(self):
        text = '<PERMISSION DELETE="NO" />'
        permission = Permission.from_string(text)
        right = Permission(delete = False)
        self.assertEqual(permission, right)

    def test_from_string_write(self):
        text = '<PERMISSION WRITE="NO" />'
        permission = Permission.from_string(text)
        right = Permission(write = False)
        self.assertEqual(permission, right)

    def test_from_string_full(self):
        text = '''
<PERMISSION READ="NO" EXECUTE="YES" BROWSE="NO" DELETE="YES" WRITE="NO" />'''
        permission = Permission.from_string(text)
        right = Permission(False, True, False, True, False)
        self.assertEqual(permission, right)

    def test_from_lxml(self):
        text = '<PERMISSION />'
        permission = Permission.from_lxml_element(etree.XML(text))
        right = Permission()
        self.assertEqual(permission, right)

    def test_from_lxml_read(self):
        text = '<PERMISSION READ="NO" />'
        permission = Permission.from_lxml_element(etree.XML(text))
        right = Permission(read = False)
        self.assertEqual(permission, right)

    def test_from_lxml_execute(self):
        text = '<PERMISSION EXECUTE="NO" />'
        permission = Permission.from_lxml_element(etree.XML(text))
        right = Permission(execute = False)
        self.assertEqual(permission, right)

    def test_from_lxml_browse(self):
        text = '<PERMISSION BROWSE="NO" />'
        permission = Permission.from_lxml_element(etree.XML(text))
        right = Permission(browse = False)
        self.assertEqual(permission, right)

    def test_from_lxml_delete(self):
        text = '<PERMISSION DELETE="NO" />'
        permission = Permission.from_lxml_element(etree.XML(text))
        right = Permission(delete = False)
        self.assertEqual(permission, right)

    def test_from_lxml_write(self):
        text = '<PERMISSION WRITE="NO" />'
        permission = Permission.from_lxml_element(etree.XML(text))
        right = Permission(write = False)
        self.assertEqual(permission, right)

    def test_from_lxml_full(self):
        text = '''
<PERMISSION READ="NO" EXECUTE="YES" BROWSE="NO" DELETE="YES" WRITE="NO" />'''
        permission = Permission.from_lxml_element(etree.XML(text))
        right = Permission(False, True, False, True, False)
        self.assertEqual(permission, right)

    def test_to_string(self):
        permission = Permission()
        copy = Permission.from_string(str(permission))
        self.assertEqual(permission, copy)

    def test_to_string_read(self):
        permission = Permission(read = True)
        copy = Permission.from_string(str(permission))
        self.assertEqual(permission, copy)

    def test_to_string_execute(self):
        permission = Permission(execute = True)
        copy = Permission.from_string(str(permission))
        self.assertEqual(permission, copy)

    def test_to_string_browse(self):
        permission = Permission(browse = True)
        copy = Permission.from_string(str(permission))
        self.assertEqual(permission, copy)

    def test_to_string_delete(self):
        permission = Permission(delete = True)
        copy = Permission.from_string(str(permission))
        self.assertEqual(permission, copy)

    def test_to_string_write(self):
        permission = Permission(write = True)
        copy = Permission.from_string(str(permission))
        self.assertEqual(permission, copy)

    def test_to_string_full(self):
        permission = Permission(True, False, True, False, True)
        copy = Permission.from_string(str(permission))
        self.assertEqual(permission, copy)

    def test_to_lxml(self):
        permission = Permission()
        copy = Permission.from_lxml_element(permission.to_lxml_element())
        self.assertEqual(permission, copy)

    def test_to_lxml_read(self):
        permission = Permission(read = True)
        copy = Permission.from_lxml_element(permission.to_lxml_element())
        self.assertEqual(permission, copy)

    def test_to_lxml_execute(self):
        permission = Permission(execute = True)
        copy = Permission.from_lxml_element(permission.to_lxml_element())
        self.assertEqual(permission, copy)

    def test_to_lxml_browse(self):
        permission = Permission(browse = True)
        copy = Permission.from_lxml_element(permission.to_lxml_element())
        self.assertEqual(permission, copy)

    def test_to_lxml_delete(self):
        permission = Permission(delete = True)
        copy = Permission.from_lxml_element(permission.to_lxml_element())
        self.assertEqual(permission, copy)

    def test_to_lxml_write(self):
        permission = Permission(write = True)
        copy = Permission.from_lxml_element(permission.to_lxml_element())
        self.assertEqual(permission, copy)

    def test_to_lxml_full(self):
        permission = Permission(True, False, True, False, True)
        copy = Permission.from_lxml_element(permission.to_lxml_element())
        self.assertEqual(permission, copy)

    def test_bad_root_tag(self):
        text = '<ERROR />'
        self.assertRaises(AttributeError, Permission.from_string, text)
        self.assertRaises(AttributeError, Permission.from_lxml_element,
                          etree.XML(text))

class UserTest(unittest.TestCase):
    def test_creation(self):
        user = User()
        user = User('name')
        user = User('name', 'password')
        user = User('name', 'password', True)
        user = User('name', 'password', True, True)
        user = User('name', 'password', True, True, True)
        user = User('name', 'password', True, True, True, True)
        user = User('name', 'password', True, True, True, True, True)

    def test_name(self):
        user = User('name')
        self.assertEqual('name', user.get_name())
        user.set_name('other')
        self.assertEqual('other', user.get_name())
        user.set_name(None)
        self.assertEqual(None, user.get_name())
        user = User(name = 'name')
        self.assertEqual('name', user.get_name())

    def test_password(self):
        user = User('name', 'password')
        self.assertEqual('password', user.get_password())
        user.set_password('other')
        self.assertEqual('other', user.get_password())
        user.set_password(None)
        self.assertEqual(None, user.get_password())
        user = User(password = 'password')
        self.assertEqual('password', user.get_password())

    def test_read(self):
        user = User('name', 'password', True)
        self.assertTrue(user.get_read())
        user.set_read(False)
        self.assertFalse(user.get_read())
        user.set_read(None)
        self.assertEqual(None, user.get_read())
        user = User(read = True)
        self.assertTrue(user.get_read())

    def test_execute(self):
        user = User('name', 'password', True, True)
        self.assertTrue(user.get_execute())
        user.set_execute(False)
        self.assertFalse(user.get_execute())
        user.set_execute(None)
        self.assertEqual(None, user.get_execute())
        user = User(execute = True)
        self.assertTrue(user.get_execute())

    def test_browse(self):
        user = User('name', 'password', True, True, False)
        self.assertFalse(user.get_browse())
        user.set_browse(True)
        self.assertTrue(user.get_browse())
        user.set_browse(None)
        self.assertEqual(None, user.get_browse())
        user = User(browse = True)
        self.assertTrue(user.get_browse())

    def test_delete(self):
        user = User('name', 'password', True, True, False, False)
        self.assertFalse(user.get_delete())
        user.set_delete(True)
        self.assertTrue(user.get_delete())
        user.set_delete(None)
        self.assertEqual(None, user.get_delete())
        user = User(delete = True)
        self.assertTrue(user.get_delete())

    def test_write(self):
        user = User('name', 'password', True, True, False, False, False)
        self.assertFalse(user.get_write())
        user.set_write(True)
        self.assertTrue(user.get_write())
        user.set_write(None)
        self.assertEqual(None, user.get_write())
        user = User(write = True)
        self.assertTrue(user.get_write())

    def test_equality(self):
        self.assertEqual(
            User('name', 'password', False, False, False, False, False),
            User('name', 'password', False, False, False, False, False))
        self.assertNotEqual(
            User('name', 'password', False, False, False, False, False),
            User('other', 'password', False, False, False, False, False))
        self.assertNotEqual(
            User('name', 'password', False, False, False, False, False),
            User('name', 'other', False, False, False, False, False))
        self.assertNotEqual(
            User('name', 'password', False, False, False, False, False),
            User('name', 'password', True, False, False, False, False))
        self.assertNotEqual(
            User('name', 'password', False, False, False, False, False),
            User('name', 'password', False, True, False, False, False))
        self.assertNotEqual(
            User('name', 'password', False, False, False, False, False),
            User('name', 'password', False, False, True, False, False))
        self.assertNotEqual(
            User('name', 'password', False, False, False, False, False),
            User('name', 'password', False, False, False, True, False))
        self.assertNotEqual(
            User('name', 'password', False, False, False, False, False),
            User('name', 'password', False, False, False, False, True))
        self.assertNotEqual(User(), 'another type')

    def test_from_string(self):
        text = '<USER />'
        user = User.from_string(text)
        right = User()
        self.assertEqual(user, right)

    def test_from_string_name(self):
        text = '<USER name="name" />'
        user = User.from_string(text)
        right = User(name = 'name')
        self.assertEqual(user, right)

    def test_from_string_password(self):
        text = '<USER password="pass" />'
        user = User.from_string(text)
        right = User(password = 'pass')
        self.assertEqual(user, right)

    def test_from_string_read(self):
        text = '<USER READ="NO" />'
        user = User.from_string(text)
        right = User(read = False)
        self.assertEqual(user, right)

    def test_from_string_execute(self):
        text = '<USER EXECUTE="NO" />'
        user = User.from_string(text)
        right = User(execute = False)
        self.assertEqual(user, right)

    def test_from_string_browse(self):
        text = '<USER BROWSE="NO" />'
        user = User.from_string(text)
        right = User(browse = False)
        self.assertEqual(user, right)

    def test_from_string_delete(self):
        text = '<USER DELETE="NO" />'
        user = User.from_string(text)
        right = User(delete = False)
        self.assertEqual(user, right)

    def test_from_string_write(self):
        text = '<USER WRITE="NO" />'
        user = User.from_string(text)
        right = User(write = False)
        self.assertEqual(user, right)

    def test_from_string_full(self):
        text = '''
<USER name="name" password="pass" READ="NO" EXECUTE="YES" BROWSE="NO"
      DELETE="YES" WRITE="NO" />'''
        user = User.from_string(text)
        right = User('name', 'pass', False, True, False, True, False)
        self.assertEqual(user, right)

    def test_from_lxml(self):
        text = '<USER />'
        user = User.from_lxml_element(etree.XML(text))
        right = User()
        self.assertEqual(user, right)

    def test_from_lxml_name(self):
        text = '<USER name="name" />'
        user = User.from_lxml_element(etree.XML(text))
        right = User(name = 'name')
        self.assertEqual(user, right)

    def test_from_lxml_password(self):
        text = '<USER password="pass" />'
        user = User.from_lxml_element(etree.XML(text))
        right = User(password = 'pass')
        self.assertEqual(user, right)

    def test_from_lxml_read(self):
        text = '<USER READ="NO" />'
        user = User.from_lxml_element(etree.XML(text))
        right = User(read = False)
        self.assertEqual(user, right)

    def test_from_lxml_execute(self):
        text = '<USER EXECUTE="NO" />'
        user = User.from_lxml_element(etree.XML(text))
        right = User(execute = False)
        self.assertEqual(user, right)

    def test_from_lxml_browse(self):
        text = '<USER BROWSE="NO" />'
        user = User.from_lxml_element(etree.XML(text))
        right = User(browse = False)
        self.assertEqual(user, right)

    def test_from_lxml_delete(self):
        text = '<USER DELETE="NO" />'
        user = User.from_lxml_element(etree.XML(text))
        right = User(delete = False)
        self.assertEqual(user, right)

    def test_from_lxml_write(self):
        text = '<USER WRITE="NO" />'
        user = User.from_lxml_element(etree.XML(text))
        right = User(write = False)
        self.assertEqual(user, right)

    def test_from_lxml_full(self):
        text = '''
<USER name="name" password="pass" READ="NO" EXECUTE="YES" BROWSE="NO"
      DELETE="YES" WRITE="NO" />'''
        user = User.from_lxml_element(etree.XML(text))
        right = User('name', 'pass', False, True, False, True, False)
        self.assertEqual(user, right)

    def test_to_string(self):
        user = User()
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_string_name(self):
        user = User(name = 'name')
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_string_password(self):
        user = User(password = 'pass')
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_string_read(self):
        user = User(read = True)
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_string_execute(self):
        user = User(execute = True)
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_string_browse(self):
        user = User(browse = True)
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_string_delete(self):
        user = User(delete = True)
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_string_write(self):
        user = User(write = True)
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_string_full(self):
        user = User('name', 'pass', True, False, True, False, True)
        copy = User.from_string(str(user))
        self.assertEqual(user, copy)

    def test_to_lxml(self):
        user = User()
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_to_lxml_name(self):
        user = User(name = 'name')
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_to_lxml_password(self):
        user = User(password = 'pass')
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_to_lxml_read(self):
        user = User(read = True)
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_to_lxml_execute(self):
        user = User(execute = True)
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_to_lxml_browse(self):
        user = User(browse = True)
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_to_lxml_delete(self):
        user = User(delete = True)
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_to_lxml_write(self):
        user = User(write = True)
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_to_lxml_full(self):
        user = User('name', 'pass', True, False, True, False, True)
        copy = User.from_lxml_element(user.to_lxml_element())
        self.assertEqual(user, copy)

    def test_bad_root_tag(self):
        text = '<ERROR />'
        self.assertRaises(AttributeError, User.from_string, text)
        self.assertRaises(AttributeError, User.from_lxml_element,
                          etree.XML(text))

class ReturnTest(unittest.TestCase):
    def test_creation(self):
        ret = Return()
        ret = Return('ALLOW')

    def test_value(self):
        ret = Return('ALLOW')
        self.assertEqual('ALLOW', ret.get_value())
        ret.set_value('DENY')
        self.assertEqual('DENY', ret.get_value())
        ret.set_value(None)
        self.assertEqual(None, ret.get_value())
        ret = Return(value = 'ALLOW')
        self.assertEqual('ALLOW', ret.get_value())

    def test_equality(self):
        self.assertEqual(Return('ALLOW'), Return('ALLOW'))
        self.assertNotEqual(Return('ALLOW'), Return('DENY'))
        self.assertNotEqual(Return(), 'another type')

    def test_from_string(self):
        text = '<RETURN />'
        ret = Return.from_string(text)
        right = Return()
        self.assertEqual(ret, right)

    def test_from_string_value(self):
        text = '<RETURN value="ALLOW" />'
        ret = Return.from_string(text)
        right = Return(value = 'ALLOW')
        self.assertEqual(ret, right)

    def test_from_lxml(self):
        text = '<RETURN />'
        ret = Return.from_lxml_element(etree.XML(text))
        right = Return()
        self.assertEqual(ret, right)

    def test_from_lxml_value(self):
        text = '<RETURN value="ALLOW" />'
        ret = Return.from_lxml_element(etree.XML(text))
        right = Return(value = 'ALLOW')
        self.assertEqual(ret, right)

    def test_to_string(self):
        ret = Return()
        copy = Return.from_string(str(ret))
        self.assertEqual(ret, copy)

    def test_to_string_value(self):
        ret = Return(value = 'ALLOW')
        copy = Return.from_string(str(ret))
        self.assertEqual(ret, copy)

    def test_to_lxml(self):
        ret = Return()
        copy = Return.from_lxml_element(ret.to_lxml_element())
        self.assertEqual(ret, copy)

    def test_to_lxml_value(self):
        ret = Return(value = 'ALLOW')
        copy = Return.from_lxml_element(ret.to_lxml_element())
        self.assertEqual(ret, copy)

    def test_bad_root_tag(self):
        text = '<ERROR />'
        self.assertRaises(AttributeError, Return.from_string, text)
        self.assertRaises(AttributeError, Return.from_lxml_element,
                          etree.XML(text))

class SecurityListTest(unittest.TestCase):
    def test_creation(self):
        slist = SecurityList()
        slist = SecurityList([Permission(), Return()])

    def test_elements(self):
        element_0 = Condition()
        element_1 = Return()
        slist = SecurityList()
        self.assertEqual(0, len(slist.get_elements()))
        slist.add_element(element_0)
        self.assertEqual(1, len(slist.get_elements()))
        self.assertEqual(element_0, slist.get_element(0))
        slist.add_element(element_1)
        self.assertEqual(2, len(slist.get_elements()))
        self.assertEqual(element_0, slist.get_element(0))
        self.assertEqual(element_1, slist.get_element(1))
        slist.remove_element(0)
        self.assertEqual(1, len(slist.get_elements()))
        self.assertEqual(element_1, slist.get_element(0))
        slist.add_element(element_0, 0)
        self.assertEqual(2, len(slist.get_elements()))
        self.assertEqual(element_0, slist.get_element(0))
        self.assertEqual(element_1, slist.get_element(1))
        self.assertRaises(IndexError, slist.get_element, 2)
        slist = SecurityList(elements = [element_0, element_1])
        self.assertEqual(2, len(slist.get_elements()))
        self.assertEqual(element_0, slist.get_element(0))
        self.assertEqual(element_1, slist.get_element(1))

    def test_equality(self):
        self.assertEqual(SecurityList([Condition()]),
                         SecurityList([Condition()]))
        self.assertNotEqual(SecurityList([Condition()]),
                            SecurityList([]))
        self.assertNotEqual(SecurityList(), 'another type')

    def test_from_string(self):
        text = '<SECURITY />'
        slist = SecurityList.from_string(text)
        right = SecurityList()
        self.assertEqual(slist, right)

    def test_from_string_elements(self):
        text = '<SECURITY><CONDITION /><RETURN /></SECURITY>'
        slist = SecurityList.from_string(text)
        right = SecurityList([Condition(), Return()])
        self.assertEqual(slist, right)

    def test_from_lxml(self):
        text = '<SECURITY />'
        slist = SecurityList.from_lxml_element(etree.XML(text))
        right = SecurityList()
        self.assertEqual(slist, right)

    def test_from_lxml_elements(self):
        text = '<SECURITY><CONDITION /><RETURN /></SECURITY>'
        slist = SecurityList.from_lxml_element(etree.XML(text))
        right = SecurityList([Condition(), Return()])
        self.assertEqual(slist, right)

    def test_to_string(self):
        slist = SecurityList()
        copy = SecurityList.from_string(str(slist))
        self.assertEqual(slist, copy)

    def test_to_string_elements(self):
        slist = SecurityList([Condition(), Return()])
        copy = SecurityList.from_string(str(slist))
        self.assertEqual(slist, copy)

    def test_to_lxml(self):
        slist = SecurityList()
        copy = SecurityList.from_lxml_element(slist.to_lxml_element())
        self.assertEqual(slist, copy)

    def test_to_string_elements(self):
        slist = SecurityList([Condition(), Return()])
        copy = SecurityList.from_lxml_element(slist.to_lxml_element())
        self.assertEqual(slist, copy)

    def test_bad_root_tag(self):
        text = '<ERROR />'
        self.assertRaises(AttributeError, SecurityList.from_string, text)
        self.assertRaises(AttributeError, SecurityList.from_lxml_element,
                          etree.XML(text))

class BadMarkupTest(unittest.TestCase):
    def condition_test(self):
        text = '''
<CONDITION custom="unknown">
  <PERMISSION />
  <UNKNOWN>
    <CUSTOM />
  </UNKNOWN>
</CONDITION>'''
        condition = Condition.from_string(text)
        tree = condition.to_lxml_element()
        self.assertEqual('unknown', tree.get('custom'))
        unknown = tree.findall('UNKNOWN')
        self.assertEqual(1, len(unknown))
        unknown = unknown[0]
        custom = list(unknown)
        self.assertEqual(1, len(custom))
        custom = custom[0]
        self.assertEqual('CUSTOM', custom.tag)

    def permission_test(self):
        text = '''
<PERMISSION custom="unknown">
  <UNKNOWN>
    <CUSTOM />
  </UNKNOWN>
</CONDITION>'''
        permission = Permission.from_string(text)
        tree = permission.to_lxml_element()
        self.assertEqual('unknown', tree.get('custom'))
        unknown = tree.findall('UNKNOWN')
        self.assertEqual(1, len(unknown))
        unknown = unknown[0]
        custom = list(unknown)
        self.assertEqual(1, len(custom))
        custom = custom[0]
        self.assertEqual('CUSTOM', custom.tag)

    def user_test(self):
        text = '''
<USER custom="unknown">
  <UNKNOWN>
    <CUSTOM />
  </UNKNOWN>
</USER>'''
        user = User.from_string(text)
        tree = user.to_lxml_element()
        self.assertEqual('unknown', tree.get('custom'))
        unknown = tree.findall('UNKNOWN')
        self.assertEqual(1, len(unknown))
        unknown = unknown[0]
        custom = list(unknown)
        self.assertEqual(1, len(custom))
        custom = custom[0]
        self.assertEqual('CUSTOM', custom.tag)

    def return_test(self):
        text = '''
<RETURN custom="unknown">
  <UNKNOWN>
    <CUSTOM />
  </UNKNOWN>
</RETURN>'''
        ret = Return.from_string(text)
        tree = ret.to_lxml_element()
        self.assertEqual('unknown', tree.get('custom'))
        unknown = tree.findall('UNKNOWN')
        self.assertEqual(1, len(unknown))
        unknown = unknown[0]
        custom = list(unknown)
        self.assertEqual(1, len(custom))
        custom = custom[0]
        self.assertEqual('CUSTOM', custom.tag)

    def security_list_test(self):
        text = '''
<SECURITY custom="unknown">
  <CONDITION />
  <RETURN />
  <UNKNOWN>
    <CUSTOM />
  </UNKNOWN>
</SECURITY>'''
        slist = SecurityList.from_string(text)
        tree = slist.to_lxml_element()
        self.assertEqual('unknown', tree.get('custom'))
        unknown = tree.findall('UNKNOWN')
        self.assertEqual(1, len(unknown))
        unknown = unknown[0]
        custom = list(unknown)
        self.assertEqual(1, len(custom))
        custom = custom[0]
        self.assertEqual('CUSTOM', custom.tag)

if __name__ == '__main__':
    unittest.main()
