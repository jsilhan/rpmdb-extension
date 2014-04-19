#include <gtest/gtest.h>
#include "../src/cpp/query.hpp"
#include "../src/cpp/db.hpp"
#include "../src/cpp/table.hpp"
 
const string SELECT_SCRIPT = "SELECT t1.t1f1 t1.t1f2 t1.t1f3 t1.t2 FROM t1 "
    "JOIN t2 ON t1.t2 = t2._id "
    "WHERE t2.t2f1 = 'val';";

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

TEST_F(QueryTest, SelectAliasSqlScript) {
    db.add_many_to_one(t1, t2, string("table2"), t1_name);
    EXPECT_EQ(1, t1->neightbor_tables.size());
    EXPECT_EQ(1, t2->neightbor_tables.size());
    Query q(db, t1_name);
    q.filter("table2.t2f1", "val", EQ);
    stringstream sql;
    cout << "here" << endl;
    EXPECT_TRUE(q.to_select_sql(sql));
    EXPECT_EQ(SELECT_SCRIPT, sql.str());
}
