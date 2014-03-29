#include <iostream>
 
using namespace std;
 
void say_hello(const char* name) {
    cout << "Hello " <<  name << "!\n";
}

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
using namespace boost::python;
 
BOOST_PYTHON_MODULE(swdb)
{
    class_<Swdb>("Swdb")
        .def_readonly("package", &Swdb::package)
        .def_readonly("transaction", &Swdb::transaction)
        .def_readonly("package_transformation", &Swdb::package_transformation)

    class_<Query>("Query")
        .def("new", &Query::new_query)
        // .def("order_by", &Query::order_by)
        .def("filter", raw_function(filter, 1))
        // .def("exclude", raw_function(exclude, 1))
        .def("run", &Query::run)
        // TODO __len__
        // TODO __getitem__
        // TODO __list__

    class_<PackageQuery, bases<Query>>("PackageQuery")
        .def("new", &PackageQuery::new_query)

    class_<TransactionQuery, bases<Query>>("TransactionQuery")
        .def("new", &TransactionQuery::new_query)

    class_<PackageTransformationQuery, bases<Query>>("PackageTransformationQuery")
        .def("new", &PackageTransformationQuery::new_query)

    class_<Record>("Record")
        .add_property("in_db", &Record::in_db)
        .add_property("changed", &Record::changed)
        .def("set", &Record::set)
        .def("get", &Record::get)
        .def("create_index", &Record::create_index)
        // TODO __getitem__
        // TODO __setitem__

    class_<PackageTransformation, bases<Record>>("PackageTransformation")
        .def("reserved_fields", &PackageTransformation::reserved_fields).
        .staticmethod("reserved_fields")

    class_<Package, bases<Record>>("Package")
        .def("reserved_fields", &Package::reserved_fields).
        .staticmethod("reserved_fields")
        .def("save", &Package::save).
        .def("clear", &Package::clear).

    class_<Transaction, bases<Record>>("Transaction")
        .def("reserved_fields", &Transaction::reserved_fields).
        .staticmethod("reserved_fields")
        .def("save", &Transaction::save)
        .def("add", &Transaction::add)
        .def("merge", &Transaction::merge)
        .def("end", &Transaction::end)
}

namespace PyQuery {
    object filter_or_exclude()

    str 

    object filter(Transaction self, dict kwargs) {
        list keys = py_dict.keys();
        for (int i = 0; i < len(keys); ++i) {
            extract<string> key(keys[i]);
            extract<string> val(kwargs[key]);
            for (Package& : self.packages) {

            }
        }
    }
}
