using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Xml;     //xml操作
using System.IO;     //file操作

public class GameRoomController : MonoBehaviour
{
    GameObject NetworkManager = null;

    GameObject HintMessageManager = null;

    GameObject ClientPlayerManager = null;

    ClientPlayerController MainClientPlayerController = null;

    MessageToBeSent CurrentMessageToBeSent;

    List<RoomPlayerSlot> RoomPlayerSlotList = new List<RoomPlayerSlot>();

    // 该游戏房间的地图ID
    string GameMapID = "NotAssign";

    // 当前客户端用户所处游戏房间的房间ID
    int GameRoomID = -1;

    // 这个是当前客户端用户的玩家ID，就是服务器端玩家槽位的槽位ID，也是进入游戏后的玩家ID
    // 这个玩家ID是与玩家槽位ID绑定的，玩家更换槽位会变更玩家ID
    int PlayerID = -1;

    bool IsGameRoomMaster = false;

    // 设置游戏房间的地图ID
    public void SetGameMapID(string NewGameMapID) { GameMapID = NewGameMapID; }

    // 获取游戏房间的地图ID
    public string GetGameMapID() { return GameMapID; }

    // 设置游戏房间ID
    public void SetGameRoomID(int NewGameRoomID)
    {
        GameRoomID = NewGameRoomID;
    }

    // 获取游戏房间ID
    public int GetGameRoomID()
    {
        return GameRoomID;
    }

    // 设置当前客户端用户的玩家ID
    public void SetPlayerID(int NewPlayerID) { PlayerID = NewPlayerID; }

    // 获取当前客户端用户的玩家ID
    public int GetPlayerID() { return PlayerID; }

    // 设置房主标旗
    public void SetMasterFlag(bool NewMasterFlag)
    {
        IsGameRoomMaster = NewMasterFlag;
    }

    // 获取房主标旗
    public bool GetMasterFlag()
    {
        return IsGameRoomMaster;
    }

    public int StringToInt(string s)
    {
        float Tmp = float.Parse(s);

        return (int)Tmp;
    }

    public void LoadRoomConfigFromXML(string MapID)
    {
        string filePath = Application.dataPath + "/Resources/RoomConfig/" + MapID + "_RoomConfig.xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("RoomConfig").ChildNodes;

            int RoomPlayerSlotIndex = 0;

            foreach (XmlElement SlotInfo in node)
            {
                RoomPlayerSlot NewRoomPlayerSlot = new RoomPlayerSlot();

                NewRoomPlayerSlot.SetSlotID(RoomPlayerSlotIndex);

                foreach (XmlElement SlotInfoItem in SlotInfo.ChildNodes)
                {
                    if (SlotInfoItem.Name == "SlotType")
                    {
                        if(SlotInfoItem.InnerText == "AI")
                        {
                            Debug.Log("this is AI");
                            RoomPlayer NewRoomPlayer = new RoomPlayer();
                            NewRoomPlayer.SetPlayerName(SlotInfoItem.InnerText);
                            NewRoomPlayerSlot.SetContainRoomPlayer(NewRoomPlayer);
                        }                       
                    }
                    else if (SlotInfoItem.Name == "SlotTeamID")
                    {
                        NewRoomPlayerSlot.SetTeamID(StringToInt(SlotInfoItem.InnerText));
                    }
                }

                RoomPlayerSlotList.Add(NewRoomPlayerSlot);

                RoomPlayerSlotIndex++;
            }
        }

