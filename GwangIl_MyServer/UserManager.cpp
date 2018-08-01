#include"stdafx.h"
#include"User.h"
#include"UserManager.h"

BOOL UserManager::UserManagerInit(const SOCKET listen_socket)
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

VOID UserManager::SetUserName(const DWORD m_index, const CHAR *m_name)
{
	vector_user[m_index]->SetName(m_name);
}

CHAR* UserManager::GetUsername(const DWORD m_index)
{
	return vector_user[m_index]->GetName();
}

BOOL UserManager::CheckUserLogin(const CHAR *m_name)
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

INT UserManager::GetUserLocation(const DWORD m_index)
{
	return vector_user[m_index]->GetLocation();
}

VOID UserManager::SetUserLocation(const DWORD m_index, const INT m_location)
{
	vector_user[m_index]->SetLocation(m_location);
}

INT UserManager::GetUserCount() 
{
	return UserCount; 
}

VOID UserManager::UserCountInc() 
{
	UserCount++; 
}

VOID UserManager::GetRoomUsersName(BYTE *m_packet, const INT m_Location, DWORD *m_packetLeng, const INT m_myindex)
{
	//방에있는 유저들의 이름을 패킷에 복사
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

VOID UserManager::WriteUser(const DWORD m_index, const BYTE *data, const DWORD packetLeng, const DWORD protocol)
{
	//해당유저에게 write
	vector_user[m_index]->Write(packetLeng, protocol, data);
}

VOID UserManager::WriteRoomUsers(const INT m_location, const BYTE *data, const DWORD packetLeng, const DWORD protocol)
{
	//방에있는유저에게 write
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

VOID UserManager::WriteAll(const BYTE *data, const DWORD packetLeng, const DWORD protocol)
{
	//접속한 모든 유저에게 write
	for (INT i = 0;i < MAX_USER;i++)
	{
		if (vector_user[i]->GetConnected() == TRUE)
		{
			vector_user[i]->Write(packetLeng, protocol, data);
		}
	}
}