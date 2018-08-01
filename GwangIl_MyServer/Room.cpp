#include "stdafx.h"
#include"Room.h"

BOOL Room::RoomInit(const USHORT m_roomNum)
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

INT Room::GetUserNum()
{
	return current_user_num;
}

VOID Room::UserNumInc() 
{
	current_user_num++;
}

VOID Room::UserNumDec()
{
	current_user_num--;
}