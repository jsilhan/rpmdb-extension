#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <unordered_map>
#include <sqlite3.h>
#include <gtest/gtest_prod.h>
#include "db.hpp"
#include "table.hpp"
#include "utils.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::map;
using std::unique_ptr;
using std::shared_ptr;
using std::move;
using std::vector;


struct cell {
    field_flags type;
    string text;
    int number;
    cell(field_flags t, string s) : type(t), text(s) {};
    cell(field_flags t, int i) : type(t), number(i) {};
    cell() : type(NULL_VAL) {};
    cell(sqlite3_stmt *statement, int i) {
        int column_type = sqlite3_column_type(statement, i);
        if (column_type == SQLITE_INTEGER) {
            type = INT;
            number = sqlite3_column_int(statement, i);
        } else if (column_type == SQLITE_TEXT) {
            const unsigned char *t = sqlite3_column_text(statement, i);;
            type = STRING;
            text = reinterpret_cast<const char*>(t);
        } else
            type = NULL_VAL;
    }
};

typedef unique_ptr<cell> ucell;

class Record {
private:
    FRIEND_TEST(RecordTest, TestRecordGet);
public:
    bool changed;
    vector<ucell> values_from_db;
    unordered_map<string,string> values_to_insert;
    unordered_map<string,uvector<unique_ptr<Record>>> records_to_insert;
    Table& from_table;
    sDb db;
    Record(sDb db, Table& t) :
        db(db), from_table(t), changed(false) {};
    Record(sDb db, string& tn) :
        db(db), from_table(*db->tables[tn]), changed(false) {};
    Record(sDb db, Table& t, sqlite3_stmt* statement) :
        db(db), from_table(t), changed(false) {
            for (int i = 0; i < sqlite3_column_count(statement); i++) {
                ucell c(new cell(statement, i));
                values_from_db.push_back(move(c));
            }
        };
    bool is_in_db();
    bool is_changed();
    int id() {
        if (values_from_db.empty())
            return -1;
        return values_from_db[0]->number;
    }
    bool set(const string& key, const string& value);
    bool set(const string& key, int value);
    bool set(const string& key, uvector<unique_ptr<Record>> records);
    bool append(const string& key, unique_ptr<Record> record);
    bool get(const string& key, string& value);
    bool get(const string& key, int& value);
    bool set(const string& table_alias, Record& record);
    int last_id();
    void insert_columns(stringstream& sql);
    void insert_values(stringstream& sql);
    bool to_insert_sql(stringstream& sql);
    bool others_to_insert_sql(stringstream& sql);
    bool self_to_insert_sql(stringstream& sql);
    bool to_update_sql(stringstream& sql);
    bool save();
};

typedef unique_ptr<Record> uRecord;

#endif
