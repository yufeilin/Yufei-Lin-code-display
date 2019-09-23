using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SinglePlayerModeController : MonoBehaviour
{

    GameObject NetworkManager = null;

    GameObject HintMessageManager = null;

    GameObject RoomManagerPrefab = null;
    GameObject CurrentRoomManager = null;

    MessageToBeSent CurrentMessageToBeSent;

    int StringToInt(string s)
    {
        float Tmp = float.Parse(s);

        return (int)Tmp;
    }

    public void MessageReceive(string ReceiveString)
    {
        Debug.Log("&&&&"+ReceiveString+ "&&&&");

        string[] StringSplit = ReceiveString.Split(' ');

        if(StringSplit[1] == "UISPR" && StringSplit[3] == "Success")
        {
            // SinglePlayerModeController UISPR FreedomDay Success 1234 0

            // 新建一个房间管理器实例并传入地图ID

            // 隐藏 正在连接 提示信息
            HintMessageManager.GetComponent<HintMessageController>().ConcealHintMessage();

            // 将购买信息发送请求设为不活跃以便让网络控制器停止发送该信息并在下一个帧循环中删除该信息发送请求
            CurrentMessageToBeSent.SetEnable(false);

            // 显示 房间建立成功 提示信息
            HintMessageManager.GetComponent<HintMessageController>().ShowHintMessage("房间建立成功", 3.0f);

            // 设置游戏房间的地图ID
            CurrentRoomManager.GetComponent<GameRoomController>().SetGameMapID(StringSplit[2]);

            // 将获取的房间ID号设置给房间管理器
            CurrentRoomManager.GetComponent<GameRoomController>().SetGameRoomID(StringToInt(StringSplit[4]));

            // 将获取的玩家ID号设置给房间管理器
            CurrentRoomManager.GetComponent<GameRoomController>().SetPlayerID(StringToInt(StringSplit[5]));

            // 开始切换到房间场景
            LoadSceneConfig.nextSceneName = "GameRoom";
            SceneManager.LoadScene("Loading");//加载进度条场景

        }
    }

    public void OnClickFreedomDay()
    {
        // 点击关卡按钮后首先生成房间管理器实例并通知房间管理器从XML中获取房间配置信息       
        CurrentRoomManager = GameObject.Instantiate(RoomManagerPrefab);
        CurrentRoomManager.name = "GameRoomManager";
        CurrentRoomManager.GetComponent<GameRoomController>().LoadRoomConfigFromXML("FreedomDay");

        // 将建立房间的当前用户设置为房主
        CurrentRoomManager.GetComponent<GameRoomController>().SetMasterFlag(true);

    
        // 生成发送信息
        string SendMessage = "UISPR FreedomDay ";

        // 通知网络管理器发送信息
        CurrentMessageToBeSent = NetworkManager.GetComponent<NetworkController>().AddMessageInSendList(SendMessage);

        // 显示 正在连接 提示信息
        HintMessageManager.GetComponent<HintMessageController>().ShowHintMessage("Connecting");
    }

    // Use this for initialization
    void Start ()
    {
        // 获取房间管理器预制体引用
        RoomManagerPrefab = (GameObject)Resources.Load("Prefabs/Manager/GameRoomManager");

        // 找到网络管理器的引用
        NetworkManager = GameObject.Find("NetworkManager");

        // 找到提示信息管理器的引用
        HintMessageManager = GameObject.Find("HintMessageManager");

        // 向网络管理器注册自己的数据包处理函数
        NetworkManager.GetComponent<NetworkController>().ManagerRegistion("SinglePlayerModeController", MessageReceive);
    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
