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
	BOOL CheckUserLogin(const CHAR *m_name) const;

	INT GetUserCount() const;
	VOID UserCountInc();

	VOID GetRoomUsersName(BYTE *m_packet, const INT m_Location, DWORD *m_packetLeng);

	VOID WriteRoomUsers(const INT m_location, const BYTE *data, const DWORD packetLeng, const DWORD protocol);
	VOID WriteAll(const BYTE *data, const DWORD packetLeng, const DWORD protocol);
};