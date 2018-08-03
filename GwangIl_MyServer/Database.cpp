#include"stdafx.h"
#include"ThreadSync.h"
#include"UserManager.h"
#include"Database.h"

BOOL Database::DBInit()
{
	mysql_init(&mysql);
	CONNECT();
	return TRUE;
}

inline BOOL Database::CONNECT()
{
	if (mysql_real_connect(&mysql, "127.0.0.1", "root", "pki117611!", "myserver", 3307, NULL, 0) != 0)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL Database::DBInsertUser(CHAR* m_name, CHAR* m_password)
{
	if (mysql_ping(&mysql) != 0)			//DB연결이 끊어졌을 경우 다시 연결
		CONNECT();
	CHAR query[256];
	ThreadSync::getInstance().Enter();
	sprintf(query, "insert into user values ('%s', '%s')", m_name, m_password);
	if (mysql_query(&mysql, query) != 0)
	{
		return FALSE;
	}
	ThreadSync::getInstance().Leave();
	UserManager::getInstance().UserCountInc();
	return TRUE;
}

BOOL Database::DBLoginUser(CHAR* m_name, CHAR* m_password)
{
	if (mysql_ping(&mysql) != 0)
		CONNECT();

	CHAR query[256];
	ThreadSync::getInstance().Enter();
	sprintf(query, "select userpassword from user where username = '%s'", m_name);

	if (mysql_query(&mysql, query) != 0)
	{
		return FALSE;
	}

	Sql_Result = mysql_store_result(&mysql);

	if (Sql_Result->row_count == 0)                 //해당 유저가 없다면
	{
		mysql_free_result(Sql_Result);
		return FALSE;
	}

	if (strcmp(mysql_fetch_row(Sql_Result)[0], m_password) != 0)  //유저의 비밀번호가 틀리다면
	{
		mysql_free_result(Sql_Result);
		return FALSE;
	}

	mysql_free_result(Sql_Result);
	ThreadSync::getInstance().Leave();
	return TRUE;
}