#include <gtest/gtest.h>
#include "../src/cpp/query.hpp"
#include "../src/cpp/db.hpp"
#include "../src/cpp/table.hpp"
 
const string SELECT_SCRIPT = "SELECT t1.t1f1 t1.t1f2 t1.t1f3 t1.t2 FROM t1 "
    "JOIN t2 ON t1.t2 = t2._id "
    "WHERE t2.t2f1 = 'val';";
 
const string SELECT_WITHIN_TABLE_SCRIPT = "SELECT t2.t2f1 t2.t2f2 FROM t2 "
    "WHERE t2.t2f1 = 'val';";
 
const string SELECT_3_SCRIPT = "SELECT t3.t2 t3.t3f1 FROM t3 "
    "JOIN t2 ON t3.t2 = t2._id "
    "JOIN t1 ON t2._id = t1.t2 "
    "WHERE t1.t1f1 = 'val';";

class QueryTest : public ::testing::Test {
public:
    sTable t1;
    sTable t2;
    string t1_name;
    string t2_name;
    Db db;

    virtual void SetUp() {
        t1_name = "t1";
        t2_name = "t2";
        t1 = make_shared<Table>(t1_name);
        t1->add_field("t1f1", STRING);
        t1->add_field("t1f2", INT);
        t1->add_field("t1f3", STRING);
        t2 = make_shared<Table>(t2_name);
        t2->add_field("t2f1", STRING);
        t2->add_field("t2f2", INT);
        db.tables[t1->name] = t1;
        db.tables[t2->name] = t2;
    }

    virtual void TearDown() {}
};

TEST_F(QueryTest, SelectSqlScript) {
    EXPECT_EQ(2, db.tables.size());
    EXPECT_TRUE(db.tables.find("t2") != db.tables.end());
    db.add_many_to_one(t1, t2);
    Query q(db, t1_name);
    q.filter("t2.t2f1", "val", EQ);
    stringstream sql;
    EXPECT_TRUE(q.to_select_sql(sql));
    EXPECT_EQ(SELECT_SCRIPT, sql.str());
}

TEST_F(QueryTest, SelectSqlWithinTableScript) {
    Query q(db, t2_name);
    q.filter("t2f1", "val", EQ);
    stringstream sql;
    EXPECT_TRUE(q.to_select_sql(sql));
    EXPECT_EQ(SELECT_WITHIN_TABLE_SCRIPT, sql.str());
}

TEST_F(QueryTest, SelectAliasSqlScript) {
    db.add_many_to_one(t1, t2, "table2", t1_name);
    EXPECT_EQ(1, t1->neightbor_tables.size());
    EXPECT_EQ(1, t2->neightbor_tables.size());
    Query q(db, t1_name);
    q.filter("table2.t2f1", "val", EQ);
    stringstream sql;
    EXPECT_TRUE(q.to_select_sql(sql));
    EXPECT_EQ(SELECT_SCRIPT, sql.str());
}

TEST_F(QueryTest, Select3SqlScript) {
    string t3_name = "t3";
    sTable t3(new Table(t3_name));
    EXPECT_EQ(1, t3.use_count());
    db.tables[t3->name] = t3;
    EXPECT_EQ(2, t3.use_count());
    t3->add_field("t3f1", STRING);
    db.add_many_to_one(t1, t2);
    db.add_many_to_one(t3, t2);
    EXPECT_EQ(1, t1->neightbor_tables.size());
    EXPECT_EQ(2, t2->neightbor_tables.size());
    EXPECT_EQ(1, t3->neightbor_tables.size());
    EXPECT_EQ(4, t1->fields_from_db.size());
    EXPECT_EQ(2, t2->fields_from_db.size());
    EXPECT_EQ(2, t3->fields_from_db.size());
    EXPECT_EQ(db.tables.count("t3"), 1);
    EXPECT_EQ(db.tables.count(t2_name), 1);
    EXPECT_EQ(db.tables.count(t1_name), 1);
    Query q(db, t3_name);
    EXPECT_EQ(3, t3.use_count());
    q.filter("t2.t1.t1f1", "val", EQ);
    stringstream sql;
    EXPECT_TRUE(q.to_select_sql(sql));
    EXPECT_EQ(SELECT_3_SCRIPT, sql.str());
}
