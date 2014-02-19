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
#include "db_ext.hpp"
#include "bdb/read.hpp"

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

enum type {
    TEXT = 0,
    INT
};

bool MyDb::execute(const string& sql, const char *context) {
    char *zErrMsg = nullptr;
    int rc = sqlite3_exec(pdb, sql.c_str(), NULL, 0, &zErrMsg);
    if (rc == SQLITE_OK)
        return true;
    if (*context != '\0')
        cerr << context << ", SQL error: " << zErrMsg << "\n";
    sqlite3_free(zErrMsg);
    return false;
}

bool MyDb::init_table() {
    string sql = "CREATE TABLE packages IF NOT EXISTS ("  \
        "id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
        "name TEXT NOT NULL," \
        "type INT NOT NULL);" \
        "CREATE UNIQUE INDEX name_index ON PACKAGES (name, type);";
    
    return execute(sql, "init_table");
}

bool MyDb::insert_record(const string& name, int type) {
    stringstream sql;
    sql << "INSERT OR IGNORE INTO PACKAGES (ID, NAME, TYPE) VALUES (NULL, '";
    sql << name << "', " << type << ");";
    return execute(sql.str(), "insert_record");
}

bool MyDb::init() {
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

MyDb::~MyDb() {
    sqlite3_close(pdb);
}

MyDb::MyDb(const string path, int init_flags=0) : pdb(nullptr),
    initialized(false) {}

void MyDb::iter_one_column(const string& query, int column,
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

bool MyDb::get_field(int type, const string& name, const string& column,
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

bool MyDb::get_field(int type, const string& name, const string& column,
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

bool MyDb::update_value(int type, const string& name, const string& column,
    bool override, function<void(stringstream&)> fnc) {
    if (!init())
        return false;
    add_column(column, true);
    stringstream sql("UPDATE OR ");
    if (override)
        sql << "REPLACE";
    else
        sql << "IGNORE";
    sql << "packages SET '" << column << "' = '";
    fnc(sql);
    sql << "' WHERE name = '" << name << "' AND type = " << type << ";";
    return execute(sql.str(), "update_value");
}

bool MyDb::set_field(int type, const string& name, const string& column,
    const string& value, bool override) {
    return update_value(type, name, column, override, [&](stringstream& sql) {
        sql << "'" << value << "'";
    });
}

bool MyDb::set_field(int type, const string& name, const string& column,
    int value, bool override) {
    return update_value(type, name, column, override, [&](stringstream& sql) {
        sql << value;
    });
}

bool MyDb::add_column(const string& column, bool text) {
    stringstream sql;
    sql << "ALTER TABLE packages ADD COLUMN " << column;
    if (text)
        sql << " TEXT;";
    else
        sql << " INT;";
    return execute(sql.str(), "");
}


void insert_records(MyDb& db, BDB& bdb) {
    // bdb.iter_keys([&](Dbt& key, Dbt& data) {
    //     string str((char *) key.get_data(), key.get_size());
    //     db.insert_record(str, 0);
    //     return false;
    // });
}

void time_for_200_samples(MyDb& db, BDB& bdb) {

    vector<string*>* samples = bdb.get_200_samples();

    auto t0 = high_resolution_clock::now();

    string value;
    for (string* str: *samples) {
        db.get_field(0, *str, "name", value);
    }

    auto t1 = high_resolution_clock::now();
    milliseconds total_ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
    cout << "duration = " << total_ms.count() << "\n";
    // memory leak!
}


void list_columns(MyDb& db) {
    // for (column& c: db.get_columns()) {
    //     cout << c.name << ",\t\t" << "type: ";
    //     if (c.text == true)
    //         cout << "TEXT\n";
    //     else
    //         cout << "INT\n";
    // }
}

int main(int argc, const char *argv[]) {
    MyDb db("test.db", 0);
    // BDB bdb("Name");
    // insert_records(db, bdb);
    // time_for_200_samples(db, bdb);
    // if (argc == 2) {
    //     if (strcmp(argv[1], (char *) "list") == 0)
    //         list_columns(db);
    //     return EXIT_SUCCESS;
    // }
    // db.add_column("new_text_column", true);
    // db.add_column("new_int_column", false);
    cout << "works";
}
