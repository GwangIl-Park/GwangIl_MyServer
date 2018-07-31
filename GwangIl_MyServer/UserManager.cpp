#include"stdafx.h"
#include"User.h"
#include"UserManager.h"

BOOL UserManager::UserManagerInit(SOCKET listen_socket)
{
	UserCount = 0;
	for (DWORD i = 0; i < MAX_USER; i++)
	{
		User *newUser = new User();
		vector_user.push_back(newUser);
		newUser->UserInit(listen_socket, i);
	}
	return TRUE;
}

VOID UserManager::SetUserName(DWORD m_index, CHAR *m_name)
{
	vector_user[m_index]->SetName(m_name);
}

BOOL UserManager::CheckUserLogin(CHAR *m_name)
{
	for (DWORD i = 0;i < MAX_USER;i++)
	{
		if (vector_user[i]->GetConnected() == TRUE)
		{
			if (strcmp(m_name, vector_user[i]->GetName()) == 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

VOID UserManager::SetUserLocation(DWORD m_index, Location m_location)
{
	vector_user[m_index]->SetLocation(m_location);
}

VOID UserManager::WriteUser(DWORD m_index, BYTE *data, DWORD packetLeng, DWORD protocol)
{
	std::cout << m_index << std::endl;
	vector_user[m_index]->Write(packetLeng, protocol, data);
}

VOID UserManager::WriteAll(BYTE *data, DWORD packetLeng, DWORD protocol)
{
	for (INT i = 0;i < MAX_USER;i++)
	{
		if (vector_user[i]->GetConnected() == TRUE)
		{
			vector_user[i]->Write(packetLeng, protocol, data);
		}
	}
}