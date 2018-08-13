#pragma once
#define MAX_ROOM_USER 6
class Room
{
private:
	INT roomNum;
	INT current_user_num;
	BOOL isPlaying;
public:
	BOOL RoomInit(const USHORT roomNum);
	BOOL Start();
	INT GetUserNum() const;
	VOID UserNumInc();
	VOID UserNumDec();
};