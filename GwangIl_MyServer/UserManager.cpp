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
	//�ش� ������ ���� ������ Ȯ��
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
	//���� �� ���
	return UserCount; 
}

VOID UserManager::UserCountInc() 
{
	//���� �� ����
	UserCount++; 
}

VOID UserManager::GetRoomUsersName(BYTE *m_packet, const INT m_Location, DWORD *m_packetLeng)
{
	//�濡�ִ� �������� �̸��� ��Ŷ�� ����
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
	//�濡�ִ��������� write
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
	//������ ��� �������� write
	for (INT i = 0;i < MAX_USER;i++)
	{
		if (vector_user[i]->GetConnected() == TRUE)
		{
			vector_user[i]->Write(packetLeng, protocol, data);
		}
	}
}