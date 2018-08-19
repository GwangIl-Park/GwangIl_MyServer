#pragma once

class Packet
{
private:
	DWORD packet_pos;
	BYTE packet_buffer[MAX_BUFFER];

	struct Header
	{
		DWORD* h_length;
		Protocol* h_protocol;
	};
	Header header;

public:
	Packet();
	Packet(BYTE *m_buffer, DWORD m_packetLeng);
	virtual ~Packet();
	
	//Header
	DWORD GetLength();
	Protocol GetProtocol();

	//Data
	VOID ReadDWORD(DWORD* data);
	VOID ReadINT(INT* data);
	VOID ReadString(CHAR* data, DWORD length);

	VOID SetHeader(Protocol protocol);
	VOID WriteDWORD(DWORD data);
	VOID WriteINT(INT data);
	VOID WriteString(CHAR* data, DWORD length);

	BYTE* GetBuffer();
	VOID PacketClear();
};