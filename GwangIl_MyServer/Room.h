#pragma once
#define MAX_ROOM_USER 6
class Room
{
private:
	INT roomNum;
	INT current_user_num;
	BOOL isPlaying;
public:
	BOOL RoomInit(USHORT roomNum);
	BOOL Start();
	INT GetUserNum() { return current_user_num; }
};