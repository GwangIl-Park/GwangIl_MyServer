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

INT UserManager::GetUserLocation(DWORD m_index)
{
	return vector_user[m_index]->GetLocation();
}

VOID UserManager::SetUserLocation(DWORD m_index, INT m_location)
{
	vector_user[m_index]->SetLocation(m_location);
}

VOID UserManager::GetRoomUsersName(BYTE *m_packet, INT m_Location, DWORD *m_packetLeng, INT m_myindex)
{
	DWORD totalNameLeng = 0;
	for (INT i = 0;i < MAX_USER;i++)
	{
		if (vector_user[i]->GetConnected() == TRUE && i != m_myindex)
		{
			if (vector_user[i]->GetLocation() == m_Location)
			{
				DWORD tempNameLeng;
				CHAR tempName[32];
				strcpy(tempName, vector_user[i]->GetName());
				tempNameLeng = strlen(tempName);
				std::cout << tempNameLeng;
				memcpy(m_packet + sizeof(INT) + totalNameLeng, &tempNameLeng, sizeof(DWORD));
				memcpy(m_packet + sizeof(INT) + sizeof(DWORD) + totalNameLeng, tempName, tempNameLeng);
				totalNameLeng += (sizeof(DWORD) + tempNameLeng);
			}
		}
	}
	*m_packetLeng += totalNameLeng;
}

VOID UserManager::WriteUser(DWORD m_index, BYTE *data, DWORD packetLeng, DWORD protocol)
{
	vector_user[m_index]->Write(packetLeng, protocol, data);
}

VOID UserManager::WriteRoomUsers(INT m_location, BYTE *data, DWORD packetLeng, DWORD protocol)
{
	for (INT i = 0;i < MAX_USER;i++)
	{
		if (vector_user[i]->GetConnected() == TRUE)
		{
			if (vector_user[i]->GetLocation() == m_location)
			{
				vector_user[i]->Write(packetLeng, protocol, data);
			}
		}
	}
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