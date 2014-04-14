#include <gtest/gtest.h>
#include "../src/cpp/record.hpp"
#include "../src/cpp/table.hpp"
#include "../src/cpp/db.hpp"
 
const string INSERT_SCRIPT = "INSERT INTO t1 (t1f3, t1f2)"
    " VALUES ('bla', '3');";

TEST(RecordTest, InsertSqlScript) {
    string t1_name = "t1";
    uTable t(new Table(t1_name));
    t->add_field("t1f1", STRING);
    t->add_field("t1f2", INT);
    t->add_field("t1f3", STRING);
    Db db;
    db.tables[t->name] = move(t);
    Record r(db, t1_name);
    r.set("t1f2", 3);
    r.set("t1f3", "bla");
    stringstream sql;
    EXPECT_TRUE(r.self_to_insert_sql(sql));
    EXPECT_EQ(sql.str(), INSERT_SCRIPT);
}

// test required fields
