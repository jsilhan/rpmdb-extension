#include <gtest/gtest.h>
#include "../src/cpp/table.hpp"
#include "../src/cpp/utils.hpp"
 
const string INIT_SCRIPT = "CREATE TABLE IF NOT EXISTS t ("
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

TEST(TableTest, Cell) {
    Table t("t");
    t.add_field("f2", INT);
    t.add_field("f3", STRING);
    stringstream sql;
    unsigned long i;
    EXPECT_TRUE(t.get_cell_index("f2", i));
    EXPECT_EQ(1, i);
    EXPECT_TRUE(t.get_cell_index("f3", i));
    EXPECT_EQ(2, i);
    EXPECT_FALSE(t.get_cell_index("bla", i));
}

// test expandable fields
