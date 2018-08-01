#pragma once

enum Protocol
{
	PT_REG_USER = 1000,
	PT_USER_CON,
	PT_ROOM_ENTER,
	PT_ROOM_USERINC,
	PT_USER_CHAT
};

class Packet
{
private:
	DWORD index;
	DWORD remainReadLeng;
	DWORD sendPacketLeng;
public:
	BOOL PacketInit(DWORD m_index);
	BOOL ReadPacket(BYTE *packet, DWORD recvLeng);
	BOOL MakeWritePacket(DWORD protocol, BYTE *data, DWORD packetLeng, BYTE *packet);

	VOID PROC_REG_USER(BYTE *packet, DWORD packetLeng);
	VOID PROC_USER_CONNECT(BYTE *packet, DWORD packetLeng);
	VOID PROC_ROOM_ENTER(BYTE *packet, DWORD packetLeng);
	VOID PROC_USER_CHAT(BYTE *packet, DWORD packetLeng);
};