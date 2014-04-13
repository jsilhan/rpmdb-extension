#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <memory>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::unique_ptr;
using std::map;


enum cardinality {
    ONE_TO_ONE,
    ONE_TO_MANY,
    MANY_TO_ONE,
};

enum pkg_type {
    RPM_PKG,
};

enum field_flags {
    INT = 1,
    STRING = 2,
    REQUIRED = 1 << 8,
};

struct ReservedField {
    string name;
    field_flags type;
    string app_name;
    string description;
};

typedef unique_ptr<ReservedField> uReservedField;

class Table {
public:
    string name;
    bool protect;
    bool expandable;
    vector<uReservedField> reserved_fields;
    map<string,field_flags> fields_from_db;
    Table(string& name, bool protect=false);
    Table(const char* name, bool protect=false);
    void add_field(string name, field_flags flags);
    bool field_valid(const string& name, field_flags type);
    string to_init_sql();
};

typedef unique_ptr<Table> uTable;
#endif
