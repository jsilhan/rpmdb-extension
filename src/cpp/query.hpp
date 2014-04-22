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
#include <sqlite3.h>
#include "table.hpp"
#include "db.hpp"
#include "utils.hpp"
#include "record.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::stoi;
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
public:
    Db& db;
    sTable relative_to;
    vector<uFieldFilter> filters;
    sqlite3_stmt *statement;
    bool add_select_clause(stringstream& sql);
    bool add_join_clauses(vector<ustring>& path, stringstream& sql, string& last_field);
    bool add_join_clause(sTable& t, string& table_alias, string& last_table_name, stringstream& sql);
    bool add_where_clauses(uFieldFilter& filter, stringstream& sql, string& last_field);
    // string& get_last_table_name(uFieldFilter& filter);
    void get_last_table_name(uFieldFilter& filter, string& table_name);
    Query(Db& db, sTable& t) : db(db), relative_to(t), statement(nullptr) {};
    Query(Db& db, string& tn) : db(db), relative_to(db.tables[tn]), statement(nullptr) {};
    bool to_select_sql(stringstream& sql);
    void filter(const string& path, string value, int value_flags);

    typedef int size_type;
 
    class iterator {
    public:
        typedef iterator self_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        iterator(sqlite3_stmt *ptr, Table& t, Db& db) : statement(ptr), table(t), db(db) { }
        self_type operator++() {
                self_type i = *this; res = sqlite3_step(statement); return i; }
        self_type operator++(int junk) { res = sqlite3_step(statement); return *this; }
        uRecord operator*() {
            if (res == SQLITE_ROW) {
                string id = (char *) sqlite3_column_text(statement, 0);
                uRecord record(new Record(db, table, stoi(id)))
            }
                return *statement; }
        Record operator->() { return statement; }
        bool operator==(const self_type& rhs) { return statement == rhs.statement; }
        bool operator!=(const self_type& rhs) { return statement != rhs.statement; }
    private:
        sqlite3_stmt *statement;
        int res;
        bool begin;
        bool end;
        Table& table;
        Db& db;
    };
};

typedef unique_ptr<Query> uQuery;

#endif
