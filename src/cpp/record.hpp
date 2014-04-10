#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "table.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;


class Record {
private:
    bool record_id;
    bool changed;
    unordered_map<string,string> values_from_db;
    unordered_map<string,string> values_to_insert;
    Table* from_table;
public:
    bool is_in_db();
    bool is_changed();
    void set(string key, string value);
    void get(string key);
    string to_update_sql();
};
