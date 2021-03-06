#ifndef SWDB_H
#define SWDB_H

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
#include "query.hpp"
#include "record.hpp"
#include "table.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;


struct Swdb {
    sDb db;
    vector<string> actors;
    pkg_type default_pkg_type;
    Swdb(string path="test.db", int type=RPM_PKG);
    bool init();
    uRecord urecord(string table_name);
    Record record(string table_name);
    uQuery uquery(string table_name);
    Query query(string table_name);
    bool create_index(const string& table_name, const string& field_name, bool unique=false);
};

#endif
