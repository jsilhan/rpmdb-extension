#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <string>
#include <memory>
#include <assert.h>
#include <string.h>

using std::cout;
using std::cerr;
using std::endl;
using std::function;
using std::ofstream;
using std::string;
using std::unique_ptr;
using std::move;
using std::vector;
using std::make_shared;


void split(const string& s, char delim, vector<string>& elems);

#endif
