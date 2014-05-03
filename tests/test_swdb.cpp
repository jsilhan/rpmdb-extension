#include <stdio.h>
#include <gtest/gtest.h>
#include "../src/cpp/swdb.hpp"
#include "../src/cpp/record.hpp"
 
class SwdbTest : public ::testing::Test {
public:
    // virtual void SetUp() {
    //     remove("test.db");
    // }

    // virtual void TearDown() {
    //     remove("test.db");
    // }
};

TEST_F(SwdbTest, StoreRecords) {
    Swdb swdb("test.db");
    EXPECT_TRUE(swdb.init());
    // uRecord transaction = swdb.record("transaction");
    // uRecord pkg1 = swdb.record("pkgs");
    // pkg1->set("name", "lib-1-x.noarch");
    // EXPECT_TRUE(pkg1->save());

    uQuery q = swdb.query("pkgs");
    q->filter("name", "lib-1-x.noarch", EQ);

    q->iter([&](Record& r) {
        string name;
        cout << "### values = " << r.values_from_db.size() << endl;
        r.get("name", name);
        cout << "### record_id = " << r.id() << "name = " << name << endl;
    });

    // string name;
    // cout << "### before it" << endl;
    // auto it = q->begin();
    // cout << "### after it" << endl;
    // ++it;
    // cout << "### after inc it" << endl;
    // uRecord r = *it;
    // r->get("name", name);
    // cout << "### name = " << name << endl;

    // for (uRecord record : *q) {
    //     record->get("name", name);
    //     cout << "### name = " << name << endl;
    // }

    // save packages
    // query for packages
}

TEST(SliteAlone, SwdbTest) {
    // sqlite3* db;
    // sqlite3_stmt* statement;
    // sqlite3_open("test.db", &db);
    // string selectQuery = "SELECT pkgs.name, pkgs.type FROM pkgs WHERE pkgs.name = 'lib-1-x.noarch';";
    // // const char * selectQuery = "SELECT pkgs.name, pkgs.type FROM pkgs WHERE pkgs.name = 'lib-1-x.noarch';";
    // int rc = sqlite3_prepare(db, selectQuery.c_str(), -1, &statement, NULL);
    // cout << "SliteAlone rc = " << rc << endl;

    // while (1) {
    //     int s;
    //     printf("in select while\n");
    //     s = sqlite3_step (statement);
    //     if (s == SQLITE_ROW) {
    //         int bytes;
    //         const unsigned char * text;
    //         bytes = sqlite3_column_bytes(statement, 0);
    //         text  = sqlite3_column_text (statement, 0);
    //         printf ("%s\n", text);
    //     }
    //     else if (s == SQLITE_DONE) {
    //         break;
    //     }
    //     else {
    //         fprintf (stderr, "Failed.\n");
    //         exit (1);
    //     }
    // }
}
