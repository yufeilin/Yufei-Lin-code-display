using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;
using UnityEngine.UI;

// 信息发送请求 数据结构
public class MessageToBeSent
{
    public MessageToBeSent(string NewMessageString, float NewSendInterval = 0.5f)
    {
        MessageString = NewMessageString;
        SendInterval = NewSendInterval;
    }

    // 待发送信息的信息字符串
    string MessageString = "";

    // 当前信息发送请求是否活跃
    bool Enable = true;

    // 当前距离该信息发送请求上次发送已经过去的时间
    float TimeFromLastSend = 0.0f;

    // 消息发送间隔
    float SendInterval = 0.5f;

    public string GetMessageString() { return MessageString; }
    public bool GetEnable() { return Enable; }
    public void SetEnable(bool NewEnable) { Enable = NewEnable; }
    public void TimeFromLastSendOffset(float TimeOffset) { TimeFromLastSend += TimeOffset; }
    public bool CheckNeedSend() { return (TimeFromLastSend >= SendInterval) ? true : false; }
    public void ClearTimeFromLastSend() { TimeFromLastSend = 0.0f; }
}


public class NetworkController : MonoBehaviour {

    // 客户端最高权威

    // 客户端数据包接收线程
    Thread ReceiveThread = null;

    int CurrentPlayerID = -1;


    // 是否在建立Socket连接标旗
    // 当网络控制器的SocketConnect函数开始执行时此标旗会被置true
    // 当网络控制器的SocketConnect函数执行结束时此标旗会被置false
    public bool IsSocketConnecting = false;

    // Socket连接过程完成标旗
    // 当网络控制器的SocketConnect函数运行完毕时会将该标旗置true以通知登陆控制器Socket连接过程已经完成
    // 这个标旗只反应连接过程的完成并不能反应连接过程是否成功
    public bool SocketConnectingComplete = false;

    // Socket连接成功标旗
    // 当网络控制器的SocketConnect函数运行完毕时会将该标旗置true或false
    // IsSocketConnected == true 意味着网络控制器建立Socket成功，登陆控制器可以进行下一步操作
    // IsSocketConnected == false 意味着网络控制器建立Socket失败
    public bool IsSocketConnected = false;

    // 链接建立标识符
    bool HasConnected = false;

    Socket SocketSend;

    List<string> ReceiveMessage = new List<string>();

    // 管理器注册表
    // 需要接收数据包的管理器需要提前通过调用网络控制器中的函数进行注册
    // 管理器只有注册后网络控制器才能够通过分发机制将数据包分发到该管理器
    Dictionary<string, Action<string>> ManagerRegistry = new Dictionary<string, Action<string>>();

    // 信息发送请求链表
    List<MessageToBeSent> MessageInSendList = new List<MessageToBeSent>();

    // 待删除信息发送请求位于信息发送请求链表的索引值链表
    List<int> RemoveMessageIndexList = new List<int>();

    int GetCurrentPlayerID()
    {
        return CurrentPlayerID;
    }

    // 管理器在网络控制器中注册自己以便接收数据包
    public void ManagerRegistion(string ManagerName, Action<string> MessageReceiveFunction)
    {
        if (ManagerRegistry.ContainsKey(ManagerName))
        {
            ManagerRegistry[ManagerName] = MessageReceiveFunction;
        }
        else
        {
            ManagerRegistry.Add(ManagerName, MessageReceiveFunction);
        }
    }

    // 管理器在网络控制器中注销自己以便取消数据包的接收
    public void ManagerCancellation(string ManagerName)
    {
        if (ManagerRegistry.ContainsKey(ManagerName))
        {
            ManagerRegistry.Remove(ManagerName);
        }
    }

    // 将一条消息加入信息发送区并将该发送信息数据结构返回给请求的管理器
    public MessageToBeSent AddMessageInSendList(string AddMessageString)
    {
        MessageToBeSent NewMessageToBeSent = new MessageToBeSent(AddMessageString);
        MessageInSendList.Add(NewMessageToBeSent);
        return NewMessageToBeSent;
    }

