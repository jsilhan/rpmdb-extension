#include <gtest/gtest.h>
#include "../../src/cpp/record.hpp"
#include "../../src/cpp/table.hpp"
#include "../../src/cpp/db.hpp"

const string INSERT_SCRIPT = "INSERT INTO t1 (t1f3, t1f2)"
    " VALUES ('bla', '3');";

const string UPDATE_SCRIPT = "UPDATE t1 SET t1f3='bla', t1f2='3' "
    "WHERE id='-1';";

class RecordTest : public ::testing::Test {
};

TEST_F(RecordTest, TestRecordGet) {
    string t1_name = "pkg";
    uTable t(new Table(t1_name));
    t->add_field("name", STRING);
    t->add_field("type", INT);
    sDb db(new Db("unused.db"));
    db->tables[t->name] = move(t);
    EXPECT_THROW(Record(db, "unknown"), std::out_of_range);
    Record r1(db, t1_name);
    r1.values_from_db.push_back(cell(INT, 1));
    r1.values_from_db.push_back(cell(STRING, "pkg_name_xxx"));
    r1.values_from_db.push_back(cell(INT, 3));
    string s;
    int i;
    EXPECT_TRUE(r1.get("name", s));
    EXPECT_EQ("pkg_name_xxx", s);
    EXPECT_EQ(1, r1.id());
    EXPECT_TRUE(r1.get("type", i));
    EXPECT_EQ(3, i);
}

TEST_F(RecordTest, InsertSqlScript) {
    string t1_name = "t1";
    uTable t(new Table(t1_name));
    t->add_field("t1f1", STRING);
    t->add_field("t1f2", INT);
    t->add_field("t1f3", STRING);
    sDb db(new Db("unused.db"));
    db->tables[t->name] = move(t);
    Record r(db, t1_name);
    EXPECT_TRUE(r.set("t1f2", 3));
    EXPECT_TRUE(r.set("t1f3", "bla"));

    // no new column will be added
    EXPECT_EQ(0, r.fields_to_append.size());

    stringstream insert_sql;
    EXPECT_TRUE(r.to_insert_sql(insert_sql));
    EXPECT_EQ(INSERT_SCRIPT, insert_sql.str());
    stringstream update_sql;
    EXPECT_TRUE(r.to_update_sql(update_sql));
    EXPECT_EQ(UPDATE_SCRIPT, update_sql.str());

    // table not extensible
    EXPECT_FALSE(r.set("new", 333));
    uTable t_extensible(new Table("extensible", false, true));
    db->tables["extensible"] = move(t_extensible);
    Record re(db, "extensible");
    EXPECT_TRUE(re.set("new", 333));
    EXPECT_EQ(1, re.fields_to_append.size());
    EXPECT_EQ("new", re.fields_to_append.begin()->first);
}

TEST_F(RecordTest, NewColumns) {

}

// test required fields
