#ifndef QUERY_H
#define QUERY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <memory>
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
    sDb db;
    sTable relative_to;
    vector<uFieldFilter> filters;
    bool add_select_clause(stringstream& sql);
    bool add_join_clauses(vector<ustring>& path, stringstream& sql, string& last_field);
    bool add_join_clause(sTable& t, string& table_alias, string& last_table_name, stringstream& sql);
    bool add_where_clauses(uFieldFilter& filter, stringstream& sql, string& last_field);
    // string& get_last_table_name(uFieldFilter& filter);
    void get_last_table_name(uFieldFilter& filter, string& table_name);
    Query(sDb db, sTable& t) : db(db), relative_to(t) {};
    Query(sDb db, string& tn) : db(db), relative_to(db->tables[tn]) {};
    bool to_select_sql(stringstream& sql);
    void filter(const string& path, string value, int value_flags);

    class iterator {
    private:
        sqlite3_stmt *statement;
        int res;
        Table& table;
        sDb db;
    public:
        typedef iterator self_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        iterator(Query& q, int r) : table(*q.relative_to), db(q.db), statement(nullptr), res(r) {}
        iterator(Query& q) : table(*q.relative_to), db(q.db), statement(nullptr) {
            stringstream sql;
            if (!q.to_select_sql(sql))
                res = SQLITE_DONE;
            int rc = db->prepare_select(sql, &statement);
            if (rc != SQLITE_OK)
                res = SQLITE_DONE;

            res = sqlite3_step(statement);
            if (res == SQLITE_ROW)
                table.update_fields_metadata(statement);
        }
        ~iterator() {
            // if (statement != nullptr)
            //     sqlite3_finalize(statement);
        }
        self_type operator++() {
            res = sqlite3_step(statement);
            return *this;
        }
        self_type operator++(int junk) {
            res = sqlite3_step(statement);
            return *this;
        }
        Record operator*() {
            return Record(db, table, statement);
        }
        // Record operator->() { return Record(db, table, statement); }
        bool operator==(const self_type& rhs) {
            return res == rhs.res;
        }
        bool operator!=(const self_type& rhs) {
            return !(*this == rhs);
        }
    };
    iterator begin() {
        return iterator(*this);
    }
    iterator end() {
        return iterator(*this, SQLITE_DONE);
    }
};

typedef unique_ptr<Query> uQuery;

#endif
