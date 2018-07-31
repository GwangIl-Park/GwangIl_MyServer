using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class LobbyCtrl : MonoBehaviour {
    public List<Text> roomList = new List<Text>();
    public void RoomMemberInit(byte[] m_recvBuf)
    {
        for(int i=0;i<6;i++)
        {
            roomList[i].text =  "Room"+(i+1)+"\n현재 인원 : "+BitConverter.ToInt32(m_recvBuf, 8 + 4 * i).ToString();
        }
    }
}
