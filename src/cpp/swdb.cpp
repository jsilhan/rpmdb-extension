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
#include "swdb.hpp"


using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;

Swdb::Swdb(string path, pkg_type type) :
    db(path), default_pkg_type(type) {
        // TODO actor params
        db.init();        
}

uRecord Swdb::record(string& table_name) {
    return uRecord(new Record(db, table_name));
}

uQuery Swdb::query(string& table_name) {
    return uQuery(new Query(db, table_name));
}
