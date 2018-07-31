#include"stdafx.h"
#include"User.h"

BOOL User::UserInit(SOCKET listen_socket, DWORD m_index)
{
	index = m_index;
	isConnected = FALSE;
	location = OFFLINE;
	memset(name, 0, sizeof(name));
	return SessionInit(listen_socket, m_index);
}