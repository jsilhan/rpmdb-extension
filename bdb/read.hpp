#include <functional>
#include <string>
#include <db_cxx.h>
#include <vector>

using std::string;
using std::vector;
using std::function;

class BDB {
private:
    Db my_database;
public:
    BDB(string db_name);
    ~BDB();
    void iter_keys(function<bool(Dbt&, Dbt&)> fnc);
    void save_200_samples(string filename);
    int get_int_value(string& key);
    vector<string*>* get_200_samples();
};
