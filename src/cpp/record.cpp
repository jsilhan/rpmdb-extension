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

bool Record::is_in_db() {
    return record_id >= 0;
};

bool Record::is_changed() {
    return changed;
};

void Record::set(string key, string value) {
    if (!is_reserved(key))
        values_to_insert[key] = value;
};

void Record::get(string key) {
    return values_from_db[key];
};

string Record::to_update_sql() {
    if (values_to_insert.empty())
        return "";
    stringstream sql;
    sql << "UPDATE " << from_table << " SET ";
    int size = values_to_insert.size();
    for (string& key: values_to_insert) {
        sql << key << "='" << values_to_insert[key] << "'";
        if (size != 1) {
            sql << ", ";
            size--;
        }
    }
    sql << " WHERE " << "id='" << record_id << "';";
    return sql.str();
}
