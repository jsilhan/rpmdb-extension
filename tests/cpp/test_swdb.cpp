#include <stdio.h>
#include <gtest/gtest.h>
#include "../../src/cpp/swdb.hpp"
#include "../../src/cpp/record.hpp"
 
class SwdbTest : public ::testing::Test {
public:
    virtual void SetUp() {
        remove("test.db");
    }
};

TEST_F(SwdbTest, StoreRecords) {
    Swdb swdb("test.db");
    EXPECT_TRUE(swdb.init());
    uRecord pkg1 = swdb.urecord("pkgs");
    pkg1->set("name", "xxx.noarch");
    EXPECT_TRUE(pkg1->save());

    uQuery q = swdb.uquery("pkgs");
    q->filter("name", "xxx.noarch", EQ);

    auto it = q->begin();
    Record record = *it;
    string name;
    EXPECT_TRUE(record.get("name", name));
    EXPECT_EQ(1, record.id());
    EXPECT_EQ("xxx.noarch", name);
    ++it;
    EXPECT_TRUE(it == q->end());
}
