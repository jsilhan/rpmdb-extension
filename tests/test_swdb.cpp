#include <stdio.h>
#include <gtest/gtest.h>
#include "../src/cpp/swdb.hpp"
#include "../src/cpp/record.hpp"
 
class SwdbTest : public ::testing::Test {
public:
    virtual void TearDown() {
        remove("test.db");
    }
};

TEST_F(SwdbTest, StoreRecords) {
    Swdb swdb("test.db");
    EXPECT_TRUE(swdb.init());
    uRecord transaction = swdb.record("transaction");
    uRecord pkg1 = swdb.record("pkgs");
    pkg1->set("name", "lib-1-x.noarch");
    // uRecord pkg2 = swdb.record("pkg");
    // pkg2->set("name", "app-3-y.x86_64");
    EXPECT_TRUE(pkg1->save());

    uQuery q = swdb.query("pkgs");
    q->filter("name", "lib-1-x.noarch", EQ);

    // save packages
    // query for packages
}
