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
	BOOL SessionInit(const SOCKET listen_socket, const DWORD m_index);
	BOOL InitRead();
	BOOL OnConnected(const HANDLE iocp_handle);
	BOOL OnRead(const DWORD packetLeng);
	BOOL Write(const DWORD packetLeng, const DWORD protocol, const BYTE* data);
};