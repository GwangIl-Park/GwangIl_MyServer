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
	VOID GetUserNum(INT *m_userNum);
};