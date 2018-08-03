#pragma once
#include"Singleton.h"
class Database : public Singleton<Database>
{
private:
	MYSQL_RES *Sql_Result;
	MYSQL mysql;
	inline BOOL CONNECT();
public:
	BOOL DBInit();
	BOOL DBInsertUser(CHAR* m_name, CHAR* m_password);
	BOOL DBLoginUser(CHAR* m_name, CHAR* m_password);
};
