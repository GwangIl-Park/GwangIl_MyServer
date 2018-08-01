#pragma once
#include"User.h"
#include"Singleton.h"
class UserManager : public Singleton<UserManager>
{
private:
	INT UserCount;
	std::vector<User*> vector_user;
public:
	BOOL UserManagerInit(const SOCKET listen_socket);
	VOID SetUserName(const DWORD m_index, const CHAR *m_name);
	CHAR* GetUsername(const DWORD m_index);
	BOOL CheckUserLogin(const CHAR *m_name);

	INT GetUserLocation(const DWORD m_index);
	VOID SetUserLocation(const DWORD m_index, const INT m_location);

	INT GetUserCount();
	VOID UserCountInc();

	VOID GetRoomUsersName(BYTE *m_packet, const INT m_Location, DWORD *m_packetLeng, const INT m_myindex);

	VOID WriteUser(const DWORD m_index, const BYTE *data, const DWORD packetLeng, const DWORD protocol);
	VOID WriteRoomUsers(const INT m_location, const BYTE *data, const DWORD packetLeng, const DWORD protocol);
	VOID WriteAll(const BYTE *data, const DWORD packetLeng, const DWORD protocol);
};