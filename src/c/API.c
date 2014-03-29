UsdCustom:
void usd_app_set_int(UsdCustom, char *key, int value, int override);
void usd_app_set_str(UsdCustom, char *key, char *value, int override);
void usd_app_set_foreign_int(UsdCustom, char *namespace, char *key, int value, int override);
void usd_app_set_foreign_str(UsdCustom, char *namespace, char *key, char *value, int override);
bool usd_app_get_int(UsdCustom, char *key, int *value);
bool usd_app_get_foreign_int(UsdCustom, char *namespace, char *key, int *value);
bool usd_app_get_str(UsdCustom, char *key, char **value);
bool usd_app_get_foreign_str(UsdCustom, char *namespace, char *key, char **value);
void usd_app_remove_int(UsdCustom, char *key);
void usd_app_remove_str(UsdCustom, char *key);

UsdApp:
enum usd_app_type usd_app_type(UsdApp);
char *usd_app_name(UsdApp);
char *usd_app_version(UsdApp);
char *usd_app_description(UsdApp);
char *usd_app_group(UsdApp);
char *usd_app_group(UsdApp);
char *usd_app_arch(UsdApp);
char *usd_app_author(UsdApp);
char *usd_app_licence(UsdApp);
char *usd_app_uninstall_script(UsdApp);
int usd_app_state(UsdApp);
int usd_app_size(UsdApp);
int usd_app_version_cpm(UsdApp, char *version);
UsdFile *usd_app_files(UsdApp);
UsdDir *usd_app_dirs(UsdApp);
UsdAppRel *usd_app_provides(UsdApp);
UsdAppRel *usd_app_requires(UsdApp);
UsdAppRel *usd_app_conflicts(UsdApp);
Repo *usd_app_repos(UsdApp);
// plus all UsdCustom functions

UsdRpmPackage:
int usd_rpm_transaction_id(UsdRpmPackage);
int usd_rpm_install_id(UsdRpmPackage);
UsdAppRel *usd_rpm_obsoletes(UsdRpmPackage);
char *usd_rpm_release(UsdRpmPackage);
char *usd_rpm_url(UsdRpmPackage);
char *usd_rpm_summary(UsdRpmPackage);
char *usd_rpm_signature(UsdRpmPackage);
int usd_rpm_epoch(UsdRpmPackage);
int usd_rpm_evr_cmp(UsdRpmPackage, UsdRpmPackage);
// plus all UsdCustom functions
// plus all UsdApp functions
// plus wrappers to rpm functions that manipulate with headers

UsdAppRel:
int usd_apprel_type(UsdAppRel);
UsdApp usd_apprel_latest_same_arch(UsdAppRel);
UsdApp usd_apprel_latest(UsdAppRel);
enum comp usd_apprel_comparator(UsdAppRel);
int usd_apprel_flags(UsdAppRel);
char *usd_apprel_what(UsdAppRel);
char *usd_apprel_version(UsdAppRel);

UsdRepo:
int usd_repo_type(UsdRepo);
char *usd_repo_url(UsdRepo);
char *usd_repo_name(UsdRepo);
// plus all UsdCustom functions

UsdDirectory:
int usd_dir_type(UsdDir);
int usd_dir_attributes(UsdDir);
char *usd_dir_name(UsdDir);
UsdFile *usd_dir_files(UsdDir);
UsdApp *usd_dir_apps(UsdDir);
// plus all UsdCustom functions

UsdFile:
int usd_file_type(UsdFile);
int usd_file_attributes(UsdFile);
char *usd_file_name(UsdFile);
UsdDir *usd_file_dirs(UsdFile);
UsdApp *usd_file_apps(UsdFile);
// plus all UsdCustom functions

UsdDb:
UsdDb usd_db_init(char *namespace);
bool usd_db_close(UsdDb);
void usd_db_begin_transaction(UsdDb, ...);
void usd_db_end_transaction(UsdDb, ...);
void usd_db_discard_transaction(UsdDb, ...);
int usd_db_last_sync(UsdDb, enum usd_app_type);
bool usd_db_force_sync(UsdDb, enum usd_app_type);
UsdAppIter usd_db_apps(UsdDb, UsdCondition);
UsdFileIter usd_db_files(UsdDb, UsdCondition);
UsdDirIter usd_db_dirs(UsdDb, UsdCondition);
UsdRepoIter usd_db_repos(UsdDb, UsdCondition);

bool usd_iter_next(UsdAppIter, UsdApp*);
bool usd_iter_next(UsdFileIter, UsdFile*);
bool usd_iter_next(UsdDirIter, UsdDir*);
bool usd_iter_next(UsdRepoIter, UsdRepo*);

UsdCondition:
UsdCondition usd_match_int(enum usd_entity, int field, int key);
UsdCondition usd_match_str(enum usd_entity, int field, char *key);
UsdCondition usd_match_custom_int(enum usd_entity, char *field, int key);
UsdCondition usd_match_custom_str(enum usd_entity, char *field, char *key);
UsdCondition usd_match_foreign_int(char *namespace, enum usd_entity, char *field, int key);
UsdCondition usd_match_foreign_str(char *namespace, enum usd_entity, char *field, char *key);
UsdCondition usd_notmatch_int(enum usd_entity, int field, int key);
UsdCondition usd_notmatch_str(enum usd_entity, int field, char *key);
UsdCondition usd_notmatch_custom_int(enum usd_entity, char *field, int key);
UsdCondition usd_notmatch_custom_str(enum usd_entity, char *field, char *key);
UsdCondition usd_notmatch_foreign_int(char *namespace, enum usd_entity, char *field, int key);
UsdCondition usd_notmatch_foreign_str(char *namespace, enum usd_entity, char *field, char *key);
UsdCondition usd_cond_and(UsdCondition, UsdCondition);
UsdCondition usd_cond_or(UsdCondition, UsdCondition);
