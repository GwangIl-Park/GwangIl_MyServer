#pragma once
#include"Singleton.h"

class ThreadSync : public Singleton<ThreadSync>
{
private:
	CRITICAL_SECTION cs;
public:
	ThreadSync()
	{
		std::cout << "ThreadSync" << std::endl;
		InitializeCriticalSection(&cs);
	}
	~ThreadSync()
	{
		std::cout << "~ThreadSync" << std::endl;
		DeleteCriticalSection(&cs);
	}

	inline VOID Enter()
	{
		EnterCriticalSection(&cs);
	}

	inline VOID Leave()
	{
		LeaveCriticalSection(&cs);
	}
};