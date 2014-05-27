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

struct FieldFilter {
    vector<string> path;
    string value;
    int value_flags;
};

struct Query {
    enum comparator_flags {
        EQ = 0,
        NEQ = 1,
        GT = 2,
        GTE = 3,
        LTE = 4,
        LT = 5,
        ICASE = 1 << 8,
    };
    
    sDb db;
    sTable relative_to;
    vector<FieldFilter> filters;
    bool add_select_clause(stringstream& sql);
    bool add_join_clauses(vector<string>& path, stringstream& sql, string& last_field);
    bool add_join_clause(sTable& t, string& table_alias, string& last_table_name, stringstream& sql);
    bool add_where_clauses(FieldFilter& filter, stringstream& sql, string& last_field);
    void get_last_table_name(FieldFilter& filter, string& table_name);
    Query(sDb db, sTable& t) : db(db), relative_to(t) {};
    Query(sDb db, string& tn) : db(db), relative_to(db->tables.at(tn)) {};
    Query(sDb db, string&& tn) : db(db), relative_to(db->tables.at(tn)) {};
    bool to_select_sql(stringstream& sql);
    void filter(const string& path, string value, int value_flags);

    Record operator[](int idx) {
        auto it = this->begin();
        for (int i = 0; i < idx; ++i) {
            if (it == this->end())
                throw std::out_of_range("doesn't have that many rows");
            it++;    
        }
        return *it;
    }

    class iterator : public std::iterator<std::input_iterator_tag, Record> {
    private:
        sDb db;
        Table& table;
        sqlite3_stmt *statement;
        int res;
    public:
        typedef Record value_type;
        typedef Record& reference;
        typedef Record* pointer;
        typedef Record result_type;
        typedef iterator self_type;
        typedef std::input_iterator_tag iterator_category;
        typedef int difference_type;
        iterator(Query& q, int r) : db(q.db), table(*q.relative_to), statement(nullptr), res(r) {}
        iterator(Query& q) : db(q.db), table(*q.relative_to), statement(nullptr) {
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
            //     sqlite3_finalize(statement); // FIXME
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
