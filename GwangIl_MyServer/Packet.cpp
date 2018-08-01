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
	memcpy(packet, &packetLeng, sizeof(DWORD));
	memcpy(packet + sizeof(DWORD), &protocol, sizeof(DWORD));
	memcpy(packet + sizeof(DWORD) + sizeof(DWORD), data, packetLeng);
	return TRUE;
}

VOID Packet::PROC_REG_USER(const BYTE *packet, const DWORD packetLeng)
{
	DWORD nameLeng;
	memcpy(&nameLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	CHAR name[32];
	memcpy(&name, packet + sizeof(DWORD) * 3, nameLeng);
	DWORD passwordLeng;
	memcpy(&passwordLeng, packet + sizeof(DWORD) * 3 + nameLeng, sizeof(DWORD));
	CHAR password[32];
	memcpy(&password, packet + sizeof(DWORD) * 4 + nameLeng, passwordLeng);
	sendPacketLeng = sizeof(DWORD) * 2;
	if (Database::getInstance().DBInsertUser(name, password) == FALSE)
	{
		UserManager::getInstance().WriteUser(index, (BYTE*)"\0", sendPacketLeng, PT_REG_USER);
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetAllUserNum(roomMemberCount);
	//
	UserManager::getInstance().SetUserName(index, name);
	UserManager::getInstance().SetUserLocation(index, LOBBY);
	UserManager::getInstance().WriteUser(index, (BYTE*)roomMemberCount, sendPacketLeng, PT_REG_USER);
}

VOID Packet::PROC_USER_CONNECT(const BYTE *packet, const DWORD packetLeng)
{
	DWORD nameLeng;
	memcpy(&nameLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	CHAR name[32];
	memcpy(&name, packet + sizeof(DWORD) * 3, nameLeng);
	DWORD passwordLeng;
	memcpy(&passwordLeng, packet + sizeof(DWORD) * 3 + nameLeng, sizeof(DWORD));
	CHAR password[32];
	memcpy(&password, packet + sizeof(DWORD) * 4 + nameLeng, passwordLeng);
	sendPacketLeng = sizeof(DWORD) * 2;
	if (UserManager::getInstance().CheckUserLogin(name) == TRUE || Database::getInstance().DBLoginUser(name, password) == FALSE)
	{
		UserManager::getInstance().WriteUser(index, (BYTE*)"\0", sendPacketLeng, PT_REG_USER);
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
	INT roomNum;
	memcpy(&roomNum, packet + sizeof(DWORD) * 2, sizeof(INT));
	BYTE sendPacket[MAX_BUFFER];
	DWORD sendPacketLeng = sizeof(DWORD) * 2;

	CHAR sendUserName[32];
	DWORD sendUserNameLeng = 0;
	strcpy((CHAR*)sendUserName, UserManager::getInstance().GetUsername(index));
	sendUserNameLeng = strlen(sendUserName);
	sendPacketLeng += (sizeof(DWORD) + sendUserNameLeng);
	memcpy(sendPacket, &sendUserNameLeng, sizeof(DWORD));
	memcpy(sendPacket + sizeof(DWORD), sendUserName, sendUserNameLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket, sendPacketLeng, PT_ROOM_USERINC);

	memset(sendPacket, 0, MAX_BUFFER);
	sendPacketLeng = sizeof(DWORD) * 2;

	INT RoomUserNum = RoomManager::getInstance().GetUserNum(roomNum);
	memcpy(sendPacket, &RoomUserNum, sizeof(INT));
	sendPacketLeng += sizeof(INT);

	UserManager::getInstance().GetRoomUsersName(sendPacket, roomNum, &sendPacketLeng, index);
	INT temp1;
	memcpy(&temp1, sendPacket + 12, sizeof(INT));
	UserManager::getInstance().WriteUser(index, sendPacket, sendPacketLeng, PT_ROOM_ENTER);

	RoomManager::getInstance().UserNumInc(roomNum);
	UserManager::getInstance().SetUserLocation(index, roomNum);
}

VOID Packet::PROC_USER_CHAT(const BYTE *packet, const DWORD packetLeng)
{
	INT roomNum = UserManager::getInstance().GetUserLocation(index);

	CHAR UserName[32];
	DWORD UserNameLeng = 0;

	strcpy(UserName, UserManager::getInstance().GetUsername(index));
	UserNameLeng = strlen(UserName);

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

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket, sendPacketLeng, PT_USER_CHAT);
}