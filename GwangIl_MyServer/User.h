#pragma once
#include"Session.h"

enum Location { ROOM1, ROOM2, ROOM3, ROOM4, ROOM5, ROOM6, LOBBY, OFFLINE };

class User : public Session
{
private:
	BOOL isConnected;
	DWORD index;
	CHAR name[32];
	INT location;
public:
	BOOL UserInit(const SOCKET listen_socket, const DWORD m_index);

	VOID SetConnected();
	BOOL GetConnected();
	VOID SetDisconnected(const SOCKET listen_socket);

	VOID SetName(const CHAR *m_name);
	CHAR* GetName();

	VOID SetLocation(const INT m_location);
	INT GetLocation();

	DWORD GetIndex();
};