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

#ifndef QUERY_H
#define QUERY_H

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

int comparator_mask = 0x0f;
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
    string path;
    string value;
    int value_flags;
};

typedef unique_ptr<FieldFilter> uFieldFilter;

class Query {
private:
    Swdb& db;
    Table& relative_to;
    vector<uFieldFilter> filters;
public:
    Query(Swdb& db, Table& t) : db(db), relative_to(t) {};
    Query(Swdb& db, string& n) : db(db), relative_to(db.tables[n]) {};
    string to_select_sql();
    void filter(string path, string value, int value_flags);
};

#endif
