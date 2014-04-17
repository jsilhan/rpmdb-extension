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

bool Table::field_valid(const string& name, field_flags type) {
    // contains non alpha characters
    // starts with '_'
    // key in internally used names (e.g. 'id')
    // key exist in table with different type
    return true;
}

void Table::add_field(string table_name, field_flags flags, bool required) {
    fields_from_db[table_name] = flags;
}

Table::Table(string& name, bool protect, bool extensible) :
    name(name), protect(protect), extensible(extensible) {}

Table::Table(const char* name, bool protect, bool extensible) :
    name(name), protect(protect), extensible(extensible) {}

bool Table::to_init_sql(stringstream& sql) {
    sql << "CREATE TABLE " << name << " (";
    sql << "_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,";

    for (auto i = fields_from_db.begin(); i != fields_from_db.end(); i++) {
        if (i != fields_from_db.begin())
            sql << ", ";

        sql << i->first << " ";
        if (i->second & INT)
            sql << "INT";
        else
            sql << "STRING";
    }

    sql << ");";
    return true;
}

bool Table::with_neighbor_by_alias(const string& table_alias, function<void(Table&)> fnc) {
    if (neightbor_tables.count(table_alias) == 0)
        return false;
    sTable rel_table = neightbor_tables[table_alias].lock();
    if (rel_table == nullptr)
        return false;
    fnc(*rel_table);
    return true;
}

bool Table::table_ref_name(sTable& t, string& table_name) {
    for (auto kv : t->neightbor_tables) {
        sTable st = kv.second.lock();
        bool comp = (st->name == name);
        st.reset();
        if (comp) {
            table_name = kv.first;
            return true;
        }
    }
    return false;
}
