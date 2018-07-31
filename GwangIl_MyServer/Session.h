#pragma once
#include"Packet.h"
class Session : public Packet
{
private:
	DWORD index;
	SOCKET session_socket;
	BYTE session_read_buffer[MAX_BUFFER];
	MYOVERLAPPED overlapped_accept;
	MYOVERLAPPED overlapped_read;
	MYOVERLAPPED overlapped_write;
public:
	Session();
	~Session();
	BOOL SessionInit(SOCKET listen_socket, DWORD m_index);
	BOOL InitRead();
	BOOL OnConnected(HANDLE iocp_handle);
	BOOL OnDisconnected();
	BOOL OnRead(DWORD packetLeng);
	BOOL Write(DWORD packetLeng, DWORD protocol, BYTE* data);
};