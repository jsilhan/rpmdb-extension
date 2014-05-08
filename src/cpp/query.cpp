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
#include <unistd.h>

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
    FieldFilter filter;
    assert(!path.empty());
    filter.value_flags = value_flags;
    filter.value = value;
    split(path, '.', filter.path);
    filters.push_back(move(filter));
}

bool Query::add_where_clauses(FieldFilter& filter, stringstream& sql,
    string& last_table) {

    sql << "WHERE ";

    for (auto i = filters.begin(); i != filters.end(); i++) {
        if (i != filters.begin())
            sql << ", ";
        vector<string>& path = i->path;
        sql << last_table << "." << path.at(path.size() - 1);
        sql << " = '" << i->value << "'";
        // TODO consider flags
    }
    return true;
}

bool Query::add_join_clause(sTable& t, string& table_alias, string& last_field, stringstream& sql) {
    if (t->neightbor_tables.count(table_alias) == 0)
        return false;

    bool ret = true;
    sTable other_st = t->neightbor_tables[table_alias].lock();
    Table other_t = *other_st;
    sql << "JOIN " << other_t.name << " ON ";

    if (t->fields_from_db.count(table_alias) > 0) { // many to one
        sTable rel_table = relative_to->neightbor_tables[table_alias].lock();
        sql << t->name << "." << rel_table->name << " = ";
        sql << other_t.name << "._id ";
    } else { // one to many
        string self_name;
        if (!t->table_ref_name(other_t, self_name))
            ret = false;
        else
            sql << t->name << "._id = " << other_t.name << "." << self_name << " ";
    }

    last_field = other_t.name;
    return ret;
}

bool Query::add_join_clauses(vector<string>& path, stringstream& sql, string& last_table_name) {
    sTable &first_table = relative_to;
    last_table_name = relative_to->name;
    for (unsigned long i = 0; i < path.size() - 1; i++) {
        string& current_field = path.at(i);
        bool success_join = add_join_clause(first_table, current_field, last_table_name, sql);
        if (!success_join || first_table->neightbor_tables.count(current_field) == 0)
            return false;
        first_table = first_table->neightbor_tables[current_field].lock();
    }

    return true;
}

bool Query::add_select_clause(stringstream& sql) {
    sql << "SELECT ";
    sql << relative_to->name << ".*";
    sql << " FROM " << relative_to->name << " ";
    return true;
}

bool Query::to_select_sql(stringstream& sql) {
    if (!add_select_clause(sql))
        return false;
    stringstream join_clauses;
    stringstream where_clause;

    for (FieldFilter& filter : filters) {
        string last_table;
        if (!add_join_clauses(filter.path, join_clauses, last_table) ||
            !add_where_clauses(filter, where_clause, last_table))
            return false;
    }
    sql << join_clauses.str() << where_clause.str() << ";";
    return true;
}
