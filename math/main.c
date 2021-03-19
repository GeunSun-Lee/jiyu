#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "db.h"

#define ESC 27
#define COUNT 20
#define RESULT_TRUE 1
#define RESULT_FALSE 0
#define USE_DB 1

typedef enum OP_TYPE {
	OP_TYPE_PLUS = 0,
	OP_TYPE_MINUS,
	OP_TYPE_MULTIPLE,
	OP_TYPE_MAX,
} OP_TYPE;

int print_result(int result)
{
	if (result) {
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
	int result = 0;

	switch (op) {
	case OP_TYPE_PLUS:
		result = ((x + y) == val) ? RESULT_TRUE : RESULT_FALSE;
		print_result(result);
		break;
	case OP_TYPE_MINUS:
		result = ((x - y) == val) ? RESULT_TRUE : RESULT_FALSE;
		print_result(result);
		break;
	case OP_TYPE_MULTIPLE:
		result = ((x * y) == val) ? RESULT_TRUE : RESULT_FALSE;
		print_result(result);
		break;
	defult:
		printf("[ERR] type error: %d\n", op);
		return -1;
	}

	return result;
}

#if USE_DB
int save_db(int score)
{
	char sql[512] = { 0, };

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	int rc = 0;

	//sqlite> insert into ScoreBoard(name, year, month, day, hour, min, score) values("jiyu", 2021, 3, 19, 11, 56, 80);


	snprintf(sql, sizeof(sql), "CREATE TABLE IF NOT EXISTS  %s\
			(name TEXT,\
			 year INTEGER,\
			 month INTEGER,\
			 day INTEGER,\
			 hour INTEGER,\
			 min INTEGER,\
			 score INTEGER);\
			INSERT INTO %s(name, year, month, day, hour, min, score) VALUES('JIYU', %d, %02d, %02d, %02d, %02d, %d);", 
			DB_TABLE_NAME, DB_TABLE_NAME, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, score * 5);

    rc = exec_db(sql);
    if (rc == -1) {
        printf("[ERR] Failed to initialize a DB\n");
        return -1;
    }
}
#elif
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
	printf(" JIYU's\n");
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
			if (ret)
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
			if (ret)
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
			if (ret) 
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
	connect_db();
	save_db(score);
	close_db();
#elif
	save_file(score);
#endif


	return 0;
}
