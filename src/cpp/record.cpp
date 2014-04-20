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

bool Record::set(const string& key, uvector<uRecord> records) {
    if (!from_table.field_valid(key, INT))
        return false;
    // TODO check if neighbor and valid cardinality
    if (records_to_insert.find(key) != records_to_insert.end())
        delete records_to_insert[key].release();
    records_to_insert[key] = move(records);
    return true;
};

bool Record::append(const string& key, uRecord record) {
    if (!from_table.field_valid(key, INT))
        return false;
    // TODO check if neighbor and valid cardinality
    if (records_to_insert.count(key) > 0)
        records_to_insert[key] = uvector<uRecord>(new vector<uRecord>);
    records_to_insert[key]->push_back(move(record));
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

bool Record::to_update_sql(stringstream& sql) {
    if (values_to_insert.empty())
        return false;
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
    return true;
}

bool Record::others_to_insert_sql(stringstream& sql) {
    for (auto& kv : records_to_insert) {
        uvector<uRecord>& records = kv.second;
        for (uRecord& r : *records) {
            if (r->is_in_db())
                return false;
            // if (!r->set_fk(this) || !r->to_insert_sql(sql))
            //     return false;
        }
    }
    return true;
}

bool Record::to_insert_sql(stringstream& sql) {
    if (self_to_insert_sql(sql) && others_to_insert_sql(sql))
        return true;
    return false;
}

int Record::last_id() {
    stringstream sql;
    sql << "select seq from sqlite_sequence where name='";
    sql << from_table.name << "';";
    // TODO execute
    return 34;
}

void Record::insert_columns(stringstream& sql) {
    sql << "(";
    for (auto i = values_to_insert.begin(); i != values_to_insert.end(); i++) {
        if (i != values_to_insert.begin())
            sql << ", ";
        sql << i->first;
    }
    sql << ") ";
}

void Record::insert_values(stringstream& sql) {
    sql << "VALUES (";
    for (auto i = values_to_insert.begin(); i != values_to_insert.end(); i++) {
        if (i != values_to_insert.begin())
            sql << ", ";
        sql << "'" << i->second << "'";
    }
    sql << ");";
}

bool Record::self_to_insert_sql(stringstream& sql) {
    if (values_to_insert.empty())
        return false;
    sql << "INSERT INTO " << from_table.name << " ";
    insert_columns(sql);
    insert_values(sql);
    return true;
}

bool Record::save() {
    stringstream sql;
    // setup savepoint
    if (is_in_db()) {
        if (from_table.protect || !to_update_sql(sql))
            return false;
    } else if (!to_insert_sql(sql)) {
        return false;
    }
    // if (execute(sql.str()))
    //     return true;
    return false;
}
