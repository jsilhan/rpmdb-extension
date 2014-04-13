#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include <unordered_map>
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
using std::unordered_map;

struct table_relation {
    string t1_column;
    string t2_column;
};

class Connections {
private:
    unordered_map<string,string> table_relations;
    bool fill_tr(string& t1_name, string& t2_name, table_relation& tr);
public:
    void connect_tables(string& t1, string& t2, cardinality c);
    bool get_relation(string& t1_name, string& t2_name, table_relation& tr);
};

#endif
