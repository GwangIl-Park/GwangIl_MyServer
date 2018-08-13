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
		newUser->UserInit(listen_socket);
	}
	return TRUE;
}

BOOL UserManager::CheckUserLogin(const CHAR *m_name) const
{
	//해당 유저가 연결 중인지 확인
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

INT UserManager::GetUserCount() const
{
	//유저 수 얻기
	return UserCount; 
}

VOID UserManager::UserCountInc() 
{
	//유저 수 증가
	UserCount++; 
}

VOID UserManager::GetRoomUsersName(BYTE *m_packet, const INT m_Location, DWORD *m_packetLeng)
{
	//방에있는 유저들의 이름을 패킷에 복사
	DWORD totalNameLeng = 0;
	for (INT i = 0;i < MAX_USER;i++)
	{
		if (vector_user[i]->GetConnected() == TRUE)
		{
			if (vector_user[i]->GetLocation() == m_Location)
			{
				DWORD tempNameLeng;
				CHAR tempName[32];
				strcpy(tempName, vector_user[i]->GetName());
				tempNameLeng = (DWORD)strlen(tempName);
				std::cout << tempNameLeng;
				memcpy(m_packet + sizeof(INT) + totalNameLeng, &tempNameLeng, sizeof(DWORD));
				memcpy(m_packet + sizeof(INT) + sizeof(DWORD) + totalNameLeng, tempName, tempNameLeng);
				totalNameLeng += (sizeof(DWORD) + tempNameLeng);
			}
		}
	}
	*m_packetLeng += totalNameLeng;
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