#ifndef SWDB_H
#define SWDB_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "table.hpp"
#include "connections.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;


class Swdb {
public:
    unordered_map<string,uTable> tables;
    Connections connections;
    // parameters
    // vector<Package> actors;
    string path;
    pkg_type default_pkg_type;
};

#endif
