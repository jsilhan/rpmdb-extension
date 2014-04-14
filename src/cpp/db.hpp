#ifndef DB_H
#define DB_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include <sqlite3.h>
#include "table.hpp"
#include "connections.hpp"


using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;


class Db {
public:
    unordered_map<string,uTable> tables;
    Connections connections;
    bool initialized;
    sqlite3 *sql_db;
    string path;
    Db(string path);
    ~Db();
    bool init();
    bool init_tables();
    bool execute(string sql, string context);
    bool add_column(Table& t, const string& column, field_flags flags);
};

#endif
