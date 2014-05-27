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


/**
 * Returns true whether table name and type is allowed, false otherwise
 */
bool Table::is_new_field_valid(const string& name, field_flags type) {
    if (!is_valid_field_name(name))
        return false;
    // key exist in table with different type
    if (fields_from_db.count(name) > 0 && fields_from_db[name] != type)
        return false;
    return true;
}

/**
 * Returns true whether table name is allowed, false otherwise
 */
bool Table::is_valid_field_name(const string& name) {
    for (const char& c : name) {
        if (!isalnum(c))
            return false;
    }
    return true;
}

/**
 * Adds new field to the table. When required is set, value of the field
 * cannot be NULL
 */
void Table::add_field(string table_name, field_flags flags, bool required) {
    fields_from_db[table_name] = flags;
}

/**
 * Assigns index of column named 'key' to 'i' and return true,
 * else returns false
 */
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
 * Update table metadata (column name and type) from recent record.
 * Record can have more (by appending new columns)
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

/**
 * Translates table object into init script. 'Sql' value is appended
 * with commands.
 */
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

/**
 * Assigns to 'table_name' how 't' neighbor table references this table
 */
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
