#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <assert.h>
#include <string.h>
#include "table.hpp"
#include "query.hpp"

using std::cout;
using std::cerr;
using std::function;
using std::ofstream;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::move;
using std::vector;

typedef unique_ptr<stringstream> ustringstream;

void split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

void append_joined(stringstream target, const vector<ustringstream>& v, const string& token) {
  for (auto i = v.begin(); i != v.end(); i++){
    if (i != v.begin())
        target << token;
    target << **i;
  }
}
