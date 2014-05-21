#include <string>
#include "../cpp/swdb.hpp"
#include "../cpp/record.hpp"
// #include "../cpp/swdb.hpp"

#include <boost/python.hpp>
using namespace boost::python;


void translator(std::out_of_range const& x) {
    PyErr_SetString(PyExc_IndexError, "Rows out of range");
}

BOOST_PYTHON_MODULE(swdb)
{
    bool (Record::*set_int)(const string&, int) = &Record::set;
    bool (Record::*set_str)(const string&, const string&) = &Record::set;
    bool (Record::*set_record)(const string&, vector<Record>) = &Record::set;
    bool (Record::*get_int)(const string&, int&) = &Record::get;
    bool (Record::*get_str)(const string&, string&) = &Record::get;

    class_<Record>("Record", no_init)
        .def("is_changed", &Record::is_changed)
        .def("is_in_db", &Record::is_in_db)
        .def("id", &Record::id)
        .def("set", set_int)
        .def("set", set_str)
        .def("set", set_record)
        .def("get", get_int)
        .def("get", get_str)
        .def("append", &Record::append)
    ;

    class_<Query>("Query", no_init)
        .def("filter", &Query::filter)
        // .def("__iter__", range(&Query::begin, &Query::end))
        // .def("__iter__", iterator<Query>())
    ;

    class_<Swdb>("Swdb", init<std::string, int>())
        .def("init", &Swdb::init)
        .def("record", &Swdb::record)
        .def("query", &Swdb::query)
        // .def_readwrite("name", & SomeClass::name)
        // .add_property("number", &SomeClass::getNumber, &SomeClass::setNumber)
    ;
}