    // 将一条信息从信息发送区中移除
    public void RemoveMessageInSendList(string RemoveMessageString)
    {
        int RemoveIndex = -1;

        int n = MessageInSendList.Count;
        for(int i=0;i<n;i++)
        {
            if(MessageInSendList[i].GetMessageString() == RemoveMessageString)
            {
                RemoveIndex = i;
            }
        }

        if(RemoveIndex >= 0)
        {
            MessageInSendList.Remove(MessageInSendList[RemoveIndex]);
        }
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

                string[] split = str.Split(' ');

                // 将接收信息添加到数据包处理队列中
                ReceiveMessage.Add(str);

            }
            catch { }
        }
    }

    public void MessageSend(string str)
    {
        Debug.Log(str);

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

    // 启动Socket链接
    public bool SocketConnect()
    {
        bool Result = false;

        try
        {
            int _port = 8888;
            string _ip = "10.25.196.89";

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
            Result = true;
            HasConnected = true;
        }
        catch (Exception)
        {
            Result = false;
            Debug.Log("IP或者端口号错误...");
        }

        return Result;
    }

    public void AsynSocketConnect()
    {
        // 将正在进行连接建立标旗置true
        IsSocketConnecting = true;
        // 将连接建立过程完成标旗置false
        SocketConnectingComplete = false;

        // 获取连接建立成功标旗的值
        IsSocketConnected = SocketConnect();

        // 将连接建立过程完成标旗置true
        SocketConnectingComplete = true;
        // 将正在进行连接建立标旗置false
        IsSocketConnecting = false;
    }

    // 关闭Socket链接
    public void SocketDisconnect()
    {
        if(HasConnected)
        {
            // 通知服务器断开连接
            MessageSend("DisConnect ");

            // 关闭数据包接收进程
            if(ReceiveThread!=null){ ReceiveThread.Abort(); }            

            // 关闭Socket
            SocketSend.Close();
        }
    }

    void ReceiveMessageDistribution()
    {
        GameObject LogInManager = GameObject.Find("LogInManager");

        int n = ReceiveMessage.Count;
        for(int i=0;i<n;i++)
        {
            string[] MessageSplit = ReceiveMessage[i].Split(' ');

            if(ManagerRegistry.ContainsKey(MessageSplit[0]))
            {
                ManagerRegistry[MessageSplit[0]](ReceiveMessage[i]);
            }

            if(MessageSplit[0] == "LogInController")
            {
                LogInManager.GetComponent<LogInController>().MessageReceive(ReceiveMessage[i]);
            }
        }

        ReceiveMessage.Clear();
    }

    void SendMessageInMessageInSendList()
    {
        int n = MessageInSendList.Count;

        // 遍历所有的信息发送请求
        for(int i=0;i<n;i++)
        {
            if(MessageInSendList[i].GetEnable())
            {
                // 如果当前信息发送请求处于活跃状态则更新该信息发送请求中的计时器并检查是否发送

                // 累加信息发送请求中的计时器时间
                MessageInSendList[i].TimeFromLastSendOffset(Time.deltaTime);
                if (MessageInSendList[i].CheckNeedSend())
                {
                    // 如果计时器时间大于等于发送间隔则立即发送当前消息
                    MessageSend(MessageInSendList[i].GetMessageString());
                    // 发送完毕后清空计时器时间
                    MessageInSendList[i].ClearTimeFromLastSend();


                }
            }
            else
            {
                // 如果当前信息发送请求处于不活跃状态则将该信息发送请求的索引放入待删除链表中
                RemoveMessageIndexList.Add(i);
            }
        }

        n = RemoveMessageIndexList.Count;

        // 删除所有不活跃的信息发送请求
        for (int i=0;i<n;i++)
        {
            MessageInSendList[RemoveMessageIndexList[i]] = null;
            MessageInSendList.RemoveAt(RemoveMessageIndexList[i]);
        }

        // 清空不活跃信息发送请求索引数组
        RemoveMessageIndexList.Clear();
    }

    // Use this for initialization
    void Start()
    {
        // 这个网络控制器所在的空物体是整个游戏周期都存在的所以它不会因为场景的载入而被销毁
        // 使用下面这句可以将该脚本附着的物体放入一个全局场景中，该场景不会因为场景载入而销毁
        GameObject.DontDestroyOnLoad(gameObject);

        //SocketInitialize();
    }

    // Update is called once per frame
    void Update()
    {
        // 每帧处理接收数据包分发
        ReceiveMessageDistribution();

        // 每帧处理发送链表中的待发送数据包
        SendMessageInMessageInSendList();
    }

    void OnDestroy()
    {
        MessageSend("DisConnect ");

        if(ReceiveThread != null) { ReceiveThread.Abort(); }
        
        SocketSend.Close();
    }
}
