#include <sqlite3.h>

#define DB_FILE_NAME "score.db"
#define DB_TABLE_NAME "ScoreBoard"

int connect_db(void);
int close_db(void);
int exec_db(char *sql);
