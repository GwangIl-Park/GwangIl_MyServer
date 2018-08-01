using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class LobbyCtrl : MonoBehaviour
{
    public GameObject MainObject;
    private MainCtrl mainCtrl;

    public List<Button> roomList = new List<Button>();

    void Awake()
    {
        mainCtrl = MainObject.GetComponent<MainCtrl>();
    }

    public void RoomMemberInit(byte[] m_recvBuf)
    {
        for (int i = 0; i < 6; i++)
        {
            roomList[i].transform.GetChild(0).GetComponent<Text>().text = "Room" + (i + 1) + "\n현재 인원 : " + BitConverter.ToInt32(m_recvBuf, 8 + 4 * i).ToString();
        }
    }

    public void OnClickRoomEnter(Button m_button)
    {
        int tempSendLen = (sizeof(int) * 3);
        byte[] tempSendBuf = new byte[4096];
        Buffer.BlockCopy(BitConverter.GetBytes(tempSendLen), 0, tempSendBuf, 0, sizeof(int));
        Buffer.BlockCopy(BitConverter.GetBytes(Protocol.PT_ROOM_ENTER), 0, tempSendBuf, sizeof(int), sizeof(int));
        for (int i=0;i<6;i++)
        {
            if(roomList[i]==m_button)
            {
                Buffer.BlockCopy(BitConverter.GetBytes(i), 0, tempSendBuf, sizeof(int) * 2, sizeof(int));
                mainCtrl.GetSocket().Send(tempSendBuf, tempSendLen, 0);
            }
        }
    }
}