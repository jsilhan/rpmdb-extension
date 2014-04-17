#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <functional>
#include <map>

using std::string;
using std::stringstream;
using std::vector;
using std::unique_ptr;
using std::weak_ptr;
using std::shared_ptr;
using std::function;
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
    RELATION = 3,
    // REQUIRED = 1 << 8,
};

struct ReservedField {
    string name;
    field_flags type;
    string app_name;
    string description;
};

// typedef unique_ptr<ReservedField> uReservedField;

class Table {
public:
    string name;
    bool protect;
    bool extensible;
    // vector<uReservedField> reserved_fields;
    map<string,field_flags> fields_from_db;
    map<string,weak_ptr<Table>> neightbor_tables;
    Table(string& name, bool protect=false, bool extensible=false);
    Table(const char* name, bool protect=false, bool extensible=false);
    void add_field(string table_name, field_flags flags, bool required=false);
    bool table_ref_name(shared_ptr<Table>& t, string& table_name);
    bool with_neighbor_by_alias(const string& table_alias, function<void(Table&)> fnc);
    bool field_valid(const string& name, field_flags type);
    bool to_init_sql(stringstream& sql);
};

typedef unique_ptr<Table> uTable;
typedef shared_ptr<Table> sTable;
typedef weak_ptr<Table> wTable;

#endif
