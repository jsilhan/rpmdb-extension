#!/usr/bin/env python

import unittest
import swdb

class SubjectRealPossibilitiesTest(unittest.TestCase):
    def setUp(self):
        pass

    def test_basic(self):
        s = swdb.Swdb("location.db", 3)
        rec = s.record("pkgs")
        self.assertTrue(rec.set("type", 3))
