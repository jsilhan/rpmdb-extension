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
#include "db.hpp"
#include "query.hpp"
#include "record.hpp"
#include "swdb.hpp"


using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::make_shared;
using std::move;
using std::vector;

/**
 * Main handler to Swdb
 * path - from where database should be opened or where should be
 *        new database created
 * type - Search or insert records only with the same package type. If zero
 *        is set all queries will run against all kinds of packages (can be
 *        explicitly filtered in query afterwards).
 */
Swdb::Swdb(string path, int type) :
    default_pkg_type((pkg_type) type) {
    db = make_shared<Db>(path);
    string pkg("pkgs");
    string repo("repos");
    string pkg_change("pkg_changes");
    string group("groups");
    string transaction("transactions");
    string actors("actors");
    string trans_output("trans_outputs");

    sTable t_pkg(new Table(pkg));
    sTable t_repo(new Table(repo, true, true));
    sTable t_pkg_change(new Table(pkg_change, true));
    sTable t_group(new Table(group, false, true));
    sTable t_transaction(new Table(transaction, true));
    sTable t_actors(new Table(actors, true));
    sTable t_trans_output(new Table(trans_output, true));

    db->tables[pkg] = t_pkg;
    db->tables[repo] = t_repo;
    db->tables[pkg_change] = t_pkg_change;
    db->tables[group] = t_group;
    db->tables[transaction] = t_transaction;
    db->tables[trans_output] = t_trans_output;
    
    string pkg_name = "name";
    t_pkg->add_field(pkg_name, STRING);
    t_pkg->add_field("type", INT);
    create_index(string("pkgs"), pkg_name, true);

    t_repo->add_field("name", STRING);
    t_repo->add_field("last_synced", INT);
    t_repo->add_field("is_expired", INT);

    t_pkg_change->add_field("reason", INT);
    t_pkg_change->add_field("done", INT);
    t_pkg_change->add_field("state", INT);
    db->add_many_to_one(t_pkg_change, t_pkg, "pkg1", "pkg1_change");
    db->add_many_to_one(t_pkg_change, t_pkg, "pkg2", "pkg2_change");
    db->add_many_to_one(t_pkg_change, t_group);

    t_group->add_field("name", STRING);
    t_group->add_field("is_installed", INT);

    t_transaction->add_field("beg_timestamp", INT);
    t_transaction->add_field("end_timestamp", INT);
    t_transaction->add_field("rpmdb_version", INT);
    t_transaction->add_field("cmdline", INT);
    t_transaction->add_field("loginuid", INT);
    t_transaction->add_field("return_code", INT);
    db->add_many_to_one(t_actors, t_transaction);
    db->add_many_to_one(t_actors, t_pkg);

    t_trans_output->add_field("msg", STRING);
    t_trans_output->add_field("type", INT);
    db->add_many_to_one(t_trans_output, t_transaction);
}

/**
 * Method that could be optionally called to ensure that database
 * was initialized. This is called implicitly before any database
 * operation.
 */
bool Swdb::init() {
    return db->init();
}

/**
 * Returns new Record object, wrapped by unique pointer, that
 * could be inserted to table named 'table_name' as a row after
 * setting fields and firing record's save method.
 */
uRecord Swdb::urecord(string table_name) {
    return uRecord(new Record(db, table_name));
}

/**
 * Returns new Record object that could be inserted to table named 'table_name'
 * as a row after setting fields and firing record's save method.
 */
Record Swdb::record(string table_name) {
    return Record(db, table_name);
}

/**
 * Returns new Query object, wrapped by unique pointer. Its results will be
 * from 'table_name' table only.
 */
uQuery Swdb::uquery(string table_name) {
    return uQuery(new Query(db, table_name));
}

/**
 * Returns new Query object. Its results will be from 'table_name' table only.
 */
Query Swdb::query(string table_name) {
    return Query(db, table_name);
}

/**
 * creates index on frequently searched field that could have been created
 * on demand.
 * field name - column that should be indexed
 * table name - table where the column is
 * unique - create unique (true) / normal (false) index
 */
bool Swdb::create_index(const string& table_name, const string& field_name, bool unique) {
    stringstream sql;
    sql << "CREATE ";
    if (unique)
        sql << "UNIQUE ";
    sql << "INDEX IF NOT EXISTS `idx_" << field_name << "` ON `";
    sql << table_name << "`(`" << field_name << "`);";
    if (db->execute(sql.str(), "create index on record"))
        return true;
    return false;
}
