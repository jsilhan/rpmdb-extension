#include <gtest/gtest.h>
#include "../src/cpp/table.hpp"
 
const string INIT_SCRIPT = "CREATE TABLE t ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
    "f1 STRING, f2 INT, f3 STRING);";

TEST(TableTest, InitSqlScript) {
    Table t("t");
    t.add_field("f1", STRING);
    t.add_field("f2", INT);
    t.add_field("f3", STRING);
    ASSERT_EQ(t.to_init_sql(), INIT_SCRIPT);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
