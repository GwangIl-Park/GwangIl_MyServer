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

	VOID GetRoomUsersName(const INT m_location, Packet &packet);

	VOID WriteRoomUsers(const INT m_location, Packet &packet);
	VOID WriteAll(Packet &packet);
};