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
        self.assertTrue(rec.set("name", "n:e-v-r.a"))
        self.assertTrue(rec.save())

        q = s.query("pkgs")
        q.filter("name", "n:e-v-r.a", swdb.flags.EQ);
        self.assertEqual(3, q[0]["type"])
