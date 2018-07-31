#pragma once
#include"Singleton.h"
class Database : public Singleton<Database>
{
private:
	MYSQL_RES *Sql_Result;
	MYSQL mysql;
	inline BOOL CONNECT()
	{
		if (mysql_real_connect(&mysql, "127.0.0.1", "root", "pki117611!", "myserver", 3307, NULL, 0) != 0)
		{
			std::cout << mysql_error(&mysql) << std::endl;
			return FALSE;
		}
		return TRUE;
	}
public:
	BOOL DBInit();
	BOOL DBInsertUser(CHAR* m_name, CHAR* m_password);
	BOOL DBLoginUser(CHAR* m_name, CHAR* m_password);
};
