#include <sqlite3.h>

#define DB_FILE_NAME "score.db"
#define DB_TABLE_NAME "ScoreBoard"

int connect_db(sqlite3 **db);
int close_db(sqlite3 *db);
int exec_db(sqlite3 *db, char *sql);
int print_last_items(sqlite3 *db, int count);
