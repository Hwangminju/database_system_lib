#include <my_global.h>
#include <mysql.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

void welcome();
void menu();
void book_search();
void student_info();

void finish_with_error(MYSQL *con)
{
	fprintf(stderr, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);
}

int main(void)
{
	welcome();
}

void welcome()
{
	printf("Welcome to Central library of Dongguk Univ.\n\n");
	menu();
}

void menu()
{
	int no = 0;

	printf("You can select menu.\n\n");
	printf("=================================\n");
	printf("1. Book search\n2. Lending of student\n\n");
	printf("Select menu: ");
	scanf_s("%d", &no);
	printf("\n");

	switch (no)
	{
	case 1:
		book_search();
		break;
	case 2:
		student_info();
	default:
		welcome();
	}
}

void book_search()
{
	int sel;
	printf("================================\n");
	printf("1) Book title\n2) Author\n3) Publication\n4) Call_no\n\n");
	printf("Select: ");
	scanf_s("%d", &sel);
	printf("\n");

	MYSQL *con = mysql_init(NULL);
	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "localhost", "root", "dgu1234",
		"dgulib", 0, NULL, 0) == NULL)
	{
		fprintf_s(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	char name[20];
	char buff[255];

	switch (sel)
	{
	case 1:
		printf("Book Title: ");
		scanf_s("%s", name, sizeof(name));

		sprintf_s(buff, sizeof(buff), "select * from book where title = '%s'", name);
		// 입력한 제목을 가진 책의 정보를 출력.
		break;
	case 2:
		printf("Author: ");
		scanf_s("%s", name, sizeof(name));

		sprintf_s(buff, sizeof(buff), "select * from book where author = '%s'", name);
		// 입력한 작가가 집필한 책의 정보를 출력.
		break;
	case 3:
		printf("Publication: ");
		scanf_s("%s", name, sizeof(name));

		sprintf_s(buff, sizeof(buff), "select * from book where publication = '%s'", name);
		// 입력한 출판사에서 출판한 책의 정보를 출력.
		break;
	case 4:
		printf("Call_no: ");
		scanf_s("%s", name, sizeof(name));

		sprintf_s(buff, sizeof(buff), "select * from book where call_no = '%s'", name);
		// 입력한 call number를 가진 책의 정보를 출력.
		break;
	default:
		exit(1);
	}

	mysql_query(con, buff);

	MYSQL_RES *result = mysql_store_result(con);

	if (result == NULL)
	{
		finish_with_error(con);
	}

	int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;
	MYSQL_FIELD *field;

	while ((row = mysql_fetch_row(result)))
	{
		for (int i = 0; i < num_fields; i++)
		{
			if (i == 0)
			{
				while (field = mysql_fetch_field(result))
				{
					printf("%-15s", field->name);
				}

				printf("\n");
			}
			printf("%-15s", row[i] ? row[i] : "NULL");
		}
	}

	printf("\n");

	mysql_free_result(result);

	mysql_close(con);
	menu();
}

void student_info()
{
	MYSQL *con = mysql_init(NULL);

	if (con == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "localhost", "root", "dgu1234",
		"dgulib", 0, NULL, 0) == NULL)
	{
		fprintf_s(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}

	int s_no = 0;
	printf("Student No.: ");
	scanf_s("%d", &s_no);

	char buff[255];
	sprintf_s(buff, sizeof(buff), "select title, book.call_no, due_date from book, rent where '%d' = rent.S_no and book.call_no = rent.call_no", s_no);
	// 입력한 학번에 해당하는 학생이 빌린 책의 제목과 call number, 대출 due date를 출력.
	mysql_query(con, buff);

	MYSQL_RES *result = mysql_store_result(con);

	if (result == NULL)
	{
		finish_with_error(con);
	}

	int num_fields = mysql_num_fields(result);

	MYSQL_ROW row;
	MYSQL_FIELD *field;

	while ((row = mysql_fetch_row(result)))
	{
		for (int i = 0; i < num_fields; i++)
		{
			if (i == 0)
			{
				while (field = mysql_fetch_field(result))
				{
					printf("%-15s", field->name);
				}

				printf("\n");
			}
			printf("%-15s", row[i] ? row[i] : "NULL");
		}
	}

	printf("\n");

	mysql_free_result(result);

	mysql_close(con);
	menu();
}
