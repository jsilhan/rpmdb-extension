#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "db.hpp"

using std::cout;
using std::cerr;
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
    if (sqlite3_open(path.c_str(), &sql_db))
        cerr << "Can't open database: " << sqlite3_errmsg(sql_db) << "\n";
    bool ret = init_tables();
    initialized = true;
    return ret;
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

Db::Db(string path) : sql_db(nullptr),
    initialized(false), path(path) {}

Db::~Db() {
    sqlite3_close(sql_db);
}

bool Db::execute(string sql, string context) {
    char *err_msg = nullptr;
    int rc = sqlite3_exec(sql_db, sql.c_str(), NULL, 0, &err_msg);
    if (rc == SQLITE_OK)
        return true;
    if (!context.empty())
        cerr << context << ", SQL error: " << err_msg << "\n";
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
