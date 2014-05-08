#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include <iterator>
#include "table.hpp"
#include "record.hpp"

using std::cout;
using std::cerr;
using std::distance;
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
    return !values_from_db.empty();
};

bool Record::is_changed() {
    return changed;
};

bool Record::could_be_added(const string& name, field_flags type) {
    if (!from_table.is_new_field_valid(name, type))
        return false;
    if (from_table.fields_from_db.count(name) == 0)
        if (!from_table.extensible)
            return false;
    fields_to_append[name] = type;
    return true;
}

bool Record::set(const string& key, const string& value) {
    if (!could_be_added(key, STRING))
        return false;
    values_to_insert[key] = value;
    return true;
};

bool Record::set(const string& key, int value) {
    if (!could_be_added(key, INT))
        return false;
    values_to_insert[key] = to_string(value);
    return true;
};

bool Record::set(const string& key, vector<uRecord> records) {
    if (!from_table.is_new_field_valid(key, INT))
        return false;
    // cannot add records to non-existing relation
    unsigned long unused;
    if (from_table.neightbor_tables.count(key) == 0)
        return false;
    // TODO check if neighbor and valid cardinality
    // if (records_to_insert.count(key) > 0)
    //     delete records_to_insert[key].release();
    records_to_insert[key] = move(records);
    return true;
};

bool Record::append(const string& key, uRecord record) {
    if (!from_table.is_new_field_valid(key, INT))
        return false;
    // TODO check if neighbor and valid cardinality
    records_to_insert[key].push_back(move(record));
    return true;
};

bool Record::get(const string& key, int& value) {
    unsigned long i;
    if (!from_table.get_cell_index(key, i))
        return false;
    if (i >= values_from_db.size())
        return false;
    cell& r = values_from_db[i];
    if (r.type != INT)
        return false;
    value = r.number;
    return true;
};

bool Record::get(const string& key, string& value) {
    unsigned long i;
    if (!from_table.get_cell_index(key, i))
        return false;
    if (values_from_db[i].type != STRING)
        return false;
    value = values_from_db[i].text;
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
    sql << " WHERE " << "id='" << id() << "';";
    return true;
}

bool Record::others_to_insert_sql(stringstream& sql) {
    for (auto& kv : records_to_insert) {
        vector<uRecord>& records = kv.second;
        for (uRecord& r : records) {
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
    for (auto kv : fields_to_append) {
        db->add_column(from_table, kv.first, kv.second);
    }
    if (db->execute(sql.str(), "update/insert record"))
        return true;
    return false;
}
