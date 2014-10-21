#include <stdio.h>
#include "./../include/shieldsql.h"
/*
int main()
{
	MYSQL* db = 0;
	MYSQL_RES* res = 0;
	UserInfo data;

	printf("%p\n", db);
	connectDB(&db, "localhost", "root", "jjssm", "shield");
	printf("%p\n", db);
	printf("number: %llu\n", getQueryDataNum(&db, "select * from userlist", &res));

	display(&res, &data);

	return 0;
}
*/

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

ullong getQueryDataNum(MYSQL** db, char* query, MYSQL_RES** res)
{
	if( sendQuery(db, query) == FALSE ) {
		return 0;
	}

	*res = mysql_store_result(*db);

	return (*res)->row_count;
}

int getQueryDataRow(MYSQL_RES** res, UserInfo* data)
{
	MYSQL_ROW row;

	if( (row = mysql_fetch_row(*res)) == 0 ) {
		return FALSE;
	}

	strcpy(data->id, row[1]);
	strcpy(data->pw, row[2]);
	strcpy(data->name, row[3]);
	strcpy(data->perm, row[4]);
	data->groupid = atoi(row[5]);

	return TRUE;

}
void closeDB(MYSQL** db, MYSQL_RES** res)
{
	mysql_free_result(*res);
	mysql_close(*db);
}

void display(MYSQL_RES** res, UserInfo* data)
{
	while(getQueryDataRow(res, data) == TRUE) {
		printf("%10s %10s %10s %10s %d\n", 
		data->id, data->pw, data->name, data->perm, data->groupid);
	}
}
