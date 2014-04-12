#include <gtest/gtest.h>
#include "../src/cpp/query.hpp"
#include "../src/cpp/swdb.hpp"
#include "../src/cpp/table.hpp"
 
const string SELECT_SCRIPT = "SELECT t1.t1f1 t1.t1f2 t1.t1f3 FROM t1"
    "JOIN t2 ON t1.id t2._t1"
    "WHERE t2.val = 'val';";

TEST(QueryTest, SelectSqlScript) {
    uTable t(new Table("t1"));
    t->add_field("t1f1", STRING);
    t->add_field("t1f2", INT);
    t->add_field("t1f3", STRING);
    uTable t2(new Table("t2"));
    t2->add_field("t2f1", STRING);
    t2->add_field("t2f2", INT);
    Swdb db;
    db.tables[t->name] = move(t);
    db.tables[t2->name] = move(t2);
    db.connections.connect_tables(*t, *t2, ONE_TO_MANY);
    Query q(db, *t);
    q.filter("t2.t2f1", "val", 0);
    ASSERT_EQ(q.to_select_sql(), SELECT_SCRIPT);
}