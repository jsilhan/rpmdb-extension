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
        return relative_to->name;
    int before_last = filter->path.size() - 2;
    return *(filter->path.at(before_last));
}

void Query::add_where_clauses(uFieldFilter& filter, stringstream& sql) {
    string& table_name = get_last_table_name(filter);
    sql << "WHERE ";

    for (auto i = filters.begin(); i != filters.end(); i++) {
        if (i != filters.begin())
            sql << ", ";
        vector<ustring>& path = (*i)->path;
        sql << table_name << "." << *(path.at(path.size() - 1));
        sql << " = '" << (*i)->value << "'";
        // TODO consider flags
    }
}

bool Query::add_join_clause(sTable& t, string& table_alias, stringstream& sql) {
    if (t->neightbor_tables.count(table_alias) == 0)
        return false;

    bool ret = true;
    sTable other_st = t->neightbor_tables[table_alias].lock();
    Table other_t = *other_st;
    sql << "JOIN " << other_t.name << " ON ";

    if (t->fields_from_db.count(table_alias) > 0) { // many to one
        sql << t->name << "." << table_alias << " = ";
        sql << other_t.name << "._id ";
    } else if (t->fields_from_db.count(table_alias) == 0) { // path not exists
        ret = false;
    } else { // one to many
        string self_name;
        if (!other_t.table_ref_name(t, self_name))
            ret = false;
        else
            sql << t->name << "._id = " << other_t.name << "." << self_name;
    }

    other_st.reset();
    return ret;
}

bool Query::add_join_clauses(vector<ustring>& path, stringstream& sql) {
    sTable& first_table = relative_to;
    int i = 0;
    for (; i < path.size() - 1; i++) {
        string& field = *path.at(i);
        cout << "### add_join_clause for field " << field << endl;
        bool success_join = add_join_clause(first_table, field, sql);
        if (i != 0) // release pointer from the last round
            first_table.reset();
        if (!success_join || first_table->neightbor_tables.count(field) == 0)
            return false;
        first_table = first_table->neightbor_tables[field].lock();
        // add_join_clause(first_table, field, sql);
    }
    if (i != 0)
        first_table.reset();

    return true;
}

void Query::add_select_clause(stringstream& sql) {
    sql << "SELECT ";
    map<string,field_flags>& fields = relative_to->fields_from_db;

    for (auto i = fields.begin(); i != fields.end(); i++) {
        if (i != fields.begin())
            sql << " ";
        sql << relative_to->name << "." << i->first;
        // TODO consider flags
    }

    sql << " FROM " << relative_to->name << " ";
}

bool Query::to_select_sql(stringstream& sql) {
    add_select_clause(sql);
    for (uFieldFilter& filter : filters) {
        if (!add_join_clauses(filter->path, sql))
            return false;
    }
    for (uFieldFilter& filter : filters) {
        add_where_clauses(filter, sql);
    }
    sql << ";";
    return true;
}
