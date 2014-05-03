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
#include <sqlite3.h>
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


struct row {
    field_flags type;
    string text;
    int number;
    row(field_flags t, string s) : type(t), text(s) {};
    row(field_flags t, int i) : type(t), number(i) {};
    row() : type(NULL_VAL) {};
    row(sqlite3_stmt *statement, int i) {
        switch (sqlite3_column_type(statement, i)) {
        case SQLITE_NULL: {
            type = NULL_VAL;
        } 
            break;
        case SQLITE_INTEGER:{
            type = INT;
            number = sqlite3_column_int(statement, i);
        }
            break;
        case SQLITE_TEXT: {
            type = STRING;
            const unsigned char *t = sqlite3_column_text(statement, i);;
            text = reinterpret_cast<const char*>(t);
        }
            break;
        default:
            type = NULL_VAL;
        }
    }
};


class Record {
private:
public:
    int record_id;
    bool changed;
    vector<row> values_from_db; // TODO could be vector<union {string, int}>
    unordered_map<string,string> values_to_insert;
    unordered_map<string,uvector<unique_ptr<Record>>> records_to_insert;
    Table& from_table;
    Db& db;
    Record(Db& db, Table& t, int id=-1) :
        db(db), from_table(t), record_id(id), changed(false) {};
    Record(Db& db, string& tn, int id=-1) :
        db(db), from_table(*db.tables[tn]), record_id(id), changed(false) {};
    Record(Db& db, Table& t, sqlite3_stmt* statement) :
        db(db), from_table(t), changed(false) {
            record_id = sqlite3_column_int(statement, 0);
            cout << "### reco constructor, columns = " << sqlite3_column_count(statement) << endl;
            for (int i = 0; i < sqlite3_column_count(statement); i++) {
                // TODO union for field representation
                // if (sqlite3_column_type(statement) ==  SQLITE_INTEGER)
                // if (sqlite3_column_type(statement) ==  SQLITE_TEXT)
                values_from_db.push_back(row(statement, i));
            }
        };
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
