#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "db_ext.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;


enum cardinality {
    ONE_TO_ONE;
    ONE_TO_MANY;
    MANY_TO_ONE;
};

enum pkg_type {
    RPM_PKG;
};

enum field_type {
    INT;
    STRING;
};

int comparator_mask = 0x0f
enum comparator_flags {
    EQ = 0;
    NEQ = 1;
    GT = 2;
    GTE = 3;
    LTE = 4;
    LT = 5;
};

enum value_flag {
    ICASE = 1 << 0;
}

struct FieldFilter {
    vector<string> field_path;
    string value;
    int value_flags;
};

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

struct Query {
    vector<FieldFilter> filter;
    Table relative_to;
    string to_select_sql();
    void filter(string path, string value, int value_flags);
};

struct Fields {
    string name;
    field_type type;
    string app_name;
    string description;
};

void Table::connect(Table other, cardinality c) {

}

struct Table {
    string name;
    vector<Fields> reserved_fields;
    vector<Fields> fields_from_db;
    unordered_map<string,Table> connections;
    string to_init_sql();
};

struct Record {
    Table from_table;
    unordered_map<string,string> values_from_db;
    unordered_map<string,string> values_to_insert;
    bool is_in_db() {};
    bool is_changed() {};
    void set(string key, string value) {};
    void get(string key) {};
    void save() {};
};

struct Swdb {
    vector<Table> tables;
    // parameters
    vector<Package> actors;
    string path;
    pkg_type default_pkg_type
};


