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
	BOOL PacketInit(const DWORD m_index);
	BOOL ReadPacket(BYTE *packet, const DWORD recvLeng);
	BOOL MakeWritePacket(const DWORD protocol, const BYTE *data, const DWORD packetLeng, BYTE *packet);

	VOID PROC_REG_USER(const BYTE *packet, const DWORD packetLeng);
	VOID PROC_USER_CONNECT(const BYTE *packet, const DWORD packetLeng);
	VOID PROC_ROOM_ENTER(const BYTE *packet, const DWORD packetLeng);
	VOID PROC_USER_CHAT(const BYTE *packet, const DWORD packetLeng);
};