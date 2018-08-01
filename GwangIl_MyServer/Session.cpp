#include"stdafx.h"
#include"Packet.h"
#include"Session.h"

Session::Session()
{
	session_socket = NULL;

	memset(&overlapped_accept, 0, sizeof(overlapped_accept));
	memset(&overlapped_read, 0, sizeof(overlapped_read));
	memset(&overlapped_write, 0, sizeof(overlapped_write));

	overlapped_accept.io_type = IO_ACCEPT;
	overlapped_read.io_type = IO_READ;
	overlapped_write.io_type = IO_WRITE;

	overlapped_accept.object = this;
	overlapped_read.object = this;
	overlapped_write.object = this;
}

Session::~Session() {}

BOOL Session::SessionInit(const SOCKET listen_socket, const DWORD m_index)
{
	index = m_index;
	memset(session_read_buffer, 0, sizeof(session_read_buffer));

	if (session_socket != NULL)
	{
		shutdown(session_socket, SD_BOTH);
		closesocket(session_socket);
		session_socket = NULL;
	}

	session_socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (AcceptEx(listen_socket, session_socket, session_read_buffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &overlapped_accept.overlapped) == FALSE)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return FALSE;
		}
	}
	PacketInit(m_index);
	return TRUE;
}

BOOL Session::InitRead()
{
	//초기 받기 진행
	WSABUF wsabuf;
	wsabuf.buf = (CHAR*)session_read_buffer;
	wsabuf.len = MAX_BUFFER;
	DWORD numberOfBytes = 0;
	DWORD flag = 0;
	INT recved = WSARecv(session_socket, &wsabuf, 1, &numberOfBytes, &flag, &overlapped_read.overlapped, NULL);
	if (recved == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL Session::OnConnected(const HANDLE iocp_handle)
{
	CreateIoCompletionPort((HANDLE)session_socket, iocp_handle, (ULONG_PTR)session_socket, 0);
	InitRead();
	return TRUE;
}

BOOL Session::OnRead(const DWORD packetLeng)
{
	ReadPacket(session_read_buffer, packetLeng);
	InitRead();
	return TRUE;
}

BOOL Session::Write(const DWORD packetLeng, const DWORD protocol, const BYTE* data)
{
	BYTE temp_write_packet[MAX_BUFFER];
	MakeWritePacket(protocol, data, packetLeng, temp_write_packet);
	WSABUF wsabuf;
	wsabuf.buf = (CHAR*)temp_write_packet;
	wsabuf.len = packetLeng;
	DWORD numberOfBytes;
	DWORD flag = 0;
	INT sended = WSASend(session_socket, &wsabuf, 1, &numberOfBytes, flag, &overlapped_write.overlapped, NULL);
	if (sended == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		return FALSE;
	}
	return TRUE;
}