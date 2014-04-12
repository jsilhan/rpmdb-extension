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

    for (auto i = fields_from_db.begin(); i != fields_from_db.end(); i++) {
        if (i != fields_from_db.begin())
            sql << ", ";

        sql << i->first << " ";
        if (i->second == INT)
            sql << "INT";
        else
            sql << "STRING";
    }

    sql << ");";
    return sql.str();
}
