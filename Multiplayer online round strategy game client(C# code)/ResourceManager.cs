using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResourceManager
{
    GameController MainGameController = null;
    // 要给每个玩家初始化自己的资源

    Dictionary<int, int> MineralOfPlayerID = new Dictionary<int, int>();
    Dictionary<int, int> MineralCapacityOfPlayerID = new Dictionary<int, int>();

    Dictionary<int, int> GasOfPlayerID = new Dictionary<int, int>();
    Dictionary<int, int> GasCapacityOfPlayerID = new Dictionary<int, int>();

    Dictionary<int, int> PlayerIDOfIndex = new Dictionary<int, int>();

    int PlayerNum = 0;

    PlayerResourceAreaController MainPlayerResourceAreaController = null;

    // 当前用户使用玩家拥有的晶体矿
    int ClientPlayerMineral = -1;
    // 当前用户使用玩家的晶体矿上限
    int ClientPlayerMineralCapacity = 500;
    // 当前用户使用玩家拥有的高能瓦斯
    int ClientPlayerGas = -1;
    // 当前用户使用玩家的高能瓦斯上限
    int ClientPlayerGasCapacity = 500;

    public ResourceManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
    }

    public void EstablishReference()
    {
        //Debug.Log("reference");
        MainPlayerResourceAreaController = GameObject.Find("Canvas/PlayerResourceArea").GetComponent<PlayerResourceAreaController>();

        if(MainPlayerResourceAreaController == null)
        {
            Debug.Log("reference");
        }

        
    }

    public void SetClientPlayerMineral(int NewClientPlayerMineral) { ClientPlayerMineral = NewClientPlayerMineral; }
    public void SetClientPlayerGas(int NewClientPlayerGas) { ClientPlayerGas = NewClientPlayerGas; }
    public void SetClientPlayerMineralCapacity(int NewClientPlayerMineralCapacity) { ClientPlayerMineralCapacity = NewClientPlayerMineralCapacity; }
    public void SetClientPlayerGasCapacity(int NewClientPlayerGasCapacity) { ClientPlayerGasCapacity = NewClientPlayerGasCapacity; }

    public int GetClientPlayerMineral() { return ClientPlayerMineral; }
    public int GetClientPlayerGas() { return ClientPlayerGas; }
    public int GetClientPlayerMineralCapacity() { return ClientPlayerMineralCapacity; }
    public int GetClientPlayerGasCapacity() { return ClientPlayerGasCapacity; }

    public void DisplayResourceUI() { MainPlayerResourceAreaController.DisplayResourceUI(); }

    // 初始化一个玩家的初始资源
    public void PlayerInitialize(int PlayerID, int InitialMineral = 0, int IntialGas = 0)
    {
        if (MineralOfPlayerID.ContainsKey(PlayerID) || GasOfPlayerID.ContainsKey(PlayerID))
        {
            Debug.Log("Player initial resources load fail.");
        }
        else
        {
            MineralOfPlayerID.Add(PlayerID, InitialMineral);
            MineralCapacityOfPlayerID.Add(PlayerID, 500);
            GasOfPlayerID.Add(PlayerID, IntialGas);
            GasCapacityOfPlayerID.Add(PlayerID, 400);
            PlayerIDOfIndex.Add(PlayerNum, PlayerID);
            PlayerNum++;
        }
    }

    public int GetPlayerMineral(int PlayerID)
    {
        return MineralOfPlayerID[PlayerID];
    }

    public int GetPlayerMineralCapacity(int PlayerID)
    {
        return MineralCapacityOfPlayerID[PlayerID];
    }

    public int GetPlayerGas(int PlayerID)
    {
        return GasOfPlayerID[PlayerID];
    }

    public int GetPlayerGasCapacity(int PlayerID)
    {
        return GasCapacityOfPlayerID[PlayerID];
    }

    public void SetPlayerMineral(int PlayerID, int NewMineral)
    {
        MineralOfPlayerID[PlayerID] = NewMineral;
    }

    public void SetPlayerGas(int PlayerID, int NewGas)
    {
        GasOfPlayerID[PlayerID] = NewGas;
    }
}
