서버 직접 만들기! 온라인 게임 서버 프로그래밍 (한동훈 저) 참고

Version1.01 서버의 기본 틀 제작 ServerIOCP, Session, SessionManager, Packet클래스(2018.07.16)

Version1.02 접속하고 메세지보내는 간단한 클라이언트 제작 후 접속과 메세지 받아지는지 테스트 (SessionManager에 객체 연결안한다던지 wsarecv의 flag초기화 안한다던지 등 잔실수 처리) (2018.07.18)

Version1.03 Room, RoomManager,User(Session 자식 클래스), UserManager 클래스 제작, MySql에 연결하고 insert되는 것 (2018.07.20)

Version1.04 유저 등록하면 DB에 저장하는 과정 추가, 패킷 받아서 처리하고 버퍼 비우는 과정 작업 (2018.07.22)

Version1.05 회원가입, 로그인 실패시 동작 성공시 동작 등 버그 수정, 클라이언트(유니티) 제작 //깃허브때문에 프로젝트 ...(2018.07.31)

Version1.06 유저 방 입장, 채팅 기능 
