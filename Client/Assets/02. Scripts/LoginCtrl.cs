using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System.Net;
using UnityEngine.UI;
using System.Text;
using System;

public class LoginCtrl : MonoBehaviour
{
    private string MyUserName;
    private int MyUserNameLeng;
    private string MyPassword;
    private int MyPasswordLeng;
    private MainCtrl mainCtrl;

    public InputField _username;
    public InputField _password;
    public Text SucFail;
    public GameObject NetworkObject;

    void Start()
    {
        mainCtrl = NetworkObject.GetComponent<MainCtrl>();
        SucFail.gameObject.SetActive(false);
    }

    public void _Reg()
    {
        mainCtrl.GetSocket().Connect("127.0.0.1", 9190);
        MyUserName = _username.text + '\0';
        MyUserNameLeng = MyUserName.Length;
        MyPassword = _password.text + '\0';
        MyPasswordLeng = MyPassword.Length;
        int tempSendLen = (sizeof(int) * 4 + MyUserNameLeng + MyPasswordLeng);
        byte[] tempSendBuf = new byte[4096];

        Buffer.BlockCopy(BitConverter.GetBytes(tempSendLen), 0, tempSendBuf, 0, sizeof(int));
        Buffer.BlockCopy(BitConverter.GetBytes(Protocol.PT_REG_USER), 0, tempSendBuf, sizeof(int), sizeof(int));
        Buffer.BlockCopy(BitConverter.GetBytes(MyUserNameLeng), 0, tempSendBuf, sizeof(int) * 2, sizeof(int));
        Buffer.BlockCopy(Encoding.UTF8.GetBytes(MyUserName), 0, tempSendBuf, sizeof(int) * 3, MyUserNameLeng);
        Buffer.BlockCopy(BitConverter.GetBytes(MyPasswordLeng), 0, tempSendBuf, sizeof(int) * 3 + MyUserNameLeng, sizeof(int));
        Buffer.BlockCopy(Encoding.UTF8.GetBytes(MyPassword), 0, tempSendBuf, sizeof(int) * 4 + MyUserNameLeng, MyPasswordLeng);

        mainCtrl.GetSocket().Send(tempSendBuf, tempSendLen, 0);
        StartCoroutine(mainCtrl.RecvMsg());
    }

    public void _Connect()
    {
        Socket tempSocket = mainCtrl.GetSocket();
        tempSocket.Connect("127.0.0.1", 9190);
        MyUserName = _username.text + '\0';
        MyUserNameLeng = MyUserName.Length;
        MyPassword = _password.text + '\0';
        MyPasswordLeng = MyPassword.Length;

        int tempSendLen = (sizeof(int) * 4 + MyUserNameLeng + MyPasswordLeng);
        byte[] tempSendBuf = new byte[4096];

        Buffer.BlockCopy(BitConverter.GetBytes(tempSendLen), 0, tempSendBuf, 0, sizeof(int));
        Buffer.BlockCopy(BitConverter.GetBytes(Protocol.PT_USER_LOGIN), 0, tempSendBuf, sizeof(int), sizeof(int));
        Buffer.BlockCopy(BitConverter.GetBytes(MyUserNameLeng), 0, tempSendBuf, sizeof(int) * 2, sizeof(int));
        Buffer.BlockCopy(Encoding.UTF8.GetBytes(MyUserName), 0, tempSendBuf, sizeof(int) * 3, MyUserNameLeng);
        Buffer.BlockCopy(BitConverter.GetBytes(MyPasswordLeng), 0, tempSendBuf, sizeof(int) * 3 + MyUserNameLeng, sizeof(int));
        Buffer.BlockCopy(Encoding.UTF8.GetBytes(MyPassword), 0, tempSendBuf, sizeof(int) * 4 + MyUserNameLeng, MyPasswordLeng);
        
        tempSocket.Send(tempSendBuf, tempSendLen, 0);
        StartCoroutine(mainCtrl.RecvMsg());
    }

    public void PROC_PT_REG_USER(byte[] m_RecvBuf)
    {
        if (BitConverter.ToInt32(m_RecvBuf, 0) == 8)
        {
            SucFail.text = "가입 실패";
            SucFail.gameObject.SetActive(true);
            _username.text = null;
            _password.text = null;
            mainCtrl.RestartSocket();
        }
        else
        {
            mainCtrl.LoginToLobby();
        }
    }

    public void PROC_PT_USER_LOGIN(byte[] m_RecvBuf)
    {
        if (BitConverter.ToInt32(m_RecvBuf, 0) == 8)
        {
            SucFail.text = "로그인 실패";
            SucFail.gameObject.SetActive(true);
            _username.text = null;
            _password.text = null;
            mainCtrl.RestartSocket();
        }
        else
        {
            mainCtrl.LoginToLobby();
        }
    }
}