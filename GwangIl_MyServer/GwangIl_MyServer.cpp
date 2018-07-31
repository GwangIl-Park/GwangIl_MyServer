
#include "stdafx.h"
#include"IOCPServer.h"
int main()
{
	std::cout << sizeof(DWORD);
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	IOCPServer iocpserver;
	iocpserver.CreateListenSocket();
	iocpserver.InitIOCP();
	getchar();
	return 0;
}
