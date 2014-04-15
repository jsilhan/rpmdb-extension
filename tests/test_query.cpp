#include <gtest/gtest.h>
#include "../src/cpp/query.hpp"
#include "../src/cpp/db.hpp"
#include "../src/cpp/table.hpp"
 
const string SELECT_SCRIPT = "SELECT t1.t1f1 t1.t1f2 t1.t1f3 FROM t1 "
    "JOIN t2 ON t1._id = t2.t1 "
    "WHERE t2.t2f1 = 'val';";

TEST(QueryTest, SelectSqlScript) {
    string t1_name = "t1";
    string t2_name = "t2";
    sTable t(new Table(t1_name));
    t->add_field("t1f1", STRING);
    t->add_field("t1f2", INT);
    t->add_field("t1f3", STRING);
    sTable t2(new Table(t2_name));
    t2->add_field("t2f1", STRING);
    t2->add_field("t2f2", INT);
    Db db("test.db");
    db.tables[t->name] = t;
    db.tables[t2->name] = t2;
    EXPECT_EQ(db.tables.size(), 2);
    EXPECT_TRUE(db.tables.find("t2") != db.tables.end());
    db.add_many_to_one(t, t2);
    Query q(db, t1_name);
    q.filter("t2.t2f1", "val", EQ);
    EXPECT_EQ(q.to_select_sql(), SELECT_SCRIPT);
}

TEST(QueryTest, SelectAliasSqlScript) {
    string t1_name = "t1";
    string t2_name = "t2";
    sTable t(new Table(t1_name));
    t->add_field("t1f1", STRING);
    t->add_field("t1f2", INT);
    t->add_field("t1f3", STRING);
    sTable t2(new Table(t2_name));
    t2->add_field("t2f1", STRING);
    t2->add_field("t2f2", INT);
    Db db("test.db");
    db.tables[t->name] = t;
    db.tables[t2->name] = t2;
    EXPECT_EQ(db.tables.size(), 2);
    EXPECT_TRUE(db.tables.find("t2") != db.tables.end());
    db.add_many_to_one(t, t2, t1_name, string("table2"));
    Query q(db, t1_name);
    q.filter("table2.t2f1", "val", EQ);
    EXPECT_EQ(q.to_select_sql(), SELECT_SCRIPT);
}
