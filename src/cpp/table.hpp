#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#ifndef TABLE_HPP
#define TABLE_HPP

using std::string;
using std::vector;
using std::unique_ptr;
using std::unordered_map;

enum cardinality {
    ONE_TO_ONE,
    ONE_TO_MANY,
    MANY_TO_ONE,
};

enum pkg_type {
    RPM_PKG,
};

enum field_type {
    INT,
    STRING,
};

struct ReservedField {
    string name;
    field_type type;
    string app_name;
    string description;
};

class Table {
public:
    string name;
    vector<ReservedField> reserved_fields;
    unordered_map<string,field_type> fields_from_db;
    Table(string& name);
    Table(const char* name);
    void add_field(string name, field_type type);
    bool field_valid(string& name, field_type type);
    string to_init_sql();
};

#endif
