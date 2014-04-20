#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include <unordered_map>
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


class Record {
private:
    int record_id;
    bool changed;
    map<string,string> values_from_db;
    unordered_map<string,string> values_to_insert;
    unordered_map<string,uvector<unique_ptr<Record>>> records_to_insert;
    Table& from_table;
    Db& db;
public:
    Record(Db& db, Table& t, int id=-1) :
        db(db), from_table(t), record_id(id), changed(false) {};
    Record(Db& db, string& tn, int id=-1) :
        db(db), from_table(*db.tables[tn]), record_id(id), changed(false) {};
    bool is_in_db();
    bool is_changed();
    int id() { return record_id; }
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
