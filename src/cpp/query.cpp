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


int comparator_mask = 0x0f;

void Query::filter(const string& path, string value, int value_flags) {
    uFieldFilter filter(new FieldFilter);
    assert(!path.empty());
    filter->value_flags = value_flags;
    filter->value = value;
    split(path, '.', filter->path);
    filters.push_back(move(filter));
}

string& Query::get_last_table_name(uFieldFilter& filter) {
    if (filter->path.size() == 1)
        return relative_to.name;
    int before_last = filter->path.size() - 2;
    return *(filter->path.at(before_last));
}

void Query::add_where_clauses(uFieldFilter& filter, stringstream& sql) {
    string& table_name = get_last_table_name(filter);
    sql << "WHERE ";

    vector<ustringstream> vect_stream;
    for (uFieldFilter& filter : filters) {
        ustringstream tmp_stream(new stringstream);
        *tmp_stream << table_name << "." << *filter->path.at(filter->path.size() - 1) << " = '" << filter->value << "'";
        // TODO consider flags
        vect_stream.push_back(move(tmp_stream));
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

bool Query::add_join_clauses(vector<ustring>& path, stringstream& sql) {
    auto first = path.rbegin() + 1;
    auto second = path.rbegin() + 2;

    while (1) {
        if (second == path.rend()) {
            if (!add_join_clause(relative_to.name, **first, sql))
                return false;
            break;
        }
        if (!add_join_clause(**second, **first, sql))
            return false;
        first++;
        second++;
    }
    return true;
}

void Query::add_select_clause(stringstream& sql) {
    sql << "SELECT ";
    vector<ustringstream> vect_stream;
    for (auto kv : relative_to.fields_from_db) {
        ustringstream tmp_stream(new stringstream);
        *tmp_stream << relative_to.name << "." << kv.first;
        // TODO consider flags
        vect_stream.push_back(move(tmp_stream));
    }
    append_joined(sql, vect_stream, ", ");

    sql << " FROM " << relative_to.name << ";";
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
