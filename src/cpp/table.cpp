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

bool Table::is_new_field_valid(const string& name, field_flags type) {
    if (!is_valid_field_name(name))
        return false;
    if (!is_field_in_db_or_extensible(name))
        return false;
    // key exist in table with different type
    if (fields_from_db[name] != type)
        return false;
    return true;
}

bool Table::is_valid_field_name(const string& name) {
    for (const char& c : name) {
        if (!isalnum(c))
            return false;
    }
    return true;
}

bool Table::is_field_in_db_or_extensible(const string& name) {
    // can create new fields only for extensible tables
    if (fields_from_db.count(name) == 0 &&
        !extensible)
        return false;
    return true;
}

void Table::add_field(string table_name, field_flags flags, bool required) {
    fields_from_db[table_name] = flags;
}

bool Table::get_cell_index(const string& key, unsigned long& i) {
    auto it = fields_from_db.find(key);
    if (it == fields_from_db.end())
        return false;
    // in fields_from_db isn't stored '_id' column but in
    // record.values_from_db is
    i = distance(fields_from_db.begin(), it) + 1;
    return true;
}

/**
 * Update table metadata (column name and type) from recent record
 * record can have more (by appending new columns)
 * or the same number of fields.
 * Note that added columns cannot be removed.
 */
void Table::update_fields_metadata(sqlite3_stmt* statement) {
    unsigned long columns_count = sqlite3_column_count(statement);
    if (columns_count == fields_from_db.size())
        return;
    assert(columns_count > fields_from_db.size());
    // first column is '_id' and isn't included in fields_from_db
    for (unsigned long i = fields_from_db.size() + 1; i < columns_count; i++) {
        const char *column_name = sqlite3_column_name(statement, i);
        fields_from_db[string(column_name)] = APPENDED;
    }
}

Table::Table(string& name, bool protect, bool extensible) :
    name(name), protect(protect), extensible(extensible) {}

Table::Table(const char* name, bool protect, bool extensible) :
    name(name), protect(protect), extensible(extensible) {}

bool Table::to_init_sql(stringstream& sql) {
    sql << "CREATE TABLE IF NOT EXISTS " << name << " (";
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

bool Table::table_ref_name(Table& t, string& table_name) {
    for (auto kv : t.neightbor_tables) {
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
