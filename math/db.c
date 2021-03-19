#include <stdio.h>
#include <stdlib.h>

#include "db.h"

int connect_db(sqlite3 **db)
{
	int rc = 0;
	char *sql = NULL;

	if (*db != NULL) {
		close_db(*db);
	}

	rc = sqlite3_open(DB_FILE_NAME, &(*db));

	if (rc != SQLITE_OK) {
		printf("[ERR] Failed to open a DB: %s\n", sqlite3_errmsg(*db));
		return -1;
	}

	return 0;
}

int close_db(sqlite3 *db)
{
	int rc = 0;

	if (db == NULL) {
		printf("[ERR] DB is NULL\n");
		return -1;
	}

	rc = sqlite3_close(db);
	if (rc != SQLITE_OK) {
		printf("[ERR] Failed to close a DB: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	db = NULL;

	return 0;
}


int exec_db(sqlite3 *db, char *sql)
{
	char *err_msg = NULL;
	int rc = 0;

	if (db == NULL) {
		printf("[ERR] DB is NULL\n");
		return -1;
	}

	if (sql == NULL) {
		printf("[ERR] sql is NULL\n");
		return -1;
	}

	rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
	if (rc != SQLITE_OK) {
		printf("[ERR] Failed to execute SQL: %s\n", sqlite3_errmsg(db));
	}

	free(err_msg);

	return 0;
}

int print_last_items(sqlite3 *db, int count)
{
	char sql[512] = { 0, };

	int rc = 0;

	snprintf(sql, sizeof(sql), "SELECT * FROM %s\
			ORDER BY ROWID LIMIT %d;",
			DB_TABLE_NAME, count);

	rc = exec_db(db, sql);
	if (rc == -1) {
		printf("[ERR] Failed to execute SQL: [%s]\n", sql);
		return -1;
	}

	return 0;
}
