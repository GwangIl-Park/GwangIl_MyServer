#pragma once
class IOCPServer
{
private:
	SOCKET listen_socket;
	SOCKADDR_IN listen_addr;
	HANDLE iocp_handle;
public:
	IOCPServer();
	~IOCPServer();
	BOOL CreateListenSocket();
	BOOL InitIOCP();
	VOID WorkerThreadMain();
};