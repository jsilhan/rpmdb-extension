#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include <sqlite3.h>
#include "db.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;


bool Db::init() {
    if (initialized)
        return true;
    if (sqlite3_open(path.c_str(), &sql_db)) {
        cerr << "Can't open database: " << sqlite3_errmsg(sql_db) << "\n";
        return false;
    }
    bool ret = init_tables();
    initialized = true;
    return ret;
}

void Db::add_many_to_one(sTable& t1, sTable& t2,
    string forward_edge, string back_edge, bool required) {
    // TODO handle required
    t1->add_field(forward_edge, RELATION);
    t1->neightbor_tables[forward_edge] = t2;    
    t2->neightbor_tables[back_edge] = t1;    
}

void Db::add_many_to_one(sTable& t1, sTable& t2,
    string forward_edge, bool required) {
    add_many_to_one(t1, t2, forward_edge, t1->name, required);
}

void Db::add_many_to_one(sTable& t1, sTable& t2, bool required) {
    add_many_to_one(t1, t2, t2->name, t1->name, required);
}

bool Db::prepare_select(stringstream& sql, sqlite3_stmt** statement) {
    init();
    int rc = sqlite3_prepare(sql_db, sql.str().c_str(), -1, statement, 0);
    if (rc == SQLITE_OK)
        return true;
    cerr << "SQL error: [select], errcode = " << rc << endl;
    return false;
}

bool Db::init_tables() {
    stringstream sql;
    for (auto& kv : tables) {
        Table& t = *kv.second;
        if (!t.to_init_sql(sql))
            return false;
    }
    return execute(sql.str(), "init_tables");
}

Db::Db(string path) : path(path), sql_db(nullptr),
    initialized(false) {}

Db::~Db() {
    if (sql_db != nullptr)
        sqlite3_close(sql_db);
}

bool Db::execute(string sql, string context) {
    // init(); // FIXME
    char *err_msg = nullptr;
    // cout << "$ " << context << "> " << sql << "\n";
    int rc = sqlite3_exec(sql_db, sql.c_str(), NULL, 0, &err_msg);
    if (rc == SQLITE_OK)
        return true;
    if (!context.empty())
        cerr << "SQL error: " << err_msg << " [" << context << "]" << endl;
    sqlite3_free(err_msg);
    return false;
}

bool Db::add_column(Table& t, const string& column, field_flags flags) {
    stringstream sql;
    sql << "ALTER TABLE " << t.name << " ADD COLUMN " << column;
    if (flags & STRING)
        sql << " STRING;";
    else
        sql << " INT;";
    return execute(sql.str(), "add_column");
}
