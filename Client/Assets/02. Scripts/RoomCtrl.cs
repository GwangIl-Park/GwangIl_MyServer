using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Text;
using System;

public class RoomCtrl : MonoBehaviour {
    
    public Text roomNum;
    public List<Text> members = new List<Text>();
    public Text chat;
    private int roomUserNum;

    public InputField inputsend;
    public Button sendBtn;

    public GameObject MainObject;
    private MainCtrl mainCtrl;
    void Awake()
    {
        mainCtrl = MainObject.GetComponent<MainCtrl>();
    }

    public void RoomInit(byte[] m_recvBuf)
    {
        roomUserNum = BitConverter.ToInt32(m_recvBuf, 8);
        int totalNameLeng = 0;
        for(int i=0;i<roomUserNum;i++)
        {
            int tempNameLeng = BitConverter.ToInt32(m_recvBuf, 12 + totalNameLeng);
            members[i].text = Encoding.UTF8.GetString(m_recvBuf, 16 + totalNameLeng, tempNameLeng);
            totalNameLeng += (sizeof(int) + tempNameLeng);
        }
        members[roomUserNum++].text = mainCtrl.GetUserName();
    }

    public void RoomUserInc(byte[] m_recvBuf)
    {
        int tempNameLeng = BitConverter.ToInt32(m_recvBuf, 8);
        members[roomUserNum++].text = Encoding.UTF8.GetString(m_recvBuf, 12, tempNameLeng);
    }

    public void OnClickSend()
    {
        string message = inputsend.text;
        int messageLeng = message.Length;

        int tempSendLen = (sizeof(int) * 3) + messageLeng;
        byte[] tempSendBuf = new byte[4096];

        Buffer.BlockCopy(BitConverter.GetBytes(tempSendLen), 0, tempSendBuf, 0, sizeof(int));
        Buffer.BlockCopy(BitConverter.GetBytes(Protocol.PT_USER_CHAT), 0, tempSendBuf, sizeof(int), sizeof(int));
        Buffer.BlockCopy(BitConverter.GetBytes(messageLeng), 0, tempSendBuf, sizeof(int) * 2, sizeof(int));
        Buffer.BlockCopy(Encoding.UTF8.GetBytes(message), 0, tempSendBuf, sizeof(int) * 3, messageLeng);

        Debug.Log(mainCtrl.GetSocket().Send(tempSendBuf, tempSendLen, 0));
    }

    public void RecvChat(byte[] m_recvBuf)
    {
        int usernameLeng = BitConverter.ToInt32(m_recvBuf, 8);
        string username = Encoding.UTF8.GetString(m_recvBuf, 8 + sizeof(int), usernameLeng);
        int messageLeng = BitConverter.ToInt32(m_recvBuf, 8 + sizeof(int) + usernameLeng);
        string message = Encoding.UTF8.GetString(m_recvBuf, 8 + sizeof(int) + sizeof(int) + usernameLeng, messageLeng);
        chat.text += "\n[" + username + "] : " + message;

        inputsend.text = null;
    }
}
