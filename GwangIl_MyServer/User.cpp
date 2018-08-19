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
	//유저 연결
	isConnected = TRUE;
}

BOOL User::GetConnected() const
{
	//유저 연결 여부 확인
	return isConnected;
}

VOID User::SetDisconnected(const SOCKET listen_socket)
{
	//유저 연결 여부 FALSE로 바꾸고 다시 유저 초기화
	isConnected = FALSE;
	UserInit(listen_socket);
}

VOID User::SetName(const CHAR *m_name)
{
	//이름 변경
	strcpy(name, m_name); 
}

CHAR* User::GetName()
{
	//이름 얻기
	return name; 
}

VOID User::SetLocation(const INT m_location)
{
	//위치 변경
	location = m_location; 
}

INT User::GetLocation() const
{
	//위치 얻기
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

VOID User::PROC_REG_USER(Packet& packet)  //유저 등록
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
	if (Database::getInstance().DBInsertUser(tempname, temppassword) == FALSE)               //DB에 유저 등록
	{
		/* LENGTH / PROTOCOL / "\0" */
		Write(sendPacket); //유저에게 등록 실패 메세지 전달
		return;
	}

	RoomManager::getInstance().GetAllUserNum(sendPacket);
	isConnected = true;
	strcpy(name, tempname);                                               //유저 이름 변경
	location = LOBBY;                                      //유저 위치 변경
	/* LENGTH / PROTOCOL / 각방유저수 */
	Write(sendPacket);  //유저에게 각 방 인원 전달
}

VOID User::PROC_USER_CONNECT(Packet& packet)
{
	std::cout << "PROC_USER_CONNECT" << std::endl;
	//유저 로그인
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
	if (UserManager::getInstance().CheckUserLogin(tempname) == TRUE || Database::getInstance().DBLoginUser(tempname, temppassword) == FALSE) //DB에서 유저 확인
	{
		Write(sendPacket);            //로그인 실패 전달
		return;
	}
	std::cout << "성공" << std::endl;
	RoomManager::getInstance().GetAllUserNum(sendPacket);
	isConnected = true;
	strcpy(name, tempname);                                               //유저 이름 변경
	location = LOBBY;                                      //유저 위치 변경
	/* LENGTH / PROTOCOL / 각방유저수 */
	Write(sendPacket);  //유저에게 각 방 인원 전달
}

VOID User::PROC_ROOM_ENTER(Packet& packet)
{
	//유저의 방 입장
	/* LENGTH / PROTOCOL / ROOMNUM / NAMELENG / NAME */
	INT roomNum;
	packet.ReadINT(&roomNum);

	Packet sendPacket; 
	packet.SetHeader(PT_ROOM_USERINC);
	DWORD tempnameLeng;
	CHAR tempname[32];
	
	packet.ReadDWORD(&tempnameLeng);
	packet.ReadString(tempname, tempnameLeng);

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket);  //방에 있는 유저들에게 입장한 유저의 이름 전달

	sendPacket.PacketClear();
	sendPacket.SetHeader(PT_ROOM_ENTER);

	INT RoomUserNum = RoomManager::getInstance().GetUserNum(roomNum);
	sendPacket.WriteINT(RoomUserNum);

	UserManager::getInstance().GetRoomUsersName(roomNum, sendPacket);

	Write(sendPacket);           //방에 있는 유저들의 이름 정보 전달
	location = roomNum;
	RoomManager::getInstance().UserNumInc(roomNum);
}

VOID User::PROC_USER_CHAT(Packet& packet)
{
	//유저가 채팅 보냈을때
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

	UserManager::getInstance().WriteRoomUsers(roomNum, sendPacket);  //유저들에게 채팅 보낸 유저 정보와 채팅 내용 전달
}