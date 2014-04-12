#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <chrono>
#include <vector>
#include <memory>
#include <string>
#include <sqlite3.h>
#include <assert.h>
#include <string.h>
#include "extend.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::basic_string;

bool Swdb::execute(const string& sql, const char *context) {
    char *zErrMsg = nullptr;
    int rc = sqlite3_exec(pdb, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc == SQLITE_OK)
        return true;
    if (*context != '\0')
        cerr << context << ", SQL error: " << zErrMsg << "\n";
    sqlite3_free(zErrMsg);
    return false;
}

bool Swdb::init_table() {
    // TODO IF NOT EXIST
    string sql = "CREATE TABLE packages ("  \
        "id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
        "name TEXT NOT NULL," \
        "type INT NOT NULL);" \
        "CREATE UNIQUE INDEX name_index ON PACKAGES (name, type);";
    
    return execute(sql, "init_table");
}

bool Swdb::insert_record(const string& name, int type) {
    stringstream sql;
    sql << "INSERT OR IGNORE INTO packages (id, name, type) VALUES (NULL, '";
    sql << name << "', " << type << ");";
    return execute(sql.str(), "insert_record");
}

bool Swdb::init() {
    if (initialized)
        return true;
    if (sqlite3_open(path, &pdb)) {
        cerr << "Can't open database: " << sqlite3_errmsg(pdb) << "\n";
    } else {
        cout << "Opened database successfully\n";
    }
    int ret = init_table();
    initialized = true;
    return ret;
}

Swdb::~Swdb() {
    sqlite3_close(pdb);
}

Swdb::Swdb(const string path, int init_flags=0) : pdb(nullptr),
    initialized(false) {}

void Swdb::iter_one_column(const string& query, int column,
    function<bool(string& value)>fnc) {
    sqlite3_stmt *statement;

    if (sqlite3_prepare(pdb, query.c_str(), -1, &statement, 0) == SQLITE_OK) {

        while (true) {
            int res = sqlite3_step(statement);

            if (res == SQLITE_ROW) {
                string s = (char *) sqlite3_column_text(statement, column);
                if (fnc(s)) break;
            }
            
            if (res == SQLITE_DONE || res==SQLITE_ERROR) {
                break;
            }    
        }
    }
}

bool Swdb::get_field(pkg_type type, const string& name, const string& column,
    string& value) {
    if (!init())
        return false;
    stringstream query;
    query << "SELECT " << column << " FROM packages WHERE name = '";
    query << name << "' AND type = " << type << ";";
    bool found = false;
    iter_one_column(query.str(), 0, [&](string& val) {
        value = val;
        found = true;
        return true;
    });
    return found;
}

bool Swdb::get_field(pkg_type type, const string& name, const string& column,
    int& value) {
    string val;
    bool ret = get_field(type, name, column, val);
    try {
        value = std::stoi(val);
    } catch (std::exception& e) {
        return false;
    }
    return ret;
}

bool Swdb::update_value(int type, const string& name, const string& column,
    bool override, function<void(stringstream&)> fnc) {
    if (!init())
        return false;
    add_column(column, true);
    stringstream sql;
    // TODO handle override
    sql << "UPDATE packages SET " << column << " = '";
    fnc(sql);
    sql << "' WHERE name = '" << name << "' AND type = " << type << ";";
    return execute(sql.str(), "update_value");
}

bool Swdb::set_field(pkg_type type, const string& name, const string& column,
    const string& value, bool override) {
    return update_value(type, name, column, override, [&](stringstream& sql) {
        sql << "'" << value << "'";
    });
}

bool Swdb::set_field(pkg_type type, const string& name, const string& column,
    int value, bool override) {
    return update_value(type, name, column, override, [&](stringstream& sql) {
        sql << value;
    });
}

bool Swdb::add_column(const string& column, bool text) {
    stringstream sql;
    sql << "ALTER TABLE packages ADD COLUMN " << column;
    if (text)
        sql << " TEXT;";
    else
        sql << " INT;";
    return execute(sql.str(), "");
}

// int main(int argc, const char *argv[]) {
//     Swdb db("test.db", 0);
//     const string pkg_name("N:E-V-R.A");
//     const string column1("new_text_column");
//     const string column2("new_int_column");
//     const string val1("val");
//     const int val2 = 44;

//     db.set_field(RPM_PKG, pkg_name, column1, val1, true);
//     db.set_field(RPM_PKG, pkg_name, column2, val2, false);
//     cout << "works";
// }
