#include <sqlite3.h>

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

SwdbDb swdb_create();
SwdbDb swdb_create_path(char *path, int init_flags);
int get_field(SwdbDb, pkg_type type, const char *name, const char *column,
    char *value);
int get_field(SwdbDb, pkg_type type, const char *name, const char *column, int *value);
int set_field(SwdbDb, pkg_type type, const char *name, const char *column,
    const char *value, int override);
int set_field(SwdbDb, pkg_type type, const char *name, const char *column,
    int value, int override);
column *get_columns(SwdbDb);
void query(SwdbDb, const char *sql_str, const char **variables,
    sqlite3_stmt*);
int create_index(SwdbDb, const char *column);
