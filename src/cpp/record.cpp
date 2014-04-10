#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "table.hpp"
#include "record.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;
using std::to_string;
using std::stoi;

bool Record::is_in_db() {
    return record_id >= 0;
};

bool Record::is_changed() {
    return changed;
};

bool Record::set(const string& key, const string& value) {
    if (!from_table.field_valid(key, STRING))
        return false;
    values_to_insert[key] = value;
    return true;
};

bool Record::set(const string& key, int value) {
    if (!from_table.field_valid(key, INT))
        return false;
    values_to_insert[key] = to_string(value);
    return true;
};

bool Record::get(const string& key, int& value) {
    if (!from_table.field_valid(key, INT))
        return false;
    value = stoi(values_from_db[key]);
    return true;
};

bool Record::get(const string& key, string& value) {
    if (!from_table.field_valid(key, STRING))
        return false;
    value = values_from_db[key];
    return true;
};

string Record::to_update_sql() {
    if (values_to_insert.empty())
        return "";
    stringstream sql;
    sql << "UPDATE " << from_table.name << " SET ";
    int size = values_to_insert.size();
    for (auto kv: values_to_insert) {
        sql << kv.first << "='" << kv.second << "'";
        if (size != 1) {
            sql << ", ";
            size--;
        }
    }
    sql << " WHERE " << "id='" << record_id << "';";
    return sql.str();
}
