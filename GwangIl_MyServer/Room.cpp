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
	//게임 시작
	isPlaying = TRUE;
	return TRUE;
}

INT Room::GetUserNum()
{
	//방에 있는 유저 수 얻기
	return current_user_num;
}

VOID Room::UserNumInc() 
{
	//유저 수 증가
	current_user_num++;
}

VOID Room::UserNumDec()
{
	//유저 수 감소
	current_user_num--;
}