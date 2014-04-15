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

Swdb::Swdb(string path, pkg_type type) :
    db(path), default_pkg_type(type) {
    // TODO actor params
    string pkg("pkg");
    string repo("repo");
    string extension("extension");
    string pkg_change("pkg_change");
    string group("group");
    string transaction("transaction");
    string actors("actors");
    string trans_output("trans_output");

    sTable t_pkg(new Table(pkg));
    sTable t_repo(new Table(repo));
    sTable t_extension(new Table(extension));
    sTable t_pkg_change(new Table(pkg_change));
    sTable t_group(new Table(group));
    sTable t_transaction(new Table(transaction));
    sTable t_actors(new Table(actors));
    sTable t_trans_output(new Table(trans_output));
    
    t_pkg->add_field("name", STRING);
    t_pkg->add_field("type", INT);

    t_repo->add_field("name", STRING);
    t_repo->add_field("last_synced", INT);
    t_repo->add_field("is_expired", INT);

    db.add_many_to_one(t_extension, t_pkg);
    db.add_many_to_one(t_extension, t_repo);

    t_pkg_change->add_field("reason", INT);
    t_pkg_change->add_field("done", INT);
    t_pkg_change->add_field("state", INT);
    db.add_many_to_one(t_pkg_change, t_pkg, "pkg1", "pkg1_change");
    db.add_many_to_one(t_pkg_change, t_pkg, "pkg2", "pkg2_change");
    db.add_many_to_one(t_pkg_change, t_group);

    t_group->add_field("name", STRING);
    t_group->add_field("is_installed", INT);

    t_transaction->add_field("beg_timestamp", INT);
    t_transaction->add_field("end_timestamp", INT);
    t_transaction->add_field("rpmdb_version", INT);
    t_transaction->add_field("cmdline", INT);
    t_transaction->add_field("loginuid", INT);
    t_transaction->add_field("return_code", INT);
    db.add_many_to_one(t_actors, t_transaction);
    db.add_many_to_one(t_actors, t_pkg);

    t_trans_output->add_field("msg", STRING);
    t_trans_output->add_field("type", INT);
    db.add_many_to_one(t_trans_output, t_transaction);

    db.init();
}

uRecord Swdb::record(string table_name) {
    return uRecord(new Record(db, table_name));
}

uQuery Swdb::query(string table_name) {
    return uQuery(new Query(db, table_name));
}