        for(int i=0;i< RoomPlayerSlotList.Count;i++)
        {
            if(RoomPlayerSlotList[i].GetContainRoomPlayer() == null)
            {
                RoomPlayer NewRoomPlayer = new RoomPlayer();
                NewRoomPlayer.SetPlayerName("YOU");
                RoomPlayerSlotList[i].SetContainRoomPlayer(NewRoomPlayer);
                break;
            }
        }

        
    }

    public void RefreashRoomDisplay()
    {
        GameObject RoomPlayerSlotBackgroundPrefab = (GameObject)Resources.Load("Prefabs/UIPrefabs/RoomPlayerSlotBackground");
        GameObject RoomPlayerBackgroundPrefab = (GameObject)Resources.Load("Prefabs/UIPrefabs/RoomPlayerBackground");



        for (int i = 0; i < RoomPlayerSlotList.Count; i++)
        {
            GameObject CurrentRoomPlayerSlotBackground = GameObject.Instantiate(RoomPlayerSlotBackgroundPrefab);

            CurrentRoomPlayerSlotBackground.GetComponent<RectTransform>().SetParent(GameObject.Find("Canvas").transform);
            CurrentRoomPlayerSlotBackground.GetComponent<RectTransform>().localPosition = new Vector3(-135, 150 - 55 *i, 0);

            if(RoomPlayerSlotList[i].GetContainRoomPlayer()!=null)
            {
                GameObject CurrentRoomPlayerBackground = GameObject.Instantiate(RoomPlayerBackgroundPrefab);

                CurrentRoomPlayerBackground.GetComponent<RectTransform>().SetParent(GameObject.Find("Canvas").transform);
                CurrentRoomPlayerBackground.GetComponent<RectTransform>().localPosition = new Vector3(-135, 150 - 55 * i, 0);

                GameObject CurrentPlayerNameText = CurrentRoomPlayerBackground.transform.Find("PlayerNameText").gameObject;

                CurrentPlayerNameText.GetComponent<Text>().text = RoomPlayerSlotList[i].GetContainRoomPlayer().GetPlayerName();
            }
        }

        // 给游戏房间场景的开始游戏按钮添加点击事件
        GameObject StartGameButton = GameObject.Find("StartButton");
        StartGameButton.GetComponent<Button>().onClick.AddListener(OnClickStartGameButton);
    }

    public void MessageReceive(string ReceiveString)
    {
        string[] StringSplit = ReceiveString.Split(' ');

        if(StringSplit[1] == "RoomOrder")
        {
            if(StringSplit[2] == "RoomStart")
            {
                if (StringSplit[3] == "Success")
                {
                    Debug.Log("Room Start confirm.");

                    // 隐藏 正在连接 提示信息
                    HintMessageManager.GetComponent<HintMessageController>().ConcealHintMessage();

                    // 将购买信息发送请求设为不活跃以便让网络控制器停止发送该信息并在下一个帧循环中删除该信息发送请求
                    CurrentMessageToBeSent.SetEnable(false);

                    // 显示 游戏马上开始 提示信息
                    HintMessageManager.GetComponent<HintMessageController>().ShowHintMessage("游戏马上开始", 3.0f);

                    MainClientPlayerController.SetGameMapID(GameMapID);

                    // 开始切换到房间场景
                    LoadSceneConfig.nextSceneName = "MainGamePlay";
                    SceneManager.LoadScene("Loading");//加载进度条场景
                }
            }
        }
    }

    public void OnClickStartGameButton()
    {

        Debug.Log("Start Game button pressed.");

        // 生成发送信息
        string SendMessage = "RoomOrder " + GameRoomID.ToString() +" RoomStart ";

        // 通知网络管理器发送信息
        CurrentMessageToBeSent = NetworkManager.GetComponent<NetworkController>().AddMessageInSendList(SendMessage);

        // 显示 正在连接 提示信息
        HintMessageManager.GetComponent<HintMessageController>().ShowHintMessage("Connecting");
    }

    public void OnClickReadyGameButton()
    {

    }

    // Use this for initialization
    void Start ()
    {
        // 使用下面这句可以将该脚本附着的物体放入一个全局场景中，该场景不会因为场景载入而销毁
        GameObject.DontDestroyOnLoad(gameObject);

        // 找到网络管理器的引用
        NetworkManager = GameObject.Find("NetworkManager");

        // 找到提示信息管理器的引用
        HintMessageManager = GameObject.Find("HintMessageManager");

        ClientPlayerManager = GameObject.Find("ClientPlayerManager");

        MainClientPlayerController = ClientPlayerManager.GetComponent<ClientPlayerController>();

        // 向网络管理器注册自己的数据包处理函数
        NetworkManager.GetComponent<NetworkController>().ManagerRegistion("GameRoomController", MessageReceive);

        Debug.Log("Enter GameRoom successful.");
    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
