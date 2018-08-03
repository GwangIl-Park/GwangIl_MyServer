#include"stdafx.h"
#include"Database.h"
#include"UserManager.h"
#include"RoomManager.h"
#include"Packet.h"

BOOL Packet::PacketInit(const DWORD m_index)
{
	index = m_index;
	remainReadLeng = 0;
	return TRUE;
}

BOOL Packet::ReadPacket(BYTE *packet, const DWORD recvLeng)
{
	//패킷의 프로토콜에 따른 처리 함수 연결
	DWORD packetLeng = 0;
	DWORD protocol = 0;
	memcpy(&packetLeng, packet, sizeof(DWORD));
	memcpy(&protocol, packet + sizeof(DWORD), sizeof(DWORD));
	remainReadLeng += recvLeng;
	switch (protocol)
	{
	case PT_REG_USER:
		PROC_REG_USER(packet, packetLeng);
		break;
	case PT_USER_CON:
		PROC_USER_CONNECT(packet, packetLeng);
		break;
	case PT_ROOM_ENTER:
		PROC_ROOM_ENTER(packet, packetLeng);
		break;
	case PT_USER_CHAT:
		PROC_USER_CHAT(packet, packetLeng);
	default:
		break;
	}

	memcpy(packet, packet + packetLeng, remainReadLeng);
	remainReadLeng -= packetLeng;
	return TRUE;
}

BOOL Packet::MakeWritePacket(const DWORD protocol, const BYTE *data, const DWORD packetLeng, BYTE *packet)
{
	//형식에 따라 패킷 생성
	memcpy(packet, &packetLeng, sizeof(DWORD));
	memcpy(packet + sizeof(DWORD), &protocol, sizeof(DWORD));
	memcpy(packet + sizeof(DWORD) + sizeof(DWORD), data, packetLeng);
	return TRUE;
}

VOID Packet::PROC_REG_USER(const BYTE *packet, const DWORD packetLeng)
{
	//유저 등록
	DWORD nameLeng;
	memcpy(&nameLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	CHAR name[32];
	memcpy(&name, packet + sizeof(DWORD) * 3, nameLeng);
	DWORD passwordLeng;
	memcpy(&passwordLeng, packet + sizeof(DWORD) * 3 + nameLeng, sizeof(DWORD));
	CHAR password[32];
	memcpy(&password, packet + sizeof(DWORD) * 4 + nameLeng, passwordLeng);
	sendPacketLeng = sizeof(DWORD) * 2;
	if (Database::getInstance().DBInsertUser(name, password) == FALSE)               //DB에 유저 등록
	{
		UserManager::getInstance().WriteUser(index, (BYTE*)"\0", sendPacketLeng, PT_REG_USER); //유저에게 등록 실패 메세지 전달
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetAllUserNum(roomMemberCount);
	
	UserManager::getInstance().SetUserName(index, name);                                               //유저 이름 변경
	UserManager::getInstance().SetUserLocation(index, LOBBY);                                          //유저 위치 변경
	UserManager::getInstance().WriteUser(index, (BYTE*)roomMemberCount, sendPacketLeng, PT_REG_USER);  //유저에게 각 방 인원 전달
}

VOID Packet::PROC_USER_CONNECT(const BYTE *packet, const DWORD packetLeng)
{
	//유저 로그인
	DWORD nameLeng;
	memcpy(&nameLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	CHAR name[32];
	memcpy(&name, packet + sizeof(DWORD) * 3, nameLeng);
	DWORD passwordLeng;
	memcpy(&passwordLeng, packet + sizeof(DWORD) * 3 + nameLeng, sizeof(DWORD));
	CHAR password[32];
	memcpy(&password, packet + sizeof(DWORD) * 4 + nameLeng, passwordLeng);
	sendPacketLeng = sizeof(DWORD) * 2;
	if (UserManager::getInstance().CheckUserLogin(name) == TRUE || Database::getInstance().DBLoginUser(name, password) == FALSE) //DB에서 유저 확인
	{
		UserManager::getInstance().WriteUser(index, (BYTE*)"\0", sendPacketLeng, PT_USER_CON);            //로그인 실패 전달
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetAllUserNum(roomMemberCount);

	UserManager::getInstance().SetUserName(index, name);
	UserManager::getInstance().SetUserLocation(index, LOBBY);
	UserManager::getInstance().WriteUser(index, (BYTE*)roomMemberCount, sendPacketLeng, PT_USER_CON);
}

VOID Packet::PROC_ROOM_ENTER(const BYTE *packet, const DWORD packetLeng)
{
	//유저의 방 입장
	INT roomNum;
	memcpy(&roomNum, packet + sizeof(DWORD) * 2, sizeof(INT));
	BYTE sendPacket[MAX_BUFFER];
	DWORD sendPacketLeng = sizeof(DWORD) * 2;

	CHAR sendUserName[32];
	DWORD sendUserNameLeng = 0;
	strcpy((CHAR*)sendUserName, UserManager::getInstance().GetUsername(index));
	sendUserNameLeng = (DWORD)strlen(sendUserName);
	sendPacketLeng += (sizeof(DWORD) + sendUserNameLeng);
	memcpy(sendPacket, &sendUserNameLeng, sizeof(DWORD));
	memcpy(sendPacket + sizeof(DWORD), sendUserName, sendUserNameLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket, sendPacketLeng, PT_ROOM_USERINC);  //방에 있는 유저들에게 입장한 유저의 이름 전달

	memset(sendPacket, 0, MAX_BUFFER);
	sendPacketLeng = sizeof(DWORD) * 2;

	INT RoomUserNum = RoomManager::getInstance().GetUserNum(roomNum);
	memcpy(sendPacket, &RoomUserNum, sizeof(INT));
	sendPacketLeng += sizeof(INT);

	UserManager::getInstance().GetRoomUsersName(sendPacket, roomNum, &sendPacketLeng, index);
	INT temp1;
	memcpy(&temp1, sendPacket + 12, sizeof(INT));
	UserManager::getInstance().WriteUser(index, sendPacket, sendPacketLeng, PT_ROOM_ENTER);            //방에 있는 유저들의 이름 정보 전달

	RoomManager::getInstance().UserNumInc(roomNum);
	UserManager::getInstance().SetUserLocation(index, roomNum);
}

VOID Packet::PROC_USER_CHAT(const BYTE *packet, const DWORD packetLeng)
{
	//유저가 채팅 보냈을때
	INT roomNum = UserManager::getInstance().GetUserLocation(index);

	CHAR UserName[32];
	DWORD UserNameLeng = 0;

	strcpy(UserName, UserManager::getInstance().GetUsername(index));
	UserNameLeng = (DWORD)strlen(UserName);

	CHAR Message[1024];
	DWORD MessageLeng = 0;

	memcpy(&MessageLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	memcpy(Message, packet + sizeof(DWORD) * 3, MessageLeng);

	BYTE sendPacket[MAX_BUFFER];
	DWORD sendPacketLeng = sizeof(DWORD) * 4 + UserNameLeng + MessageLeng;

	memcpy(sendPacket, &UserNameLeng, sizeof(DWORD));
	memcpy(sendPacket + sizeof(DWORD), UserName, UserNameLeng);
	memcpy(sendPacket + sizeof(DWORD) + UserNameLeng, &MessageLeng, sizeof(DWORD));
	memcpy(sendPacket + sizeof(DWORD) * 2 + UserNameLeng, Message, MessageLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket, sendPacketLeng, PT_USER_CHAT);  //유저들에게 채팅 보낸 유저 정보와 채팅 내용 전달
}