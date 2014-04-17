#include <gmock/gmock.h>

#include <string>
#include <gmock/gmock.h>
#include "../src/cpp/table.hpp"
#include "../src/cpp/db.hpp"

using std::string;

// class MockDb : public Db {
//     MOCK_CONST_METHOD0(init, bool());
//     MOCK_CONST_METHOD0(init_tables, bool());
//     MOCK_METHOD5(add_many_to_one, void(sTable&, sTable&,
//         string, string, bool));
//     MOCK_METHOD4(add_many_to_one, void(sTable&, sTable&,
//         string, bool));
//     MOCK_METHOD3(add_many_to_one, void(sTable&, sTable&, bool));
//     MOCK_METHOD2(execute, bool(string, string));
//     MOCK_METHOD3(add_comlumn, bool(Table& t, const string& column, field_flags flags));
// };


class Foo {
  virtual ~Foo();
  virtual int GetSize() const { return 3; }
  virtual string Describe(const char* name) { return name; }
};

class MockFoo : public Foo {
  MOCK_CONST_METHOD0(GetSize, int());
  MOCK_METHOD1(Describe, string(const char* name));
};
