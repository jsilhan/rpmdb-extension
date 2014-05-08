#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
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
using std::move;
using std::vector;

Swdb::Swdb(string path, int type) :
    default_pkg_type((pkg_type) type) {
    db = make_shared<Db>(path);
    // TODO actor params
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
    
    t_pkg->add_field("name", STRING);
    t_pkg->add_field("type", INT);

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

bool Swdb::init() {
    return db->init();
}

uRecord Swdb::urecord(string table_name) {
    return uRecord(new Record(db, table_name));
}

Record Swdb::record(string table_name) {
    return Record(db, table_name);
}

uQuery Swdb::uquery(string table_name) {
    return uQuery(new Query(db, table_name));
}

Query Swdb::query(string table_name) {
    return Query(db, table_name);
}
