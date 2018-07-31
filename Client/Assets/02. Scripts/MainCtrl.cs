using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System.Net;
using System;

public static class Protocol
{
    public const int PT_REG_USER = 1000;
    public const int PT_USER_LOGIN = 1001;
}

public class MainCtrl : MonoBehaviour {
    private Socket socket;
    
    byte[] RecvBuf = new byte[4096];
    private int RecvLen = 0;

    public GameObject LoginObject;
    private LoginCtrl loginCtrl;
    public GameObject LobbyObject;
    private LobbyCtrl lobbyCtrl;

    void Start()
    {
        lobbyCtrl = LobbyObject.GetComponent<LobbyCtrl>();
        loginCtrl = LoginObject.GetComponent<LoginCtrl>();
        LobbyObject.SetActive(false);
        SocketStart();
    }

    public void SocketStart()
    {
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        socket.NoDelay = true;
    }

    public IEnumerator RecvMsg()
    {
        while (true)
        {
            if (socket.Available > 0)
            {
                byte[] recvPackBuf = new byte[4096];
                int recvPackLen = socket.Receive(recvPackBuf, 0);
                if (recvPackLen > 0)
                {
                    Buffer.BlockCopy(recvPackBuf, 0, RecvBuf, RecvLen, recvPackLen);
                    RecvLen += recvPackLen;
                    while (true)
                    {
                        int packLen = BitConverter.ToInt32(RecvBuf, 0);

                        if (packLen > 0 && RecvLen >= packLen)
                        {
                            ParsePacket();
                            RecvLen -= packLen;
                            Buffer.BlockCopy(RecvBuf, packLen, RecvBuf, 0, RecvLen);
                        }
                        else
                        {
                            break;
                        }
                    }

                }
            }
            yield return null;
        }
    }

    public void ParsePacket()
    {
        int protocol = BitConverter.ToInt32(RecvBuf, sizeof(int));
        switch (protocol)
        {
            case Protocol.PT_REG_USER:
                loginCtrl.PROC_PT_REG_USER(RecvBuf);
                break;
            case Protocol.PT_USER_LOGIN:
                loginCtrl.PROC_PT_USER_LOGIN(RecvBuf);
                break;
            default:
                break;
        }
    }

    public void LoginToLobby()
    {
        LoginObject.SetActive(false);
        LobbyObject.SetActive(true);
        lobbyCtrl.RoomMemberInit(RecvBuf);
    }

    public Socket GetSocket()
    {
        return socket;
    }

    public void RestartSocket()
    {
        StopCoroutine(RecvMsg());
        socket.Shutdown(SocketShutdown.Both);
        socket.Close();
        socket = null;
        SocketStart();
    }
}
