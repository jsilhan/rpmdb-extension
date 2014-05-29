#include <string>
#include "../cpp/swdb.hpp"
#include "../cpp/record.hpp"
#include "../cpp/query.hpp"

#include <boost/python.hpp>
using namespace boost::python;


void translator(std::out_of_range const& x) {
    PyErr_SetString(PyExc_IndexError, x.what());
}

void translator2(std::invalid_argument const& x) {
    PyErr_SetString(PyExc_AttributeError, x.what());
}

struct PyRecord : public Record {
    using Record::Record;
    object getitem(string key) {
        unsigned long i;
        object value;
        if (!from_table.get_cell_index(key, i)) {
            throw std::invalid_argument("Column does not exist");
        }
        cell& r = values_from_db[i];
        if (r.type == INT)
            value = long_(r.number);
        else if (r.type == STRING)
            value = str(r.text);
        return value;
    }

    PyRecord(Record&& record) : Record(record.db, record.from_table) {
        changed = move(record.changed);
        values_from_db = move(record.values_from_db);
        values_to_insert = move(record.values_to_insert);
        records_to_insert = move(record.records_to_insert);
        fields_to_append = move(record.fields_to_append);
    }
};

struct PyQuery : public Query {
    using Query::Query;
    PyRecord getitem(int n) {
        return PyRecord(this->operator[](n));
    }
};

struct PySwdb : public Swdb {
    using Swdb::Swdb;
    PyRecord *get_record(string table_name) {
        return new PyRecord(db, table_name);
    }
    PyQuery *get_query(string table_name) {
        return new PyQuery(db, table_name);
    }
};


BOOST_PYTHON_MODULE(swdb)
{
    register_exception_translator<std::out_of_range>(translator);
    register_exception_translator<std::invalid_argument>(translator2);

    bool (PyRecord::*set_int)(const string&, int) = &Record::set;
    bool (PyRecord::*set_str)(const string&, const string&) = &Record::set;
    bool (PyRecord::*set_record)(const string&, vector<Record>) = &Record::set;
    bool (PyRecord::*get_int)(const string&, int&) = &Record::get;
    bool (PyRecord::*get_str)(const string&, string&) = &Record::get;

    class_<PyRecord>("Record", no_init)
        .def("is_changed", &PyRecord::is_changed)
        .def("is_in_db", &PyRecord::is_in_db)
        .def("id", &PyRecord::id)
        .def("set", set_int)
        .def("set", set_str)
        .def("set", set_record)
        .def("get", get_int)
        .def("get", get_str)
        .def("append", &PyRecord::append)
        .def("save", &PyRecord::save)
        .def("__getitem__", &PyRecord::getitem)
    ;

    enum_<Query::comparator_flags>("flags")
        .value("EQ", Query::EQ)
        .value("NEQ", Query::NEQ)
        .value("GT", Query::GT)
        .value("GTE", Query::GTE)
        .value("LTE", Query::LTE)
        .value("LT", Query::LT)
        .value("ICASE", Query::ICASE)
    ;

    class_<PyQuery>("Query", no_init)
        .def("filter", &PyQuery::filter)
        .def("__getitem__", &PyQuery::getitem)
        // .def("__iter__", range(&Query::begin, &Query::end))
        // .def("__iter__", iterator<Query>())
    ;

    class_<PySwdb>("Swdb", init<std::string, int>())
        .def("init", &PySwdb::init)
        .def("record", &PySwdb::get_record, return_value_policy<manage_new_object>())
        .def("query", &PySwdb::get_query, return_value_policy<manage_new_object>())
    ;
}
