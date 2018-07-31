#include"stdafx.h"
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
	std::cout << "RoomManagerInit" << std::endl;
	return TRUE;
}

VOID RoomManager::GetUserNum(INT *m_userNum)
{
	for (INT i = 0;i < MAX_ROOM;i++)
	{
		m_userNum[i] = vector_room[i]->GetUserNum();
	}
}