#include"stdafx.h"
#include"User.h"

BOOL User::UserInit(const SOCKET listen_socket, const DWORD m_index)
{
	index = m_index;
	isConnected = FALSE;
	location = OFFLINE;
	memset(name, 0, sizeof(name));
	return SessionInit(listen_socket, m_index);
}

VOID User::SetConnected()
{
	//���� ����
	isConnected = TRUE;
}

BOOL User::GetConnected()
{
	//���� ���� ���� Ȯ��
	return isConnected;
}

VOID User::SetDisconnected(const SOCKET listen_socket)
{
	//���� ���� ���� FALSE�� �ٲٰ� �ٽ� ���� �ʱ�ȭ
	isConnected = FALSE;
	UserInit(listen_socket, index);
}

VOID User::SetName(const CHAR *m_name)
{
	//�̸� ����
	strcpy(name, m_name); 
}

CHAR* User::GetName()
{
	//�̸� ���
	return name; 
}

VOID User::SetLocation(const INT m_location)
{
	//��ġ ����
	location = m_location; 
}

INT User::GetLocation()
{
	//��ġ ���
	return location;
}

DWORD User::GetIndex()
{
	//������ �ε��� ���
	return index;
}