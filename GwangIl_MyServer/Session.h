#pragma once
#include"Packet.h"
class Session
{
private:
	DWORD index;
	SOCKET session_socket;
	BYTE read_buffer[MAX_BUFFER];
	MYOVERLAPPED overlapped_accept;
	MYOVERLAPPED overlapped_read;
	MYOVERLAPPED overlapped_write;
public:
	Session();
	~Session();
	BOOL SessionInit(const SOCKET listen_socket);
	BOOL InitRead();
	BOOL OnConnected(const HANDLE iocp_handle);
	BOOL OnRead(const DWORD packetLeng);
	BOOL Write(Packet &packet);
	
	virtual VOID ReadPacket(Packet &m_packet){};
};