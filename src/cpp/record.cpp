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


/**
 * Returns true if the record is already in database else returns false.
 */
bool Record::is_in_db() {
    return !values_from_db.empty();
};

/**
 * Returns false whether object is stored in database with
 * the same values as it holds now, otherwise returns true.
 */
bool Record::is_changed() {
    return changed;
};

/**
 * Returns true and add new field whether table name is allowed,
 * false otherwise.
 */
bool Record::could_be_added(const string& name, field_flags type) {
    if (!from_table.is_new_field_valid(name, type))
        return false;
    if (from_table.fields_from_db.count(name) > 0)
        return true;
    if (!from_table.extensible)
        return false;
    fields_to_append[name] = type;
    return true;
}

/**
 * Sets new key with value or updates value for given key.
 * Return false in case column type is unlike given value type, key contains
 * non-alphanumeric character or ‘ id’, record is trying to be updated in
 * protected table or database connection error occurred.
 * Note: New fields are not pushed into database
 */
bool Record::set(const string& key, const string& value) {
    if (!could_be_added(key, STRING))
        return false;
    values_to_insert[key] = value;
    return true;
};

/**
 * Sets new key with value or updates value for given key.
 * Returns false in case column type is unlike given value type, key contains
 * non-alphanumeric character or ‘ id’, record is trying to be updated in
 * protected table or database connection error occurred.
 * Note: New fields are not pushed into database
 */
bool Record::set(const string& key, int value) {
    if (!could_be_added(key, INT))
        return false;
    values_to_insert[key] = to_string(value);
    return true;
};

/**
 * Insert records from vector into neighbor table having many-to-one relation
 * to origin record table. Erase all existing connected rows in foreign tables.
 * Returns false in case column type is unlike given value type, key contains
 * non-alphanumeric character or ‘ id’, record is trying to be updated in
 * protected table or database connection error occurred.
 * Note: New fields are not pushed into database
 */
bool Record::set(const string& key, vector<Record> records) {
    if (!from_table.is_new_field_valid(key, INT))
        return false;
    // cannot add records to non-existing relation
    unsigned long unused;
    if (from_table.neightbor_tables.count(key) == 0)
        return false;
    // TODO check if neighbor and valid cardinality
    records_to_insert[key] = move(records);
    return true;
};

/**
 * Appends record from vector into neighbor table having many-to-one relation
 * to origin record table.
 * Returns false in case column type is unlike given value type, key contains
 * non-alphanumeric character or ‘ id’, record is trying to be updated in
 * protected table or database connection error occurred.
 * Note: New fields are not pushed into database
 */
bool Record::append(const string& key, Record record) {
    if (!from_table.is_new_field_valid(key, INT))
        return false;
    // TODO check if neighbor and valid cardinality
    records_to_insert[key].push_back(move(record));
    return true;
};

/**
 * Returns false if column in table does not exist or database error happen.
 * Otherwise return true and assign attribute of the record to value.
 */
bool Record::get(const string& key, int& value) {
    unsigned long i;
    if (!from_table.get_cell_index(key, i))
        return false;
    assert(i < values_from_db.size());
    cell& r = values_from_db[i];
    if (r.type != INT)
        return false;
    value = r.number;
    return true;
};

/**
 * Returns false if column in table does not exist or database error happen.
 * Otherwise return true and assign attribute of the record to value.
 */
bool Record::get(const string& key, string& value) {
    unsigned long i;
    if (!from_table.get_cell_index(key, i))
        return false;
    assert(i < values_from_db.size());
    cell& r = values_from_db[i];
    if (r.type != STRING)
        return false;
    value = r.text;
    return true;
};

/**
 * Appends update script to 'sql' for all changed values
 */
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

/**
 * Appends into 'sql' insert scripts of all other records 
 * within this record
 */
bool Record::others_to_insert_sql(stringstream& sql) {
    for (auto& kv : records_to_insert) {
        vector<Record>& records = kv.second;
        for (Record& r : records) {
            if (r.is_in_db())
                return false;
        }
    }
    return true;
}

/**
 * Helper method that concatenates insert scripts of this record
 * and all records inside it.
 */
bool Record::to_insert_sql(stringstream& sql) {
    if (self_to_insert_sql(sql) && others_to_insert_sql(sql))
        return true;
    return false;
}

/**
 * Appends into 'sql' all modified column names in brackets
 * (part of insert script)
 */
void Record::insert_columns(stringstream& sql) {
    sql << "(";
    for (auto i = values_to_insert.begin(); i != values_to_insert.end(); i++) {
        if (i != values_to_insert.begin())
            sql << ", ";
        sql << i->first;
    }
    sql << ") ";
}

/**
 * Appends into 'sql' all modified values in brackets
 * (part of insert script)
 */
void Record::insert_values(stringstream& sql) {
    sql << "VALUES (";
    for (auto i = values_to_insert.begin(); i != values_to_insert.end(); i++) {
        if (i != values_to_insert.begin())
            sql << ", ";
        sql << "'" << i->second << "'";
    }
    sql << ");";
}

/**
 * Appends into 'sql' insert script of this record
 */
bool Record::self_to_insert_sql(stringstream& sql) {
    if (values_to_insert.empty())
        return false;
    sql << "INSERT INTO " << from_table.name << " ";
    insert_columns(sql);
    insert_values(sql);
    return true;
}

/**
 * Returns false when database error occur or commits all field set or appended
 * to database and returns true. Each appended record will implicitly
 * call recursively save method itself. For every unknown key new column will
 * be added to the table.
 */
bool Record::save() {
    stringstream sql;
    db->init();
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
