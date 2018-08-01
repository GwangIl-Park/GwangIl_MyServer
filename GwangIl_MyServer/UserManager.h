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
	CHAR* GetUsername(DWORD m_index) { return vector_user[m_index]->GetName(); }
	BOOL CheckUserLogin(CHAR *m_name);

	INT GetUserLocation(DWORD m_index);
	VOID SetUserLocation(DWORD m_index, INT m_location);

	INT GetUserCount() { return UserCount; }
	VOID SetUserCount() { UserCount++; }

	VOID GetRoomUsersName(BYTE *m_packet, INT m_Location, DWORD *m_packetLeng, INT m_myindex);

	VOID WriteUser(DWORD m_index, BYTE *data, DWORD packetLeng, DWORD protocol);
	VOID WriteRoomUsers(INT m_location, BYTE *data, DWORD packetLeng, DWORD protocol);
	VOID WriteAll(BYTE *data, DWORD packetLeng, DWORD protocol);
};