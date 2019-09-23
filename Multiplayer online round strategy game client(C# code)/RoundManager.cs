using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoundManager
{
    GameController MainGameController = null;

    MessageController MainMessageController = null;

    // 本场游戏中全部棋子的索引数组
    List<GameObject> GamePieceList = new List<GameObject>();

    // 当前正在进行回合的玩家ID
    int CurrentRoundPlayerID = -1;

    // 玩家回合序列
    List<int> RoundList = new List<int>();

    // 管理器构造函数
    public RoundManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;        
    }

    // 设置当前行动玩家
    public void SetCurrentRoundPlayerID(int NewCurrentRoundPlayerID)
    {
        CurrentRoundPlayerID = NewCurrentRoundPlayerID;
    }

    // 引用建立
    public void EstablishReference()
    {
        MainMessageController = MainGameController.MessageManager.GetComponent<MessageController>();
    }

    // 获取下一个将要进行回合的玩家ID并且将该ID从回合序列中取出
    public int GetNextRoundPlayerID()
    {
        int Res = -1;

        if (RoundList.Count >= 1)
        {
            Res = RoundList[0];
            RoundList.Remove(RoundList[0]);
        }
        else
        {
            Debug.Log("RoundList have no item !!!");
        }

        return Res;
    }

    // 向回合序列的尾部插入一个玩家PlayerID的回合
    public void InsertPlayerRound(int PlayerID)
    {
        RoundList.Add(PlayerID);
    }

    // 获取当前正在进行回合的玩家的ID
    public int GetCurrentRoundPlayerID()
    {
        return CurrentRoundPlayerID;
    }

    void BeginRoundUpdate(int PlayerID)
    {
        for (int i = 0; i < GamePieceList.Count; i++)
        {
            // 调用棋子的回合开始更新函数
        }
    }

    void EndRoundUpdate(int PlayerID)
    {
        for (int i = 0; i < GamePieceList.Count; i++)
        {
            // 调用棋子的回合结束更新函数
        }
    }

    public void PlayerBeginRound(int PlayerID)
    {
        // 给所有玩家显示消息
        MainMessageController.PlayerBeginRound(PlayerID);

        // 触发回合开始阶段结算
        //BeginRoundUpdate(PlayerID);

        CurrentRoundPlayerID = PlayerID;

    }

    public void PlayerEndRound(int PlayerID)
    {
        // 给所有玩家显示消息
        MainMessageController.PlayerEndRound(PlayerID);

        // 触发回合结束阶段结算
        //EndRoundUpdate(PlayerID);

        InsertPlayerRound(PlayerID);

        // 获取下一位行动的玩家ID
        int NextRoundPlayerID = GetNextRoundPlayerID();

        // 开始下一位行动的玩家的回合
        PlayerBeginRound(NextRoundPlayerID);

    }

    public void Player0End()
    {
        PlayerEndRound(0);
    }

    public void Player1End()
    {
        PlayerEndRound(1);
    }

}
