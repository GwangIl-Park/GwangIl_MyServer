#pragma once
#include"User.h"
#include"Singleton.h"
class UserManager : public Singleton<UserManager>
{
private:
	INT UserCount;
	std::vector<User*> vector_user;
public:
	BOOL UserManagerInit(SOCKET listen_socket);
	VOID SetUserName(DWORD m_index, CHAR *m_name);
	BOOL CheckUserLogin(CHAR *m_name);

	VOID SetUserLocation(DWORD m_index, Location m_location);

	INT GetUserCount() { return UserCount; }
	VOID SetUserCount() { UserCount++; }

	VOID WriteUser(DWORD m_index, BYTE *data, DWORD packetLeng, DWORD protocol);
	VOID WriteAll(BYTE *data, DWORD packetLeng, DWORD protocol);
};