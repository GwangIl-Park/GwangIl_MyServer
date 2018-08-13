#include"stdafx.h"
#include"Packet.h"
#include"Database.h"
#include"UserManager.h"
#include"RoomManager.h"
#include"User.h"
BOOL User::UserInit(const SOCKET listen_socket)
{
	isConnected = FALSE;
	location = OFFLINE;
	memset(name, 0, sizeof(name));
	return SessionInit(listen_socket);
}

VOID User::SetConnected()
{
	//유저 연결
	isConnected = TRUE;
}

BOOL User::GetConnected() const
{
	//유저 연결 여부 확인
	return isConnected;
}

VOID User::SetDisconnected(const SOCKET listen_socket)
{
	//유저 연결 여부 FALSE로 바꾸고 다시 유저 초기화
	isConnected = FALSE;
	UserInit(listen_socket);
}

VOID User::SetName(const CHAR *m_name)
{
	//이름 변경
	strcpy(name, m_name); 
}

CHAR* User::GetName()
{
	//이름 얻기
	return name; 
}

VOID User::SetLocation(const INT m_location)
{
	//위치 변경
	location = m_location; 
}

INT User::GetLocation() const
{
	//위치 얻기
	return location;
}

VOID User::PROC_REG_USER(const BYTE *packet, const DWORD packetLeng)
{
	//유저 등록
	DWORD tempnameLeng;
	memcpy(&tempnameLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	CHAR tempname[32];
	memcpy(&tempname, packet + sizeof(DWORD) * 3, tempnameLeng);
	DWORD temppasswordLeng;
	memcpy(&temppasswordLeng, packet + sizeof(DWORD) * 3 + tempnameLeng, sizeof(DWORD));
	CHAR temppassword[32];
	memcpy(&temppassword, packet + sizeof(DWORD) * 4 + tempnameLeng, temppasswordLeng);
	sendPacketLeng = sizeof(DWORD) * 2;
	if (Database::getInstance().DBInsertUser(tempname, temppassword) == FALSE)               //DB에 유저 등록
	{
		Write(sendPacketLeng, PT_REG_USER, (BYTE*)"\0"); //유저에게 등록 실패 메세지 전달
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetAllUserNum(roomMemberCount);

	strcpy(name, tempname);                                               //유저 이름 변경
	location = LOBBY;                                      //유저 위치 변경
	Write(sendPacketLeng, PT_REG_USER, (BYTE*)roomMemberCount);  //유저에게 각 방 인원 전달
}

VOID User::PROC_USER_CONNECT(const BYTE *packet, const DWORD packetLeng)
{
	//유저 로그인
	DWORD tempnameLeng;
	memcpy(&tempnameLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	CHAR tempname[32];
	memcpy(&tempname, packet + sizeof(DWORD) * 3, tempnameLeng);
	DWORD temppasswordLeng;
	memcpy(&temppasswordLeng, packet + sizeof(DWORD) * 3 + tempnameLeng, sizeof(DWORD));
	CHAR temppassword[32];
	memcpy(&temppassword, packet + sizeof(DWORD) * 4 + tempnameLeng, temppasswordLeng);
	sendPacketLeng = sizeof(DWORD) * 2;
	if (UserManager::getInstance().CheckUserLogin(tempname) == TRUE || Database::getInstance().DBLoginUser(tempname, temppassword) == FALSE) //DB에서 유저 확인
	{
		Write(sendPacketLeng, PT_USER_CON, (BYTE*)"\0");            //로그인 실패 전달
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetAllUserNum(roomMemberCount);

	strcpy(name, tempname);                                               //유저 이름 변경
	location = LOBBY;                                      //유저 위치 변경
	Write(sendPacketLeng, PT_USER_CON, (BYTE*)roomMemberCount);  //유저에게 각 방 인원 전달
}

VOID User::PROC_ROOM_ENTER(const BYTE *packet, const DWORD packetLeng)
{
	//유저의 방 입장
	INT roomNum;
	memcpy(&roomNum, packet + sizeof(DWORD) * 2, sizeof(INT));
	BYTE sendPacket[MAX_BUFFER];
	DWORD sendPacketLeng = sizeof(DWORD) * 2;

	DWORD sendUserNameLeng = 0;
	sendUserNameLeng = (DWORD)strlen(name);
	sendPacketLeng += (sizeof(DWORD) + sendUserNameLeng);
	memcpy(sendPacket, &sendUserNameLeng, sizeof(DWORD));
	memcpy(sendPacket + sizeof(DWORD), name, sendUserNameLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket, sendPacketLeng, PT_ROOM_USERINC);  //방에 있는 유저들에게 입장한 유저의 이름 전달

	memset(sendPacket, 0, MAX_BUFFER);
	sendPacketLeng = sizeof(DWORD) * 2;

	INT RoomUserNum = RoomManager::getInstance().GetUserNum(roomNum);
	memcpy(sendPacket, &RoomUserNum, sizeof(INT));
	sendPacketLeng += sizeof(INT);

	UserManager::getInstance().GetRoomUsersName(sendPacket, roomNum, &sendPacketLeng);

	Write(sendPacketLeng, PT_ROOM_ENTER, sendPacket);           //방에 있는 유저들의 이름 정보 전달
	location = roomNum;
	RoomManager::getInstance().UserNumInc(roomNum);
}

VOID User::PROC_USER_CHAT(const BYTE *packet, const DWORD packetLeng)
{
	//유저가 채팅 보냈을때
	INT roomNum = location;

	DWORD UserNameLeng = 0;

	UserNameLeng = (DWORD)strlen(name);

	CHAR Message[1024];
	DWORD MessageLeng = 0;

	memcpy(&MessageLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	memcpy(Message, packet + sizeof(DWORD) * 3, MessageLeng);

	BYTE sendPacket[MAX_BUFFER];
	DWORD sendPacketLeng = sizeof(DWORD) * 4 + UserNameLeng + MessageLeng;

	memcpy(sendPacket, &UserNameLeng, sizeof(DWORD));
	memcpy(sendPacket + sizeof(DWORD), name, UserNameLeng);
	memcpy(sendPacket + sizeof(DWORD) + UserNameLeng, &MessageLeng, sizeof(DWORD));
	memcpy(sendPacket + sizeof(DWORD) * 2 + UserNameLeng, Message, MessageLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket, sendPacketLeng, PT_USER_CHAT);  //유저들에게 채팅 보낸 유저 정보와 채팅 내용 전달
}