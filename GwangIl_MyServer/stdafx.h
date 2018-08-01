// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wsock32.lib")
#include "targetver.h"

#include<WinSock2.h>
#include<MSWSock.h>
#include<process.h>
#include<vector>
#include<iostream>
#include<mysql.h>
#define MAX_BUFFER 4096
#define MAX_USER 100

// TODO: reference additional headers your program requires here
enum IO_TYPE { IO_ACCEPT, IO_READ, IO_WRITE };

struct MYOVERLAPPED
{
	OVERLAPPED overlapped;
	IO_TYPE io_type;
	VOID *object;
};