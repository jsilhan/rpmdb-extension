#!/usr/bin/env python

import unittest
import swdb

class SubjectRealPossibilitiesTest(unittest.TestCase):
    def setUp(self):
        pass

    def test_basic(self):
        m1 = swdb.SomeClass("Pavel")
        print "name =", m1.name

        m1.number = 7.3
        print "number =", m1.number
