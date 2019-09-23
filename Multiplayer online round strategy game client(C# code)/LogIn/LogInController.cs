using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Threading;
using UnityEngine.SceneManagement;

public class LogInController : MonoBehaviour
{
    public GameObject NetworkManager = null;

    public GameObject LogInMessageManager = null;

    public GameObject UserNameText = null;

    public GameObject UserPinText = null;

    public InputField UserPinTextInput = null;

    // Socket建立线程
    Thread SocketConnectThread = null;

    // 登录按钮可用标旗
    bool LogInButtonAvaliable = true;

    // 是否在建立Socket连接标旗
    // 当网络控制器的SocketConnect函数开始执行时此标旗会被置true
    // 当网络控制器的SocketConnect函数执行结束时此标旗会被置false
    bool IsSocketConnecting = false;

    // Socket连接过程完成标旗
    // 当网络控制器的SocketConnect函数运行完毕时会将该标旗置true以通知登陆控制器Socket连接过程已经完成
    // 这个标旗只反应连接过程的完成并不能反应连接过程是否成功
    bool SocketConnectingComplete = false;

    // Socket连接成功标旗
    // 当网络控制器的SocketConnect函数运行完毕时会将该标旗置true或false
    // IsSocketConnected == true 意味着网络控制器建立Socket成功，登陆控制器可以进行下一步操作
    // IsSocketConnected == false 意味着网络控制器建立Socket失败
    bool IsSocketConnected = false;

    // 是否在发送验证请求标旗
    bool IsSendingRequest = false;

    // 已经接收到服务器回复的验证结果信息标旗
    bool ValidationReceived = false;

    // 距离上一次验证请求发送过去的时间
    float TimeFromLastRequestSend = 0.0f;

    private string userName;//用户名
    private string userPassword;//密码
    private string info;//信息

    // 用户账号密码验证请求字符串
    private string ValidationRequest = "";

    // 验证请求ID
    int ValidationRequestID = 0;

    // 当前正在验证的用户名
    string UserName = "";

    // 当前正在验证的用户密码
    string UserPin = "";

    // 
    bool SendRequestMessage()
    {
        GameObject NetworkManager = GameObject.Find("NetworkManager");

        NetworkManager.GetComponent<NetworkController>().MessageSend(ValidationRequest);

        return true;
    }

   

    public void OnClickLogInButton()
    {
        if(LogInButtonAvaliable)
        {
            // 重置验证结果接收标旗
            ValidationReceived = false;
            // 递增请求ID
            ValidationRequestID++;

            // 保存用户名
            UserName = UserNameText.GetComponent<Text>().text;
            // 保存密码
            UserPin = userPassword;

            Debug.Log(UserName);

            Debug.Log(userPassword);

            // 本地用户名和密码格式判断
            if (UserName == "") { return; }
            if (UserPin == "") { return; }

            ValidationRequest = "ValidationRequest " + ValidationRequestID.ToString() +" "+ UserName + " " + UserPin + " ";

            // 禁用登录按钮
            LogInButtonAvaliable = false;

            // 显示正在链接UI
            LogInMessageManager.GetComponent<HintMessageController>().ShowHintMessage("Connecting");

            // 开启一个建立Socket连接的线程尝试与账号验证服务器取得联系
            SocketConnectThread = new Thread(NetworkManager.GetComponent<NetworkController>().AsynSocketConnect);
            SocketConnectThread.IsBackground = true;
            SocketConnectThread.Start();           
        }
    }

    public void OnClickExitButton()
    {
        #if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;                
        #else
            Application.Quit();
        #endif
    }

    public void MessageReceive(string ReceiveString)
    {
        if(!ValidationReceived)
        {
            // 接收信息格式：
            // LogInManager RequestID UserName UserPin Success/Fail 

            string[] split = ReceiveString.Split(' ');

            // 确保验证结果信息是针对当前待验证账号的当前（通过RequestID来检测这个信息不是旧的验证结果）验证结果
            if(split[1] == ValidationRequestID.ToString() && split[2] == UserName && split[3] == UserPin)
            {

                if (split[4] == "Success")
                {
                    ValidationReceived = true;

                    Debug.Log("Loading Main Game scene.");
                    // 显示登陆成功提示框
                    // 跳转到游戏主界面
                    LogInMessageManager.GetComponent<HintMessageController>().ShowHintMessage("登陆成功", 5.0f);

                    LoadSceneConfig.nextSceneName = "MainMenu";
                    SceneManager.LoadScene("Loading");//加载进度条场景
                }
                else
                {
                    ValidationReceived = true;

                    // 关闭Socket连接
                    NetworkManager.GetComponent<NetworkController>().SocketDisconnect();

                    // 显示用户名或者密码不正确提示框
                    LogInMessageManager.GetComponent<HintMessageController>().ShowHintMessage("用户名或密码错误", 5.0f);
                }

                // 关闭认证请求发送
                IsSendingRequest = false;

                // 重置发送控制时间
                TimeFromLastRequestSend = 0.0f;

                // 启用登录按钮
                LogInButtonAvaliable = true;
            }

            
        }        
    }

	// Use this for initialization
	void Start ()
    {
        GameObject NetworkManager = GameObject.Find("NetworkManager");

        UserPinTextInput.contentType = InputField.ContentType.Password;

        //初始化
        userName = "";
        userPassword = "";
        info = "";

    }
	
	// Update is called once per frame
	void Update ()
    {
        if(IsSendingRequest)
        {
            // 如果处于验证请求发送状态则每0.2秒发送一次用户名密码验证请求
            TimeFromLastRequestSend += Time.deltaTime;

            if(TimeFromLastRequestSend >= 0.2f)
            {
                SendRequestMessage();
                TimeFromLastRequestSend = 0.0f;
            }
        }

        // 从网络控制器获取状态标旗
        IsSocketConnecting = NetworkManager.GetComponent<NetworkController>().IsSocketConnecting;
        SocketConnectingComplete = NetworkManager.GetComponent<NetworkController>().SocketConnectingComplete;
        IsSocketConnected = NetworkManager.GetComponent<NetworkController>().IsSocketConnected;

        if (SocketConnectingComplete)
        {
            // 关闭Socket连接建立线程
            if (SocketConnectThread != null) { SocketConnectThread.Abort(); }

            if (IsSocketConnected)
            {
                IsSendingRequest = true;
            }
            else
            {
                // 关闭正在链接UI
                LogInMessageManager.GetComponent<HintMessageController>().ConcealHintMessage();

                // 显示连接错误UI
                LogInMessageManager.GetComponent<HintMessageController>().ShowHintMessage("Checking network", 3.0f);

                // 启用登录按钮
                LogInButtonAvaliable = true;

                Debug.Log("Soccket connet fail.");
            }

            NetworkManager.GetComponent<NetworkController>().SocketConnectingComplete = false;
        }
    }

    void OnGUI()
    {
        GUIStyle fontStyle = new GUIStyle();
        //fontStyle.normal.background = null;    //设置背景填充
        fontStyle.normal.textColor = new Color(1, 0, 0);   //设置字体颜色
        fontStyle.fontSize = 16;       //字体大小

        userPassword = GUI.PasswordField(new Rect(Screen.width/2-100, Screen.height/2+115, 200, 30), userPassword, '*', fontStyle);//'*'为密码遮罩                                                                                               
    }

    void OnDestroy()
    {
        //MessageSend("DisConnect ");

        //ReceiveThread.Abort();

        if(SocketConnectThread != null) { SocketConnectThread.Abort(); }

        //SocketSend.Close();
    }
}
