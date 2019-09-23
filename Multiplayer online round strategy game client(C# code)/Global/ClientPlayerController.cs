using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClientPlayerController : MonoBehaviour {

    int ShopOrderNumber = -1;

    // 即将开始的游戏中加载的地图ID
    string GameMapID = "NotAssign";

    // 即将开始的游戏中当前用户的玩家ID
    int PlayerID = -1;

    // 即将开始的游戏中第一个行动的玩家(先手玩家ID)
    int FirstRoundPlayerID = -1;

    // 当前客户端玩家使用的英雄ID
    string ClientPlayerHeroID = "NotAssign";

    // 当前客户端玩家使用的卡组ID
    string ClientPlayerDeckID = "NotAssign";

    // 当前客户端玩家使用的卡组包含卡牌的CTN数组
    List<string> ClientPlayerDeckCardCTNList = null;

    public int GetShopOrderNumber() { return ShopOrderNumber; }
    public void SetShopOrderNumber(int NewShopOrderNumber) { ShopOrderNumber = NewShopOrderNumber; }

    public string GetGameMapID() { return GameMapID; }
    public void SetGameMapID(string NewGameMapID) { GameMapID = NewGameMapID; }

    public int GetPlayerID() { return PlayerID; }
    public void SetPlayerID(int NewPlayerID) { PlayerID = NewPlayerID; }

    public int GetFirstRoundPlayerID() { return FirstRoundPlayerID; }
    public void SetFirstRoundPlayerID(int NewFirstRoundPlayerID) { FirstRoundPlayerID = NewFirstRoundPlayerID; }

    public string GetClientPlayerHeroID() { return ClientPlayerHeroID; }
    public void SetClientPlayerHeroID(string NewClientPlayerHeroID) { ClientPlayerHeroID = NewClientPlayerHeroID; }

    public string GetClientPlayerDeckID() { return ClientPlayerDeckID; }
    public void SetClientPlayerDeckID(string NewClientPlayerDeckID) { ClientPlayerDeckID = NewClientPlayerDeckID; }

    public List<string> GetClientPlayerDeckCardCTNList() { return ClientPlayerDeckCardCTNList; }

    // Use this for initialization
    void Start ()
    {
        GameObject.DontDestroyOnLoad(gameObject);

        ClientPlayerDeckCardCTNList = new List<string>();
    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
