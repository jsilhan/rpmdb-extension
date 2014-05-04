#include <stdio.h>
#include <gtest/gtest.h>
#include "../src/cpp/swdb.hpp"
#include "../src/cpp/record.hpp"
 
class SwdbTest : public ::testing::Test {
public:
    virtual void SetUp() {
        remove("test.db");
    }

    // virtual void TearDown() {
    //     remove("test.db");
    // }
};

// TEST(RealSqlTest, QueryInit) {
//     Swdb swdb("test.db");
//     EXPECT_TRUE(swdb.init());
//     uRecord pkg1 = swdb.record("pkgs");
//     pkg1->set("name", "xxx.noarch");
//     EXPECT_TRUE(pkg1->save());

//     uQuery q = swdb.query("pkgs");
//     q->filter("name", "lib-1-x.noarch", EQ);

//     q->iter([&](Record& r) {
//         string name;
//         // cout << "### values = " << r.values_from_db.size() << endl;
//         // r.get("name", name); // FIXME
//         // cout << "### record_id = " << r.id() << "name = " << name << endl;
//     });
// }

TEST_F(SwdbTest, StoreRecords) {
    Swdb swdb("test.db");
    EXPECT_TRUE(swdb.init());
    uRecord pkg1 = swdb.record("pkgs");
    pkg1->set("name", "xxx.noarch");
    EXPECT_TRUE(pkg1->save());

    uQuery q = swdb.query("pkgs");
    q->filter("name", "xxx.noarch", EQ);

    // string name;
    auto it = q->begin();
    Record record = *it;
    string name;
    EXPECT_TRUE(record.get("name", name));
    EXPECT_EQ(1, record.id());
    EXPECT_EQ("xxx.noarch", name);
    ++it;
    EXPECT_TRUE(it == q->end());
    // cout << "### 1.record name = " << r.values_from_db[1]->text << endl;
    // cout << "### 1.record = " << r.id() << endl;
    // cout << "### after inc it" << endl;
    // r.get("name", name);
    // cout << "### name = " << name << endl;

    // for (Record record : *q) {
        
        // cout << "### id = " << record.id();
        // cout << " name = " << name << endl;
    // }

    // save packages
    // query for packages
}

// TEST(SliteAlone, SwdbTest) {
//     sqlite3* db;
//     sqlite3_stmt* statement;
//     sqlite3_open("test.db", &db);
//     string selectQuery = "SELECT pkgs.name, pkgs.type FROM pkgs WHERE pkgs.name = 'xxx.noarch';";
//     // const char * selectQuery = "SELECT pkgs.name, pkgs.type FROM pkgs WHERE pkgs.name = 'lib-1-x.noarch';";
//     int rc = sqlite3_prepare(db, selectQuery.c_str(), -1, &statement, NULL);
//     cout << "### test alone" << endl;
//     // while (1) {
//         int s;
//         s = sqlite3_step (statement);
//         if (s == SQLITE_ROW) {
//             const unsigned char * text;
//             text  = sqlite3_column_text (statement, 0);

//             cout << "### pkg_name = " << text << endl;
//             string s;
//             s = reinterpret_cast<const char*>(text);
//             sqlite3_finalize(statement);
//             cout << "### pkg_name = " << s << endl;
//         }
//         // else if (s == SQLITE_DONE) {
//         //     break;
//         // }
//         // else {
//         //     fprintf (stderr, "Failed.\n");
//         //     exit (1);
//         // }
//     // }
// }
