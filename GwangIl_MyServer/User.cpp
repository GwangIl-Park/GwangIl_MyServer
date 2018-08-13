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
	//���� ����
	isConnected = TRUE;
}

BOOL User::GetConnected() const
{
	//���� ���� ���� Ȯ��
	return isConnected;
}

VOID User::SetDisconnected(const SOCKET listen_socket)
{
	//���� ���� ���� FALSE�� �ٲٰ� �ٽ� ���� �ʱ�ȭ
	isConnected = FALSE;
	UserInit(listen_socket);
}

VOID User::SetName(const CHAR *m_name)
{
	//�̸� ����
	strcpy(name, m_name); 
}

CHAR* User::GetName()
{
	//�̸� ���
	return name; 
}

VOID User::SetLocation(const INT m_location)
{
	//��ġ ����
	location = m_location; 
}

INT User::GetLocation() const
{
	//��ġ ���
	return location;
}

VOID User::PROC_REG_USER(const BYTE *packet, const DWORD packetLeng)
{
	//���� ���
	DWORD tempnameLeng;
	memcpy(&tempnameLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	CHAR tempname[32];
	memcpy(&tempname, packet + sizeof(DWORD) * 3, tempnameLeng);
	DWORD temppasswordLeng;
	memcpy(&temppasswordLeng, packet + sizeof(DWORD) * 3 + tempnameLeng, sizeof(DWORD));
	CHAR temppassword[32];
	memcpy(&temppassword, packet + sizeof(DWORD) * 4 + tempnameLeng, temppasswordLeng);
	sendPacketLeng = sizeof(DWORD) * 2;
	if (Database::getInstance().DBInsertUser(tempname, temppassword) == FALSE)               //DB�� ���� ���
	{
		Write(sendPacketLeng, PT_REG_USER, (BYTE*)"\0"); //�������� ��� ���� �޼��� ����
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetAllUserNum(roomMemberCount);

	strcpy(name, tempname);                                               //���� �̸� ����
	location = LOBBY;                                      //���� ��ġ ����
	Write(sendPacketLeng, PT_REG_USER, (BYTE*)roomMemberCount);  //�������� �� �� �ο� ����
}

VOID User::PROC_USER_CONNECT(const BYTE *packet, const DWORD packetLeng)
{
	//���� �α���
	DWORD tempnameLeng;
	memcpy(&tempnameLeng, packet + sizeof(DWORD) * 2, sizeof(DWORD));
	CHAR tempname[32];
	memcpy(&tempname, packet + sizeof(DWORD) * 3, tempnameLeng);
	DWORD temppasswordLeng;
	memcpy(&temppasswordLeng, packet + sizeof(DWORD) * 3 + tempnameLeng, sizeof(DWORD));
	CHAR temppassword[32];
	memcpy(&temppassword, packet + sizeof(DWORD) * 4 + tempnameLeng, temppasswordLeng);
	sendPacketLeng = sizeof(DWORD) * 2;
	if (UserManager::getInstance().CheckUserLogin(tempname) == TRUE || Database::getInstance().DBLoginUser(tempname, temppassword) == FALSE) //DB���� ���� Ȯ��
	{
		Write(sendPacketLeng, PT_USER_CON, (BYTE*)"\0");            //�α��� ���� ����
		return;
	}

	sendPacketLeng += sizeof(INT) * 6;
	INT roomMemberCount[6];
	RoomManager::getInstance().GetAllUserNum(roomMemberCount);

	strcpy(name, tempname);                                               //���� �̸� ����
	location = LOBBY;                                      //���� ��ġ ����
	Write(sendPacketLeng, PT_USER_CON, (BYTE*)roomMemberCount);  //�������� �� �� �ο� ����
}

VOID User::PROC_ROOM_ENTER(const BYTE *packet, const DWORD packetLeng)
{
	//������ �� ����
	INT roomNum;
	memcpy(&roomNum, packet + sizeof(DWORD) * 2, sizeof(INT));
	BYTE sendPacket[MAX_BUFFER];
	DWORD sendPacketLeng = sizeof(DWORD) * 2;

	DWORD sendUserNameLeng = 0;
	sendUserNameLeng = (DWORD)strlen(name);
	sendPacketLeng += (sizeof(DWORD) + sendUserNameLeng);
	memcpy(sendPacket, &sendUserNameLeng, sizeof(DWORD));
	memcpy(sendPacket + sizeof(DWORD), name, sendUserNameLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket, sendPacketLeng, PT_ROOM_USERINC);  //�濡 �ִ� �����鿡�� ������ ������ �̸� ����

	memset(sendPacket, 0, MAX_BUFFER);
	sendPacketLeng = sizeof(DWORD) * 2;

	INT RoomUserNum = RoomManager::getInstance().GetUserNum(roomNum);
	memcpy(sendPacket, &RoomUserNum, sizeof(INT));
	sendPacketLeng += sizeof(INT);

	UserManager::getInstance().GetRoomUsersName(sendPacket, roomNum, &sendPacketLeng);

	Write(sendPacketLeng, PT_ROOM_ENTER, sendPacket);           //�濡 �ִ� �������� �̸� ���� ����
	location = roomNum;
	RoomManager::getInstance().UserNumInc(roomNum);
}

VOID User::PROC_USER_CHAT(const BYTE *packet, const DWORD packetLeng)
{
	//������ ä�� ��������
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

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket, sendPacketLeng, PT_USER_CHAT);  //�����鿡�� ä�� ���� ���� ������ ä�� ���� ����
}