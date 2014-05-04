#ifndef DB_H
#define DB_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include <assert.h>
#include <string.h>
#include <sqlite3.h>
#include "table.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::unordered_map;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;


class Db {
public:
    unordered_map<string,sTable> tables;
    bool initialized;
    sqlite3 *sql_db;
    string path;
    Db(string path="");
    ~Db();
    bool init();
    bool init_tables();
    void add_many_to_one(sTable& t1, sTable& t2,
        string forward_edge, string back_edge, bool required=false);
    void add_many_to_one(sTable& t1, sTable& t2,
        string forward_edge, bool required=false);
    void add_many_to_one(sTable& t1, sTable& t2, bool required=false);
    bool execute(string sql, string context);
    bool add_column(Table& t, const string& column, field_flags flags);
    bool prepare_select(stringstream& sql, sqlite3_stmt** statement);
};

typedef shared_ptr<Db> sDb;

#endif
