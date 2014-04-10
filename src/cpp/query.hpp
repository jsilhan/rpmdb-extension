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

typedef unique_ptr<string> ustring;

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
    Swdb& db;
    Table& relative_to;
    vector<uFieldFilter> filters;
    void add_select_clause(stringstream& sql);
    bool add_join_clauses(vector<ustring>& path, stringstream& sql);
    bool add_join_clause(string& t1_name, string& t2_name, stringstream& sql);
    void add_where_clauses(uFieldFilter& filter, stringstream& sql);
    string& get_last_table_name(uFieldFilter& filter);
public:
    Query(Swdb& db, Table& t) : db(db), relative_to(t) {};
    // Query(Swdb& db, string& n) : db(db), relative_to(db.tables[n]) {};
    string to_select_sql();
    void filter(const string& path, string value, int value_flags);
};

#endif
