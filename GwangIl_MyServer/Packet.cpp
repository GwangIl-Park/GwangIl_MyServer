#include"stdafx.h"
#include"Packet.h"

Packet::Packet()
{
	memset(packet_buffer, 0, MAX_BUFFER);
	header.h_length = (DWORD*)&packet_buffer[0];
	header.h_protocol = (Protocol*)&packet_buffer[4];
}

Packet::Packet(BYTE *m_buffer, DWORD m_packetLeng)
{
	memset(packet_buffer, 0, MAX_BUFFER);
	memcpy(packet_buffer, m_buffer, m_packetLeng);
	
	header.h_length = (DWORD*)&packet_buffer[0];
	header.h_protocol = (Protocol*)&packet_buffer[4];

	packet_pos = sizeof(DWORD) + sizeof(Protocol);
}

Packet::~Packet()
{

}

DWORD Packet::GetLength()
{
	return *header.h_length;
}

Protocol Packet::GetProtocol()
{
	return *header.h_protocol;
}

VOID Packet::ReadDWORD(DWORD* data)
{
	memcpy(data, packet_buffer + packet_pos, sizeof(DWORD));
	packet_pos += sizeof(DWORD);
}

VOID Packet::ReadINT(INT* data)
{
	memcpy(data, packet_buffer + packet_pos, sizeof(INT));
	packet_pos += sizeof(INT);
}

VOID Packet::ReadString(CHAR* data, DWORD length)
{
	memcpy(data, packet_buffer + packet_pos, length);
	packet_pos += length;
}

VOID Packet::SetHeader(Protocol protocol)
{
	*header.h_protocol = protocol;
	*header.h_length = sizeof(DWORD) + sizeof(Protocol);
}

VOID Packet::WriteDWORD(DWORD data)
{
	memcpy(packet_buffer + *header.h_length, &data, sizeof(DWORD));
	*header.h_length += sizeof(DWORD);
}

VOID Packet::WriteINT(INT data)
{
	memcpy(packet_buffer + *header.h_length, &data, sizeof(INT));
	*header.h_length += sizeof(INT);
}

VOID Packet::WriteString(CHAR* data, DWORD length)
{
	memcpy(packet_buffer + *header.h_length, data, length);
	*header.h_length += length;
}

BYTE* Packet::GetBuffer()
{
	return packet_buffer;
}

VOID Packet::PacketClear()
{
	memset(packet_buffer, 0, MAX_BUFFER);
	*header.h_length = 0;
}