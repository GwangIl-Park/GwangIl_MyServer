#include"stdafx.h"
#include"ThreadSync.h"
#include"Room.h"
#include"RoomManager.h"

BOOL RoomManager::RoomManagerInit()
{
	vector_room.reserve(6);
	for (INT i = 0; i < MAX_ROOM; i++)
	{
		Room * room = new Room();
		vector_room.push_back(room);
		room->RoomInit(i);
	}
	return TRUE;
}

INT RoomManager::GetUserNum(const INT m_roomNum)
{
	return vector_room[m_roomNum]->GetUserNum();
}

VOID RoomManager::GetAllUserNum(INT *m_userNum)
{
	for (INT i = 0;i < MAX_ROOM;i++)
	{
		m_userNum[i] = vector_room[i]->GetUserNum();
	}
}

VOID RoomManager::UserNumInc(const INT m_roomNum)
{
	ThreadSync::getInstance().Enter();
	vector_room[m_roomNum]->UserNumInc();
	ThreadSync::getInstance().Leave();
}

VOID RoomManager::UserNumDec(const INT m_roomNum)
{
	ThreadSync::getInstance().Enter();
	vector_room[m_roomNum]->UserNumDec();
	ThreadSync::getInstance().Leave();
}