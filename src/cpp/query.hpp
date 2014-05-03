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
    Db& db;
    sTable relative_to;
    vector<uFieldFilter> filters;
    bool add_select_clause(stringstream& sql);
    bool add_join_clauses(vector<ustring>& path, stringstream& sql, string& last_field);
    bool add_join_clause(sTable& t, string& table_alias, string& last_table_name, stringstream& sql);
    bool add_where_clauses(uFieldFilter& filter, stringstream& sql, string& last_field);
    // string& get_last_table_name(uFieldFilter& filter);
    void get_last_table_name(uFieldFilter& filter, string& table_name);
    Query(Db& db, sTable& t) : db(db), relative_to(t) {};
    Query(Db& db, string& tn) : db(db), relative_to(db.tables[tn]) {};
    bool to_select_sql(stringstream& sql);
    void filter(const string& path, string value, int value_flags);
    int iter(function<void(Record&)> fnc) {
        db.init();
        int count = 0;
        sqlite3_stmt* statement;
        stringstream ss;
        if (!to_select_sql(ss))
            return -1;
        cout << "### query str = " << ss.str() << endl;
        int rc = sqlite3_prepare(db.sql_db, ss.str().c_str(), -1, &statement, NULL);
        if (rc != SQLITE_OK)
            return -1;
        cout << "### iter, rc = " << rc << endl;

        int s = sqlite3_step(statement);
        relative_to->update_fields_metadata(statement);
        while (1) {
            cout << "### in select while, step = " << s << endl;
            if (s == SQLITE_ROW) {
                Record r(db, *relative_to, statement);
                fnc(r);
                count++;
            } else if (s == SQLITE_DONE) {
                break;
            } else {
                cerr << "### failed step" << endl;
                return -1;
            }
            s = sqlite3_step(statement);
        }
        return count;
    }

    typedef int size_type;
 
    class iterator {
    public:
        typedef iterator self_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        iterator(Table& t, Db& db, string sql) : statement(nullptr),
            table(t), db(db), sql(sql), res(-1) {}
        iterator(Table& t, Db& db, string sql, int s) : statement(nullptr),
            table(t), db(db), sql(sql), res(s) {}
        ~iterator() {
            sqlite3_finalize(statement);
        }
        self_type operator++() {
            init();
            res = sqlite3_step(statement);
            cout << "### step, res = " << res << endl;
            return *this;
        }
        self_type operator++(int junk) {
            init();
            res = sqlite3_step(statement);
            cout << "### junk step, res = " << res << endl;
            return *this;
        }
        uRecord operator*() {
            init();
            uRecord record(nullptr);
            cout << "### deref ok = " << (res == SQLITE_ROW) << endl;
            if (res == SQLITE_ROW) {
                cout << "### step" << endl;
                string id = (char *) sqlite3_column_text(statement, 0);
                record = uRecord(new Record(db, table, stoi(id)));
            }
            return record;
        }
        // Record operator->() { return statement; }
        bool operator==(const self_type& rhs) {
            return res == rhs.res || statement == rhs.statement;
        }
        bool operator!=(const self_type& rhs) {
            return !(*this == rhs);
        }
    private:
        bool init() {
            if (statement == nullptr)
                return db.prepare_select(sql, &statement);
            return true;
        }
        sqlite3_stmt *statement;
        string sql;
        int res;
        Table& table;
        Db& db;
    };
    iterator begin() {
        stringstream ss;
        if (to_select_sql(ss))
            return iterator(*relative_to, db, ss.str());
        return end();
    }
    iterator end() {
        return iterator(*relative_to, db, "", SQLITE_DONE);
    }
};

typedef unique_ptr<Query> uQuery;

#endif
