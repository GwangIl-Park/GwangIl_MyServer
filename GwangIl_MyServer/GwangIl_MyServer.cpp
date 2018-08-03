
#include "stdafx.h"
#include"IOCPServer.h"
int main()
{
	//시작 지점 입니다
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	IOCPServer iocpserver;
	iocpserver.CreateListenSocket();
	iocpserver.InitIOCP();            
	getchar();
	return 0;
}
