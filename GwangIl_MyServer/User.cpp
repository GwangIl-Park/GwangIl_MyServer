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
	//유저 연결
	isConnected = TRUE;
}

BOOL User::GetConnected()
{
	//유저 연결 여부 확인
	return isConnected;
}

VOID User::SetDisconnected(const SOCKET listen_socket)
{
	//유저 연결 여부 FALSE로 바꾸고 다시 유저 초기화
	isConnected = FALSE;
	UserInit(listen_socket, index);
}

VOID User::SetName(const CHAR *m_name)
{
	//이름 변경
	strcpy(name, m_name); 
}

CHAR* User::GetName()
{
	//이름 얻기
	return name; 
}

VOID User::SetLocation(const INT m_location)
{
	//위치 변경
	location = m_location; 
}

INT User::GetLocation()
{
	//위치 얻기
	return location;
}

DWORD User::GetIndex()
{
	//유저의 인덱스 얻기
	return index;
}