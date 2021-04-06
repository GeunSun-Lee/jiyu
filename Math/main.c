#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include "db.h"

int print_result(int result)
{
	if (result == RESULT_TRUE) {
		printf("%c[1;32m", ESC);
		printf("  [ O ] Correct");
		printf("%c[0m\n", ESC);
		return 0;
	} else {
		printf("%c[1;31m", ESC);
		printf("  [ X ] Incorrect");
		printf("%c[0m\n", ESC);
		return -1;
	}
}

int check_result(int op, int x, int y, int val)
{
	int answer = 0;

	switch (op) {
	case OP_TYPE_PLUS:
		answer = x + y;
		break;
	case OP_TYPE_MINUS:
		answer = x - y;
		break;
	case OP_TYPE_MULTIPLE:
		answer = x * y;
		break;
	defult:
		printf("[ERR] type error: %d\n", op);
		return -1;
	}

	return print_result((answer == val) ? RESULT_TRUE : RESULT_FALSE);
}

#if USE_DB
int save_db(sqlite3 *db, int score)
{
	char sql[512] = { 0, };
	char date[128] = { 0, };

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	int rc = 0;

	if (db == NULL) {
		printf("[ERR] DB is NULL\n");
		return -1;
	}

	snprintf(date, sizeof(date), "%d.%02d.%02d %02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);

	snprintf(sql, sizeof(sql), "CREATE TABLE IF NOT EXISTS  %s\
			(name TEXT,\
			 date TEXT,\
			 score INTEGER);\
			INSERT INTO %s(name, date, score)\
			VALUES('%s', '%s', %d);",
			DB_TABLE_NAME, DB_TABLE_NAME, USER_NAME, date, score * 5);

    rc = exec_db(db, sql);
    if (rc == -1) {
        printf("[ERR] Failed to initialize a DB\n");
        return -1;
    }

    return 0;
}

#else
int save_file(int score)
{
	FILE *fp = NULL;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	fp = fopen("score.txt", "a+");
	if (fp == NULL) {
		printf("[ERR] Failed to open a file\n");
		return -1;
	}

	fprintf(fp, "%d.%02d.%02d %02d:%02d\tScore: %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, score * 5);
	fclose(fp);

	return 0;
}
#endif

int main() 
{
	int i = 0, x = 0, y = 0;
	int val = 0, score = 0;
	int ret = 0, tmp = 0;
	int op = OP_TYPE_MAX;

	srand((unsigned int) time(NULL));

	printf("=======================================\n");
	printf(" %s's\n", USER_NAME);
	printf("\tThe First Math Program\n");
	printf("=======================================\n");

	printf("Please input right answer.\n");

	while (i < COUNT) {
		op = rand() % OP_TYPE_MAX;

		switch (op) {
		case OP_TYPE_PLUS:
			x = rand() % 100;
			y = rand() % 100;

			printf(" (%d) %d + %d = ", i + 1, x, y);
			scanf("%d", &val);

			ret = check_result(op, x, y, val);
			if (ret == RESULT_TRUE)
				score++;
			break;
		case OP_TYPE_MINUS:
			x = rand() % 100;
			y = rand() % 100;

			if (x < y) { 
				tmp = x;
				x = y;
				y = tmp;
			}

			printf(" (%d) %d - %d = ", i + 1, x, y);
			scanf("%d", &val);

			ret = check_result(op, x, y, val);
			if (ret == RESULT_TRUE)
				score++;
			break;
		case OP_TYPE_MULTIPLE:
			x = rand() % 10;
			y = rand() % 10;

			if (x == 0) x = 1;
			if (y == 0) y = 1;

			printf(" (%d) %d X %d = ", i + 1, x, y);
			scanf("%d", &val);

			ret = check_result(op, x, y, val);
			if (ret == RESULT_TRUE)
				score++;
			break;
		default:
			printf("op type: %d\n", op);
		}

		i++;
		printf("\n");
	}

	printf(" ==================================\n");
	printf(" \tTotal Score: [ %c[1;31m %d %c[0m ]\n", ESC, score * 5, ESC);
	printf(" ==================================\n");


#if USE_DB
	sqlite3 *db = NULL;

	ret = connect_db(&db);
	if (ret != 0) {
		printf("[ERR] Failed to connect DB\n");
		return -1;
	}

	ret = save_db(db, score);
	if (ret != 0) {
		printf("[ERR] Failed to save data\n");
	}

	ret = print_last_items(db, 5);
	if (ret != 0) {
		printf("[ERR] Failed to print last items\n");
	}

	ret = close_db(db);
	if (ret != 0) {
		printf("[ERR] Failed to close DB\n");
		return -1;
	}
#else
	save_file(score);
#endif

	return 0;
}
