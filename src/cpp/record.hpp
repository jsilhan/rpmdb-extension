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
#include "swdb.hpp"
#include "table.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::map;
using std::unique_ptr;
using std::move;
using std::vector;


class Record {
private:
    int record_id;
    bool changed;
    bool protect;
    map<string,string> values_from_db;
    unordered_map<string,string> values_to_insert;
    Table& from_table;
    Swdb& db;
public:
    Record(Swdb& db, Table& t, int id=-1, bool protect=false) :
        db(db), from_table(t), record_id(id), protect(protect) {
            changed = false;
    };
    bool is_in_db();
    bool is_changed();
    bool set(const string& key, const string& value);
    bool set(const string& key, int value);
    bool get(const string& key, string& value);
    bool get(const string& key, int& value);
    string to_update_sql();
    string to_insert_sql();
    bool save();
};

typedef unique_ptr<Record> uRecord;

#endif
