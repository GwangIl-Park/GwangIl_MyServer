#include"stdafx.h"
#include"Packet.h"
BOOL Packet::PacketInit()
{
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