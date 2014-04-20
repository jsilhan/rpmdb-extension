#include <gtest/gtest.h>
#include "../src/cpp/record.hpp"
#include "../src/cpp/table.hpp"
#include "../src/cpp/db.hpp"
 
const string INSERT_SCRIPT = "INSERT INTO t1 (t1f3, t1f2)"
    " VALUES ('bla', '3');";

const string UPDATE_SCRIPT = "UPDATE t1 SET t1f3='bla', t1f2='3' "
    "WHERE id='-1';";

TEST(RecordTest, InsertSqlScript) {
    string t1_name = "t1";
    uTable t(new Table(t1_name));
    t->add_field("t1f1", STRING);
    t->add_field("t1f2", INT);
    t->add_field("t1f3", STRING);
    Db db("test.db");
    db.tables[t->name] = move(t);
    Record r(db, t1_name);
    r.set("t1f2", 3);
    r.set("t1f3", "bla");
    stringstream insert_sql;
    EXPECT_TRUE(r.self_to_insert_sql(insert_sql));
    EXPECT_EQ(INSERT_SCRIPT, insert_sql.str());
    stringstream update_sql;
    EXPECT_TRUE(r.to_update_sql(update_sql));
    EXPECT_EQ(UPDATE_SCRIPT, update_sql.str());
}

// test required fields
