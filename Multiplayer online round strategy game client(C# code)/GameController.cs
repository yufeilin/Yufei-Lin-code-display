using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameController : MonoBehaviour {

    ClientPlayerController MainClientPlayerController = null;

    int PlayerID = -1;

    string GameMapID = "";

    int FirstRoundPlayerID = -1;

    // 全局网络控制器
    NetworkController MainNetworkController = null;

    // 当前正在发送的信息
    MessageToBeSent CurrentMessageToBeSent;

    public GameObject UIManager = null;

    public GameObject CardDisplayManager = null;

    public GameObject InputManager = null;

    public GameObject MessageManager = null;

    UIController MainUIController = null;
    CardDisplayController MainCardDisplayController = null;
    InputController MainInputController = null;
    MessageController MainMessageController = null;

    

    // 资源管理器
    ResourceManager MainResourceManager = null;

    // 卡牌管理器
    CardManager MainCardManager = null;

    // 外交管理器
    DiplomacyManager MainDiplomacyManager = null;

    // 回合管理器
    RoundManager MainRoundManager = null;

    // 游戏数据库管理器
    LibraryManager MainLibraryManager = null;

    // 环境管理器
    EnvironmentManager MainEnvironmentManager = null;

    // 世界管理器
    WorldManager MainWorldManager = null;

    // 游戏加载器
    GameLoader MainGameLoader = null;

    // 初始手牌管理器
    InitialHandCardManager MainInitialHandCardManager = null;

    int PlayerNum = 2;

    public void MessageReceive(string ReceiveString)
    {
        string[] StringSplit = ReceiveString.Split(' ');


    }

    public int GetPlayerID() { return PlayerID; }

    public UIController GetUIController() { return MainUIController; }
    public CardDisplayController GetCardDisplayController() { return MainCardDisplayController; }
    public InputController GetInputController() { return MainInputController; }
    public MessageController GetMessageController() { return MainMessageController; }

    public ResourceManager GetResourceManager(){ return MainResourceManager; }

    public CardManager GetCardManager() { return MainCardManager; }

    public DiplomacyManager GetDiplomacyManager() { return MainDiplomacyManager; }

    public RoundManager GetRoundManager() { return MainRoundManager; }

    // 获取游戏数据库管理器
    public LibraryManager GetLibraryManager() { return MainLibraryManager; }

    // 获取环境管理器
    public EnvironmentManager GetEnvironmentManager() { return MainEnvironmentManager; }

    // 获取世界管理器
    public WorldManager GetWorldManager() { return MainWorldManager; }

    public int GetPlayerNum()
    {
        return PlayerNum;
    }

    public void Player0End()
    {
        MainRoundManager.PlayerEndRound(0);
    }

    public void Player1End()
    {
        MainRoundManager.PlayerEndRound(1);
    }

    // Use this for initialization
    void Start ()
    {
        // 获取位于网络管理器上的网络控制器
        //MainNetworkController = GameObject.Find("NetworkManager").GetComponent<NetworkController>();

        // 向网络管理器注册自己的数据包处理函数
        //MainNetworkController.ManagerRegistion("GameManager", MessageReceive);

        // 用于单场景开发时加载测试使用
        if (GameObject.Find("ClientPlayerManager") == null)
        {
            PlayerID = 0;
            GameMapID = "FreedomDay";
            FirstRoundPlayerID = 0;
        }
        else
        {
            MainClientPlayerController = GameObject.Find("ClientPlayerManager").GetComponent<ClientPlayerController>();
            PlayerID = MainClientPlayerController.GetPlayerID();
            GameMapID = MainClientPlayerController.GetGameMapID();
            FirstRoundPlayerID = MainClientPlayerController.GetFirstRoundPlayerID();
        }

        // 1. 生成所有游戏必要组件
        //===================================================================================================================
        MainLibraryManager = new LibraryManager(this);

        MainCardManager = new CardManager(this);

        MainResourceManager = new ResourceManager(this);
        MainDiplomacyManager = new DiplomacyManager(this);
        MainRoundManager = new RoundManager(this);
        

        MainEnvironmentManager = new EnvironmentManager(this);
        MainWorldManager = new WorldManager(this);

        MainGameLoader = new GameLoader(this);

        MainInitialHandCardManager = new InitialHandCardManager(this);

        GameObject CardDisplayManagerPrefab = (GameObject)Resources.Load("Prefabs/Manager/GamePlay/CardDisplayManager");
        CardDisplayManager = GameObject.Instantiate(CardDisplayManagerPrefab);
        CardDisplayManager.name = "CardDisplayManager";
        MainCardDisplayController = CardDisplayManager.GetComponent<CardDisplayController>();
        MainCardDisplayController.Initialize(this);

        GameObject InputManagerPrefab = (GameObject)Resources.Load("Prefabs/Manager/GamePlay/InputManager");
        InputManager = GameObject.Instantiate(InputManagerPrefab);
        InputManager.name = "InputManager";
        MainInputController = InputManager.GetComponent<InputController>();
        MainInputController.SetCurrentPlayerID(PlayerID);
        MainInputController.Initialize(this);

        GameObject MessageManagerPrefab = (GameObject)Resources.Load("Prefabs/Manager/GamePlay/MassageManager");
        MessageManager = GameObject.Instantiate(MessageManagerPrefab);
        MessageManager.name = "MessageManager";
        MainMessageController = MessageManager.GetComponent<MessageController>();
        MainMessageController.Initialize(this);


        GameObject UIManagerPrefab = (GameObject)Resources.Load("Prefabs/Manager/GamePlay/UIManager");
        UIManager = GameObject.Instantiate(UIManagerPrefab);
        UIManager.name = "UIManager";
        MainUIController = UIManager.GetComponent<UIController>();
        MainUIController.Initialize(this);

        //2. 将游戏组件中的refernece建立
        //===================================================================================================================

        MainLibraryManager.EstablishReference();
        MainCardManager.EstablishReference();
        MainResourceManager.EstablishReference();
        MainDiplomacyManager.EstablishReference();
        MainRoundManager.EstablishReference();
        MainEnvironmentManager.EstablishReference();
        MainWorldManager.EstablishReference();
        MainGameLoader.EstablishReference();
        MainInitialHandCardManager.EstablishReference();

        MainCardDisplayController.EstablishReference();
        MainInputController.EstablishReference();
        MainMessageController.EstablishReference();
        MainUIController.EstablishReference();

        // 参数设置
        //===================================================================================================================

        MainRoundManager.SetCurrentRoundPlayerID(FirstRoundPlayerID);
        MainGameLoader.SetGameMapID(GameMapID);

        //===================================================================================================================
        MainLibraryManager.LoadGameDataFromXML();

        MainGameLoader.LoadGame();



        // 生成发送信息
        string SendMessage = "";

        // 通知网络管理器发送信息
        //CurrentMessageToBeSent = MainNetworkController.AddMessageInSendList(SendMessage);



        MainInitialHandCardManager.GenerateInitialHandCard(3);





    }

    // Update is called once per frame
    void Update ()
    {
        if (Input.GetKeyDown(KeyCode.S))
        {
            MainCardManager.Player0DrawACard();
        }
    }

    void OnDestroy()
    {
        // 向网络管理器注销自己的数据包处理函数
        MainNetworkController.ManagerCancellation("GameManager");
    }
}
