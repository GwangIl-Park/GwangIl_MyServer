#pragma once
#include"Singleton.h"
#include"Room.h"
#define MAX_ROOM 6

class RoomManager : public Singleton<RoomManager>
{
private:
	std::vector<Room*> vector_room;
public:
	BOOL RoomManagerInit();

	INT GetUserNum(const INT m_roomNum) const;
	VOID GetAllUserNum(INT *m_userNum);
	VOID UserNumInc(const INT m_roomNum);
	VOID UserNumDec(const INT m_roomNum);
};