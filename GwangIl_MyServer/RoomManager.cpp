#include"stdafx.h"
#include"Packet.h"
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

INT RoomManager::GetUserNum(const INT m_roomNum) const
{
	//해당 방의 유저 수 얻기
	return vector_room[m_roomNum]->GetUserNum();
}

VOID RoomManager::GetAllUserNum(Packet &packet)
{
	//모든 방의 유저 수 얻기
	for (INT i = 0;i < MAX_ROOM;i++)
	{
		packet.WriteINT(vector_room[i]->GetUserNum());
	}
}

VOID RoomManager::UserNumInc(const INT m_roomNum)
{
	//해당 방의 유저 수 증가
	vector_room[m_roomNum]->UserNumInc();
}

VOID RoomManager::UserNumDec(const INT m_roomNum)
{
	//해당 방의 유저 수 감소
	vector_room[m_roomNum]->UserNumDec();
}