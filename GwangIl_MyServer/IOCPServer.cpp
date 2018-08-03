#include"stdafx.h"
#include"Database.h"
#include"UserManager.h"
#include"RoomManager.h"
#include"IOCPServer.h"

unsigned WINAPI WorkerThread(VOID *arg)
{
	IOCPServer *iocpserver = (IOCPServer*)arg;
	iocpserver->WorkerThreadMain();
	return 0;
}

IOCPServer::IOCPServer()
{
	listen_socket = NULL;
	memset(&listen_addr, 0, sizeof(listen_addr));
	iocp_handle = NULL;
}

IOCPServer::~IOCPServer() {}

BOOL IOCPServer::CreateListenSocket()
{
	//Listen할 소켓 생성
	listen_socket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listen_socket == INVALID_SOCKET)
	{
		return FALSE;
	}
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	listen_addr.sin_port = htons(9190);

	if (bind(listen_socket, (SOCKADDR*)&listen_addr, sizeof(listen_addr)) == SOCKET_ERROR)
	{
		return FALSE;
	}
	if (listen(listen_socket, 100) == SOCKET_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL IOCPServer::InitIOCP()
{
	//IOCP핸들 생성
	iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);
	CreateIoCompletionPort((HANDLE)listen_socket, iocp_handle, (ULONG_PTR)listen_socket, 0); //Listen소켓 IOCP핸들에 등록
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	for (DWORD i = 0; i < system_info.dwNumberOfProcessors * 2; i++)
	{
		_beginthreadex(NULL, 0, WorkerThread, this, 0, NULL);         //스레드 생성
	}
	RoomManager::getInstance().RoomManagerInit();
	UserManager::getInstance().UserManagerInit(listen_socket);
	Database::getInstance().DBInit();
	return TRUE;
}

VOID IOCPServer::WorkerThreadMain()
{
	//워커스레드
	DWORD packetLeng = 0;
	VOID *completionKey = NULL;
	MYOVERLAPPED *myoverlapped = NULL;
	OVERLAPPED *overlapped = NULL;
	BOOL status;
	while (TRUE)
	{
		status = GetQueuedCompletionStatus(iocp_handle, &packetLeng, (PULONG_PTR)&completionKey, &overlapped, INFINITE);  //IO신호 받기
		if (completionKey == NULL)
			return;
		myoverlapped = (MYOVERLAPPED*)overlapped;
		if (status == FALSE || packetLeng == 0)
		{
			if (myoverlapped->io_type == IO_ACCEPT)                 //유저 등록
			{
				User* conUser = (User*)(myoverlapped->object);
				conUser->SetConnected();
				conUser->OnConnected(iocp_handle);
			}
			else                                                    //연결 해제
			{
				User* disconUser = (User*)(myoverlapped->object);
				disconUser->SetDisconnected(listen_socket);
			}
		}
		else
		{
			if (myoverlapped->io_type == IO_READ)                  //읽기 신호
			{
				User* readUser = (User*)(myoverlapped->object);
				readUser->OnRead(packetLeng);
			}
			else if (myoverlapped->io_type == IO_WRITE)
			{
			}
		}

	}
}