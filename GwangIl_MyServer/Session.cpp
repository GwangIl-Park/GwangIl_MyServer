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

BOOL Session::SessionInit(const SOCKET listen_socket)
{
	memset(read_buffer, 0, sizeof(read_buffer));

	if (session_socket != NULL)
	{
		shutdown(session_socket, SD_BOTH);
		closesocket(session_socket);
		session_socket = NULL;
	}

	session_socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (AcceptEx(listen_socket, session_socket, read_buffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &overlapped_accept.overlapped) == FALSE)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return FALSE;
		}
	}
	//���� �̸� �����صΰ� Listen���Ͽ� Accept��ȣ�� ���� ����
	return TRUE;
}

BOOL Session::InitRead()
{
	//�ʱ� �ޱ� ����
	WSABUF wsabuf;
	wsabuf.buf = (CHAR*)read_buffer;
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
	//���� ��ȣ�� ���� Accept�� ������ IOCP�ڵ鿡 ����
	CreateIoCompletionPort((HANDLE)session_socket, iocp_handle, (ULONG_PTR)session_socket, 0);
	InitRead();
	return TRUE;
}

BOOL Session::OnRead(const DWORD m_packetLeng)
{
	std::cout << "OnRead" << std::endl;
	//�б� ��ȣ ��������
	Packet recvPacket(read_buffer, m_packetLeng);
	ReadPacket(recvPacket);
	memcpy(read_buffer, read_buffer + m_packetLeng, m_packetLeng);
	//��Ŷ ó���ϰ� �ٽ� �б� ����
	InitRead();
	return TRUE;
}

BOOL Session::Write(Packet &packet)
{
	WSABUF wsabuf;
	wsabuf.buf = (CHAR*)packet.GetBuffer();
	wsabuf.len = packet.GetLength();
	printf("%d", wsabuf.buf[0]);
	DWORD numberOfBytes;
	DWORD flag = 0;
	INT sended = WSASend(session_socket, &wsabuf, 1, &numberOfBytes, flag, &overlapped_write.overlapped, NULL);
	if (sended == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		return FALSE;
	}
	return TRUE;
}