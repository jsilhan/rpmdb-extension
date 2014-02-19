#include <sqlite3.h>
#include <string>
#include <functional>

using std::string;
using std::function;
using std::stringstream;
using std::vector;

struct column {
    string name;
    int type;
};

class MyDb {
private:
    const char * path = "test.db";
    sqlite3 *pdb;
    bool initialized;
    void iter_one_column(const string& query, int column,
        function<bool(string& value)>fnc);
    bool execute(const string& sql, const char *context);
    bool update_value(int type, const string& name, const string& column,
        bool override, function<void(stringstream&)> fnc);
    bool init_table();
    bool add_column(const string& column, bool text);
    bool insert_record(const string& name, int type);
public:
    bool init();
    MyDb(const string path, int init_flags);
    ~MyDb();
    bool get_field(int type, const string& name, const string& column,
        string& value);
    bool get_field(int type, const string& name, const string& column,
        int& value);
    bool set_field(int type, const string& name, const string& column,
        const string& value, bool override);
    bool set_field(int type, const string& name, const string& column,
        int value, bool override);
    // TODO
    vector<column> get_columns();
    void query(const string& sql_str, const vector<string>& variables,
        function<bool(sqlite3_stmt *statement)>);
    bool create_index(const string& column);
};

