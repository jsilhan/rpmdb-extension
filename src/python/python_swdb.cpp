#include <string>
#include "../cpp/swdb.hpp"
// #include "../cpp/swdb.hpp"
// #include "../cpp/swdb.hpp"

#include <boost/python.hpp>
using namespace boost::python;

BOOST_PYTHON_MODULE(swdb)
{
    class_<Swdb>("Swdb", init<std::string, int>())
        .def("init", &Swdb::init)
        // .def_readwrite("name", & SomeClass::name)
        // .add_property("number", &SomeClass::getNumber, &SomeClass::setNumber)
    ;
}
