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

VOID User::ReadPacket(Packet &m_packet)
{
	std::cout << "ReadPacket" << std::endl;
	switch (m_packet.GetProtocol())
	{
	case PT_REG_USER:
		PROC_REG_USER(m_packet);
		break;
	case PT_USER_CON:
		PROC_USER_CONNECT(m_packet);
		break;
	case PT_ROOM_ENTER:
		PROC_ROOM_ENTER(m_packet);
		break;
	case PT_USER_CHAT:
		PROC_USER_CHAT(m_packet);
	default:
		break;
	}
}

VOID User::PROC_REG_USER(Packet& packet)  //���� ���
{
	/* LENGTH / PROTOCOL / NAMELENG / NAME / PASSWORDLENG / PASSWORD */


	DWORD tempnameLeng;
	CHAR tempname[32];
	DWORD temppasswordLeng;
	CHAR temppassword[32];

	packet.ReadDWORD(&tempnameLeng);
	packet.ReadString(tempname, tempnameLeng);
	packet.ReadDWORD(&temppasswordLeng);
	packet.ReadString(temppassword, tempnameLeng);

	Packet sendPacket;
	sendPacket.SetHeader(PT_REG_USER);
	if (Database::getInstance().DBInsertUser(tempname, temppassword) == FALSE)               //DB�� ���� ���
	{
		/* LENGTH / PROTOCOL / "\0" */
		Write(sendPacket); //�������� ��� ���� �޼��� ����
		return;
	}

	RoomManager::getInstance().GetAllUserNum(sendPacket);
	isConnected = true;
	strcpy(name, tempname);                                               //���� �̸� ����
	location = LOBBY;                                      //���� ��ġ ����
	/* LENGTH / PROTOCOL / ���������� */
	Write(sendPacket);  //�������� �� �� �ο� ����
}

VOID User::PROC_USER_CONNECT(Packet& packet)
{
	std::cout << "PROC_USER_CONNECT" << std::endl;
	//���� �α���
	/* LENGTH / PROTOCOL / NAMELENG / NAME / PASSWORDLENG / PASSWORD */
	DWORD tempnameLeng;
	CHAR tempname[32];
	DWORD temppasswordLeng;
	CHAR temppassword[32];

	packet.ReadDWORD(&tempnameLeng);
	packet.ReadString(tempname, tempnameLeng);
	packet.ReadDWORD(&temppasswordLeng);
	packet.ReadString(temppassword, tempnameLeng);

	Packet sendPacket;
	sendPacket.SetHeader(PT_USER_CON);
	if (UserManager::getInstance().CheckUserLogin(tempname) == TRUE || Database::getInstance().DBLoginUser(tempname, temppassword) == FALSE) //DB���� ���� Ȯ��
	{
		Write(sendPacket);            //�α��� ���� ����
		return;
	}
	std::cout << "����" << std::endl;
	RoomManager::getInstance().GetAllUserNum(sendPacket);
	isConnected = true;
	strcpy(name, tempname);                                               //���� �̸� ����
	location = LOBBY;                                      //���� ��ġ ����
	/* LENGTH / PROTOCOL / ���������� */
	Write(sendPacket);  //�������� �� �� �ο� ����
}

VOID User::PROC_ROOM_ENTER(Packet& packet)
{
	//������ �� ����
	/* LENGTH / PROTOCOL / ROOMNUM / NAMELENG / NAME */
	INT roomNum;
	packet.ReadINT(&roomNum);

	Packet sendPacket; 
	packet.SetHeader(PT_ROOM_USERINC);
	DWORD tempnameLeng;
	CHAR tempname[32];
	
	packet.ReadDWORD(&tempnameLeng);
	packet.ReadString(tempname, tempnameLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket);  //�濡 �ִ� �����鿡�� ������ ������ �̸� ����

	sendPacket.PacketClear();
	sendPacket.SetHeader(PT_ROOM_ENTER);

	INT RoomUserNum = RoomManager::getInstance().GetUserNum(roomNum);
	sendPacket.WriteINT(RoomUserNum);

	UserManager::getInstance().GetRoomUsersName(roomNum, sendPacket);

	Write(sendPacket);           //�濡 �ִ� �������� �̸� ���� ����
	location = roomNum;
	RoomManager::getInstance().UserNumInc(roomNum);
}

VOID User::PROC_USER_CHAT(Packet& packet)
{
	//������ ä�� ��������
	/* LENGTH / PROTOCOL / MESSAGELENG / MESSAGE*/
	INT roomNum = location;

	DWORD UserNameLeng = 0;

	UserNameLeng = (DWORD)strlen(name);

	CHAR Message[1024];
	DWORD MessageLeng = 0;

	packet.ReadDWORD(&MessageLeng);
	packet.ReadString(Message, MessageLeng);

	Packet sendPacket;
	sendPacket.SetHeader(PT_USER_CHAT);
	sendPacket.WriteDWORD(UserNameLeng);
	sendPacket.WriteString(name, UserNameLeng);
	sendPacket.WriteDWORD(MessageLeng);
	sendPacket.WriteString(Message, MessageLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket);  //�����鿡�� ä�� ���� ���� ������ ä�� ���� ����
}