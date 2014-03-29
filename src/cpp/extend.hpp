#include <sqlite3.h>
#include <string>
#include <functional>

using std::string;
using std::function;
using std::stringstream;
using std::vector;

enum column_type {
    TEXT = 0,
    INT
};

struct column {
    string name;
    column_type type;
};

enum pkg_type {
    RPM_PKG = 0,
};

class Swdb {
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
    Swdb(const string path, int init_flags);
    ~Swdb();
    bool get_field(pkg_type type, const string& name, const string& column,
        string& value);
    bool get_field(pkg_type type, const string& name, const string& column,
        int& value);
    bool set_field(pkg_type type, const string& name, const string& column,
        const string& value, bool override);
    bool set_field(pkg_type type, const string& name, const string& column,
        int value, bool override);
    // TODO
    vector<column> get_columns();
    bool create_index(const string& column);
};
