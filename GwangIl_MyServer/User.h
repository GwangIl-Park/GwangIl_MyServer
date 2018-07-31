#pragma once
#include"Session.h"

enum Location { OFFLINE = 0, ROOM1, ROOM2, ROOM3, ROOM4, ROOM5, ROOM6, LOBBY };

class User : public Session
{
private:
	BOOL isConnected;
	DWORD index;
	CHAR name[32];
	Location location;
public:
	BOOL UserInit(SOCKET listen_socket, DWORD m_index);

	VOID SetConnected() { isConnected = TRUE; }
	BOOL GetConnected() { return isConnected; }
	VOID SetDisconnected(SOCKET listen_socket) { UserInit(listen_socket, index); }

	VOID SetName(CHAR *m_name) { strcpy(name, m_name); }
	CHAR *GetName() { return name; }

	VOID SetLocation(Location m_location) { location = m_location; }
	Location GetLocation() { return location; }

	DWORD GetIndex() { return index; }
};