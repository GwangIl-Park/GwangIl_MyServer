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
	DWORD remainReadLeng;
protected:
	DWORD sendPacketLeng;
public:
	BOOL PacketInit();
	BOOL ReadPacket(BYTE *packet, const DWORD recvLeng);
	BOOL MakeWritePacket(const DWORD protocol, const BYTE *data, const DWORD packetLeng, BYTE *packet);

	virtual VOID PROC_REG_USER(const BYTE *packet, const DWORD packetLeng) = 0;
	virtual VOID PROC_USER_CONNECT(const BYTE *packet, const DWORD packetLeng) = 0;
	virtual VOID PROC_ROOM_ENTER(const BYTE *packet, const DWORD packetLeng) = 0;
	virtual VOID PROC_USER_CHAT(const BYTE *packet, const DWORD packetLeng) = 0;
};