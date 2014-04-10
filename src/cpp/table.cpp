#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include <memory>
#include <unordered_map>
#include <map>
#include <utility>
#include "table.hpp"
#include "utils.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;
using std::unordered_map;
using std::map;
using std::get;

bool Table::field_valid(const string& name, field_type type) {
    // contains non alpha characters
    // starts with '_'
    // key in internally used names (e.g. 'id')
    // key exist in table with different type
    return true;
}

void Table::add_field(string name, field_type type) {
    fields_from_db[name] = type;
}

Table::Table(string& name) : name(name) {};
Table::Table(const char* name) : name(name) {};

string Table::to_init_sql() {
    stringstream sql;
    sql << "CREATE TABLE " << name << " (";
    sql << "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,";

    vector<ustringstream> vect_stream;
    for (auto kv : fields_from_db) {
        ustringstream tmp_stream(new stringstream);
        *tmp_stream << kv.first << " ";
        if (kv.second == INT)
            *tmp_stream << "INT";
        else
            *tmp_stream << "STRING";
        vect_stream.push_back(move(tmp_stream));
    }
    append_joined(sql, vect_stream, ", ");
    sql << ");";
    return sql.str();
}
