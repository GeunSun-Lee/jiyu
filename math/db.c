#include <stdio.h>
#include <stdlib.h>

#include "db.h"

sqlite3 *db = NULL;

int connect_db(void)
{
	int rc = 0;
    char* sql = NULL;

	if (db != NULL) {
		close_db();
	}

	rc = sqlite3_open(DB_FILE_NAME, &db);

	if (rc != SQLITE_OK) {
		printf("[ERR] Failed to open a DB: %s\n", sqlite3_errmsg(db));
		return -1;
	}

    return 0;
}

int close_db(void)
{
	int rc = 0;
	
	rc = sqlite3_close(db);
	if (rc != SQLITE_OK) {
		printf("[ERR] Failed to close a DB: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	db = NULL;

    return 0;
}


int exec_db(char* sql)
{
	char* err_msg = NULL;
	int rc = 0;

	if (sql == NULL) {
		printf("[ERR] sql is NULL\n");
		return -1;
	}

	if (db == NULL) {
		printf("[ERR] DB is NULL\n");
		return -1;
	}

	rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
	if (rc != SQLITE_OK) {
		printf("[ERR] Failed to execute SQL: %s\n", sqlite3_errmsg(db));
	}
	
	free(err_msg);

	return 0;
}
