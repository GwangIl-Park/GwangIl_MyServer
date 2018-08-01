#pragma once
#define MAX_ROOM 6
#include"Room.h"
#include"Singleton.h"

class RoomManager : public Singleton<RoomManager>
{
private:
	std::vector<Room*> vector_room;
public:
	BOOL RoomManagerInit();

	INT GetUserNum(INT m_roomNum);
	VOID GetAllUserNum(INT *m_userNum);
	VOID UserNumInc(INT m_roomNum);
	VOID UserNumDec(INT m_roomNum);
};