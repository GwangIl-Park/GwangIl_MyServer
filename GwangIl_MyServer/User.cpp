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
	isConnected = TRUE;
}

BOOL User::GetConnected()
{
	return isConnected;
}

VOID User::SetDisconnected(const SOCKET listen_socket)
{
	UserInit(listen_socket, index);
}

VOID User::SetName(const CHAR *m_name)
{
	strcpy(name, m_name); 
}

CHAR* User::GetName()
{
	return name; 
}

VOID User::SetLocation(const INT m_location)
{
	location = m_location; 
}

INT User::GetLocation()
{
	return location;
}

DWORD User::GetIndex()
{
	return index;
}