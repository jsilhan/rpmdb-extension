#ifndef QUERY_H
#define QUERY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "table.hpp"
#include "db.hpp"
#include "utils.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;

extern int comparator_mask;
enum comparator_flags {
    EQ = 0,
    NEQ = 1,
    GT = 2,
    GTE = 3,
    LTE = 4,
    LT = 5,
};

enum value_flag {
    ICASE = 1 << 8,
};

struct FieldFilter {
    vector<ustring> path;
    string value;
    int value_flags;
};

typedef unique_ptr<FieldFilter> uFieldFilter;

class Query {
private:
    Db& db;
    sTable relative_to;
    vector<uFieldFilter> filters;
    void add_select_clause(stringstream& sql);
    bool add_join_clauses(vector<ustring>& path, stringstream& sql);
    bool add_join_clause(sTable& t, string& table_alias, stringstream& sql);
    void add_where_clauses(uFieldFilter& filter, stringstream& sql);
    string& get_last_table_name(uFieldFilter& filter);
public:
    Query(Db& db, sTable t) : db(db), relative_to(t) {};
    Query(Db& db, string& tn) : db(db), relative_to(db.tables[tn]) {};
    string to_select_sql();
    void filter(const string& path, string value, int value_flags);
};

typedef unique_ptr<Query> uQuery;

#endif
