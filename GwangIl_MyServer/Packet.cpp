#include"stdafx.h"
#include"Database.h"
#include"UserManager.h"
#include"RoomManager.h"
#include"Packet.h"

BOOL Packet::PacketInit(DWORD m_index)
{
	index = m_index;
	remainReadLeng = 0;
	return TRUE;
}

BOOL Packet::ReadPacket(BYTE *packet, DWORD recvLeng)
{
	std::cout << "readpacket" << std::endl;
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
	case PT_USER_CHAT:
		PROC_USER_CHAT(packet, packetLeng);
		break;
	default:
		break;
	}

	memcpy(packet, packet + packetLeng, remainReadLeng);
	remainReadLeng -= packetLeng;
	return TRUE;
}

BOOL Packet::MakeWritePacket(DWORD protocol, BYTE *data, DWORD packetLeng, BYTE *packet)
{
	memcpy(packet, &packetLeng, sizeof(DWORD));
	memcpy(packet + sizeof(DWORD), &protocol, sizeof(DWORD));
	memcpy(packet + sizeof(DWORD) + sizeof(DWORD), data, packetLeng);
	return TRUE;
}

VOID Packet::PROC_REG_USER(BYTE *packet, DWORD packetLeng)
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
		std::cout << index << std::endl;
		UserManager::getInstance().WriteUser(index, (BYTE*)"\0", sendPacketLeng, PT_REG_USER);
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetUserNum(roomMemberCount);
	//
	UserManager::getInstance().SetUserName(index, name);
	UserManager::getInstance().SetUserLocation(index, LOBBY);
	UserManager::getInstance().WriteUser(index, (BYTE*)roomMemberCount, sendPacketLeng, PT_REG_USER);
}

VOID Packet::PROC_USER_CONNECT(BYTE *packet, DWORD packetLeng)
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
		std::cout << index << std::endl;
		UserManager::getInstance().WriteUser(index, (BYTE*)"\0", sendPacketLeng, PT_REG_USER);
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetUserNum(roomMemberCount);

	UserManager::getInstance().SetUserName(index, name);
	UserManager::getInstance().SetUserLocation(index, LOBBY);
	UserManager::getInstance().WriteUser(index, (BYTE*)roomMemberCount, sendPacketLeng, PT_USER_CON);
}

VOID Packet::PROC_USER_CHAT(BYTE *packet, DWORD packetLeng)
{

}