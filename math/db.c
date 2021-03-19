#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

static int _print_cb(void *NU, int argc, char **argv, char **azColName)
{
	int i = 0;
	for (i = 0; i < argc; i++) {
		//printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : NULL);
		if (strncmp(azColName[i], "name", strlen(azColName[i])) == 0) {
			continue;
		}

		if (strncmp(azColName[i], "date", strlen(azColName[i])) == 0) {
			printf("[%s]", argv[i]);
		}

		if (strncmp(azColName[i], "score", strlen(azColName[i])) == 0) {
			int score = atoi(argv[i]) / 5;
			int j = 0;

			printf("[");
			for (j = 0; j < score; j++) {
				printf("+");
			}

			for (j = 0; j < 20 - score; j++) {
				printf("-");
			}
			printf("]: %3d\n", score * 5);
		}
	}

	return 0;
}

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
	char *err_msg = NULL;

	int rc = 0;

	printf(" ============================================ \n");
	printf(" \t\tHISTORY\n");
	printf(" ============================================ \n");

	snprintf(sql, sizeof(sql), "SELECT * FROM %s\
			ORDER BY ROWID LIMIT %d;",
			DB_TABLE_NAME, count);

	rc = sqlite3_exec(db, sql, _print_cb, NULL, &err_msg);
	if (rc == -1) {
		printf("[ERR] Failed to execute SQL: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	return 0;
}
