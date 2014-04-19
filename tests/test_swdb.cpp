#include <gtest/gtest.h>
#include "../src/cpp/swdb.hpp"
#include "../src/cpp/record.hpp"
 
TEST(SwdbTest, StoreRecords) {
    Swdb swdb("test.db");
    uRecord transaction = swdb.record("transaction");
    uRecord pkg1 = swdb.record("pkg");
    pkg1->set("name", "lib-1-x.noarch");
    uRecord pkg2 = swdb.record("pkg");
    pkg2->set("name", "app-3-y.x86_64");
    cout << "### here\n";
    // save packages
    // query for packages
}
