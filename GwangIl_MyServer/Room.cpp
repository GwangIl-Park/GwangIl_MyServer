#include "stdafx.h"
#include"Room.h"

BOOL Room::RoomInit(USHORT m_roomNum)
{
	roomNum = m_roomNum;
	current_user_num = 0;
	isPlaying = FALSE;
	return TRUE;
}

BOOL Room::Start()
{
	isPlaying = TRUE;
	return TRUE;
}