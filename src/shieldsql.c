#include <stdio.h>
#include "./../include/shieldsql.h"

int main()
{
	MYSQL* db = 0;
	MYSQL_RES* res = 0;

	printf("%p\n", db);
	connectDB(&db, "localhost", "root", "jjssm", "shield");
	printf("%p\n", db);
	printf("number: %d\n", getQueryDataNum(&db, "select * from userlist", &res));

	return 0;
}

int connectDB(MYSQL** db, char* serverip, char* user, char* pass, char* databaseName)
{
	if( !(*db = mysql_init( (MYSQL*)NULL )) ) {
		perror("db init");
		return FALSE;
	}

	if( !mysql_real_connect(*db, serverip, user, pass, NULL, 3306, NULL, 0) ) {
		perror("db connect");
		return FALSE;
	}

	if(mysql_select_db(*db, databaseName) != 0) {
		mysql_close(*db);
		perror("use db fail");
		return FALSE;
	}

	return TRUE;
}

int sendQuery(MYSQL** db, char* query)
{
	if(mysql_query(*db, query)) {
		perror("notting query");
		return FALSE;
	}

	return TRUE;
}

int getQueryDataNum(MYSQL** db, char* query, MYSQL_RES** res)
{
	if( sendQuery(db, query) == FALSE ) {
		return 0;
	}

	*res = mysql_store_result(*db);

	return mysql_num_rows(*res);
}
