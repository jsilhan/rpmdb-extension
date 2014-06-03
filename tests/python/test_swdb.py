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
        rec["name"] = "n:e-v-r.a";
        self.assertTrue(rec.save())

        q = s.query("pkgs")
        q.filter("name", "n:e-v-r.a", swdb.flags.EQ);
        first = q[0]
        self.assertTrue(isinstance(first, swdb.Record))
        self.assertRaises(AttributeError, first.__getitem__, "bla")
        self.assertEqual(3, first["type"])
        self.assertEqual("n:e-v-r.a", first["name"])
