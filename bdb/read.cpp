#include <iostream>
#include <fstream>
#include <functional>
#include <chrono>
#include <vector>
#include <memory>
#include <string>
#include <db_cxx.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "read.hpp"       /* time */

using std::cout;
using std::function;
using std::ofstream;
using std::string;
using std::unique_ptr;
using std::move;
using std::vector;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::basic_string;

BDB::BDB(string db_name) : my_database(NULL, 0) {
    my_database.open(NULL, db_name.c_str(), NULL, DB_UNKNOWN, DB_RDONLY, 0);
}

BDB::~BDB() {
    my_database.close(0);
}

int BDB::get_int_value(string& str) {
    Dbt data;
    Dbt key(const_cast<char*>(str.data()), str.size());
    my_database.get(NULL, &key, &data, 0);
    return *(int *) data.get_data();
}

void BDB::iter_keys(function<bool(Dbt&, Dbt&)> fnc) {
    Dbc *cursorp;
    try {
        my_database.cursor(NULL, &cursorp, 0);
        Dbt key, data;
        while (cursorp->get(&key, &data, DB_NEXT) == 0) {
            if (fnc(key, data)) break;
        }
    } catch(DbException &e) {
        my_database.err(e.get_errno(), "Error!");
    } catch(std::exception &e) {
        my_database.errx("Error! %s", e.what());
    }
    if (cursorp != NULL)
        cursorp->close();
}

void BDB::save_200_samples(string filename) {
    ofstream f(filename);
    srand(time(NULL));
    int count = 0;
    iter_keys([&](Dbt& key, Dbt& data) {
        if (rand() % 10 == 1) {
            f.write((char *) key.get_data(), key.get_size() - 1);
            count++;
            f << "\n";
        }
        if (count >= 200) return true;
        return false;
    });
    cout << "total = " << count << "\n";
}

vector<string*>* BDB::get_200_samples() {
    vector<string*>* array = new vector<string*>();
    srand(time(NULL));
    int count = 0;
    iter_keys([&](Dbt& key, Dbt& data) {
        if (rand() % 10 == 1) {
            string *str = new string((char *) key.get_data(), key.get_size());
            // cout << *str << "\n";
            array->push_back(str);
            count++;
        }
        if (count >= 200) return true;
        return false;
    });
    return array;
}

