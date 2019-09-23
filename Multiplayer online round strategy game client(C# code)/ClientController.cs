using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;

public class ClientController : MonoBehaviour
{
    // 客户端最高权威

    // 客户端数据包接收线程
    Thread ReceiveThread = null;

    int CurrentPlayerID = -1;

    Socket SocketSend;

    int GetCurrentPlayerID()
    {
        return CurrentPlayerID;
    }


    private void SocketInitialize()
    {
        try
        {
            int _port = 8888;
            string _ip = "10.25.154.111";

            //创建客户端Socket，获得远程ip和端口号
            SocketSend = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress ip = IPAddress.Parse(_ip);
            IPEndPoint point = new IPEndPoint(ip, _port);

            SocketSend.Connect(point);
            Debug.Log("连接成功!");
            //开启新的线程，不停的接收服务器发来的消息
            ReceiveThread = new Thread(MessageReceive);
            ReceiveThread.IsBackground = true;
            ReceiveThread.Start();
        }
        catch (Exception)
        {
            Debug.Log("IP或者端口号错误...");
        }

    }

    void MessageReceive()
    {
        while (true)
        {
            try
            {
                byte[] buffer = new byte[1024 * 1024 * 3];
                //实际接收到的有效字节数
                int len = SocketSend.Receive(buffer);
                if (len == 0)
                {
                    break;
                }
                string str = Encoding.UTF8.GetString(buffer, 0, len);
                Debug.Log("客户端打印：" + SocketSend.RemoteEndPoint + ":" + str);
            }
            catch { }
        }
    }

    private void MessageSend(string str)
    {
        try
        {
            string msg = str;
            byte[] buffer = new byte[1024 * 1024 * 3];
            buffer = Encoding.UTF8.GetBytes(msg);
            Debug.Log("before send.");
            SocketSend.Send(buffer);
            Debug.Log("after send.");
        }
        catch (Exception ex)
        {
            Debug.Log(ex);
            Debug.Log("Send Fail.");
        }
    }

    public void PieceMoveRequest()
    {
        MessageSend("PieceMoveRequest");
    }

    // Use this for initialization
    void Start ()
    {
        SocketInitialize();
    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    void OnDestroy()
    {
        MessageSend("DisConnect ");

        ReceiveThread.Abort();

        SocketSend.Close();
    }
}
