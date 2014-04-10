#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "table.hpp"
#include "connections.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;

bool Connections::fill_tr(string& t1_name, string& t2_name, table_relation& tr) {
    if (table_relations.find(t1_name) == table_relations.end())
        return false;
    tr.t1_column = t1_name + ".id";
    tr.t2_column = t2_name + "._" + t1_name;
    return true;
}

void Connections::connect_tables(Table& t1, Table& t2, cardinality c) {
    if (c == ONE_TO_MANY)
        table_relations[t1.name] = t2.name;
    else
        table_relations[t2.name] = t1.name;
}

bool Connections::get_relation(string& t1_name, string& t2_name, table_relation &tr) {
    if (fill_tr(t1_name, t2_name, tr) || fill_tr(t2_name, t1_name, tr))
        return true;
    return false;
}
