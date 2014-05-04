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


class Swdb {
public:
    sDb db;
    vector<ustring> actors;
    pkg_type default_pkg_type;
    Swdb(string path="test.db", pkg_type type=RPM_PKG);
    bool init();
    uRecord record(string table_name);
    uQuery query(string table_name);
};

#endif
