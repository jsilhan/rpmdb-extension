#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "table.hpp"
#include "query.hpp"
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


void Query::filter(string path, string value, int value_flags) {
    uFieldFilter filter(new FieldFilter);
    assert(!path.empty());
    filter->value_flags = value_flags;
    filter->value = value;
    split(filter->path, '.', path);
    filters.push_back(f);
}

void add_where_clauses(uFieldFilter& filter, stringstream& sql) {
    string& table_name;
    if (filter.path.size() == 1)
        table_name = relative_to.name;
    else
        table_name = filter.path.at(filter.path.size() - 2);
    sql << "WHERE ";

    vector<ustringstream> vect_stream;
    for (uFieldFilter& filter : filters) {
        ustringstream tmp_stream(new stringstream);
        tmp_stream << table_name << "." << filter.path.last() << " = '" << filter.value << "'";
        // TODO consider flags
        vect_stream.push_back(tmp_stream);
    }
    append_joined(sql, vect_stream, ", ");
}

bool Query::add_join_clause(string& t1_name, string& t2_name, stringstream& sql) {
    table_relation tr;
    if (!db.connections.get_relation(t1_name, t2_name, tr))
        return false;
    sql << "JOIN " << t2_name << " ON " << tr.t1_column << " = " << tr.t2_column << "\n";
    return true;
}

void Query::add_join_clauses(vector<string>& path, stringstream& sql) {
    auto first = s.rbegin() + 1;
    auto second = s.rbegin() + 2;

    while (1) {
        if (second == s.rend()) {
            if (!add_join_clause(relative_to.name, *first, sql))
                return false;
            break;
        }
        if (!add_join_clause(*second, *first, sql))
            return false;
        first++;
        second++;
    }
}

void Query::add_select_clause(stringstream& sql) {
    sql << "SELECT ";
    int num_columns = relative_to.fields_from_db.size();
    int i = 0;
    for (auto kv : relative_to.fields_from_db) {
        i++;
        sql << relative_to.name << "." << kv.first;
        if (i != num_columns)
            sql << ", "
    }
    sql << " FROM " << relative_to << ";";
}

string Query::to_select_sql() {
    stringstream sql;
    add_select_clause(sql);
    for (uFieldFilter& filter : filters) {
        add_join_clauses(filter->path, sql);
    }
    for (uFieldFilter& filter : filters) {
        add_where_clauses(filter, sql);
    }
    sql << ";";
    return sql.str();
}
