#include <sstream>
#include "utils.hpp"

using std::stringstream;

void split(const string& s, char delim, vector<string>& elems) {
    stringstream ss(s);
    while (1) {
        string item;
        if (!getline(ss, item, delim))
            break;
        elems.push_back(move(item));
    }
}
