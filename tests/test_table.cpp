#include <gtest/gtest.h>
#include "../src/cpp/table.hpp"
#include "../src/cpp/utils.hpp"
 
const string INIT_SCRIPT = "CREATE TABLE t ("
    "_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
    "f1 STRING, f2 INT, f3 STRING);";

TEST(TableTest, InitSqlScript) {
    Table t("t");
    t.add_field("f1", STRING);
    t.add_field("f2", INT);
    t.add_field("f3", STRING);
    stringstream sql;
    EXPECT_TRUE(t.to_init_sql(sql));
    EXPECT_EQ(sql.str(), INIT_SCRIPT);
}

// test expandable fields
