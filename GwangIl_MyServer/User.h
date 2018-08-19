#pragma once
#include"Session.h"
enum Location { ROOM1, ROOM2, ROOM3, ROOM4, ROOM5, ROOM6, LOBBY, OFFLINE };

class User : public Session
{
private:
	BOOL isConnected;
	CHAR name[32];
	INT location;
public:
	BOOL UserInit(const SOCKET listen_socket);

	VOID SetConnected();
	BOOL GetConnected() const;
	VOID SetDisconnected(const SOCKET listen_socket);

	VOID SetName(const CHAR *m_name);
	CHAR* GetName();

	VOID SetLocation(const INT m_location);
	INT GetLocation() const;

	virtual VOID ReadPacket(Packet &m_packet);

	VOID PROC_REG_USER(Packet& packet);
	VOID PROC_USER_CONNECT(Packet& packet);
	VOID PROC_ROOM_ENTER(Packet& packet);
	VOID PROC_USER_CHAT(Packet& packet);
};