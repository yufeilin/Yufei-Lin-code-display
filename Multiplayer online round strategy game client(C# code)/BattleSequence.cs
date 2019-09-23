using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// 战斗执行器
public class BattleSequence
{
    // 战斗序列执行时间
    float CurrentTime = 0.0f;

    // 前战斗行动序列 用于执行一些战斗进行之前的行动
    List<Action> PreBattleActionList = new List<Action>();

    // 战斗行动序列
    List<Action> BattleActionList = new List<Action>();

    // 后战斗行动序列 用于执行一些战斗进行之后的行动
    List<Action> LastBattleActionList = new List<Action>();

    // 进攻棋子索引
    GameObject MainAttackPiece = null;

    // 防御棋子索引
    GameObject MainDefendPiece = null;

    // 本场战斗的相关战斗信息集合
    BattleInfoSet RelativeBattleInfoSet = null;

    // 进攻棋子所属玩家ID
    int AttackPlayerID = -1;

    // 防御棋子所属玩家ID
    int DefendPlayerID = -1;

    public void SetMainAttackPiece(GameObject ParaMainAttackPiece)
    {
        MainAttackPiece = ParaMainAttackPiece;
        AttackPlayerID = ParaMainAttackPiece.GetComponentInChildren<PieceStateController>().GetOwner();
    }

    public void SetMainDefendPiece(GameObject ParaMainDefendPiece)
    {
        MainDefendPiece = ParaMainDefendPiece;
        DefendPlayerID = ParaMainDefendPiece.GetComponentInChildren<PieceStateController>().GetOwner();
    }

    public void SetRelativeBattleInfoSet(BattleInfoSet ParaRelativeBattleInfoSet)
    {
        RelativeBattleInfoSet = ParaRelativeBattleInfoSet;
    }

    // 向战斗行动队列中插入一个新的Action
    public void InsertAction(Action NewAction)
    {
        if(BattleActionList.Count == 0)
        {
            BattleActionList.Add(NewAction);
            return;
        }
        // 使用二分查找找到位置然后使用Insert插入
        int left = 0, right = BattleActionList.Count-1;
        // 如果小于第一个元素则插入队列首
        if(NewAction.GetCumulativeCooldownTime() < BattleActionList[left].GetCumulativeCooldownTime())
        {
            BattleActionList.Insert(0, NewAction);
            return;
        }
        // 如果大于队列尾的元素则插在队列尾部
        if(NewAction.GetCumulativeCooldownTime() > BattleActionList[right].GetCumulativeCooldownTime())
        {
            BattleActionList.Insert(BattleActionList.Count, NewAction);
            return;
        }

        // 二分法查找队列中小于待插入action冷却时间的最大action并将新的action插入到该action后面
        while(left < right)
        {
            int mid = (left + right) / 2;
            if(BattleActionList[mid].GetCumulativeCooldownTime() <= NewAction.GetCumulativeCooldownTime())
            {
                left = mid + 1;
            }
            else
            {
                right = mid;
            }
        }

        BattleActionList.Insert(right, NewAction);
    }

    // 向前战斗行动序列中插入战前类型的行动(不清楚是否需要自动排序)
    public void InsertActionInPreActionList(Action NewAction)
    {
        PreBattleActionList.Add(NewAction);
    }

    // 向后战斗行动序列中插入战后类型的行动(不清楚是否需要自动排序)
    public void InsertActionInLastActionList(Action NewAction)
    {
        LastBattleActionList.Add(NewAction);
    }

    public void DisplayAllActionInList()
    {
        for(int i=0;i< BattleActionList.Count;i++)
        {
            Debug.Log(BattleActionList[i].GetActionName() + " " + BattleActionList[i].GetCumulativeCooldownTime() + " " + BattleActionList[i].GetExcuteUnit().GetUnitOwner());
        }
    }

    public void ExcuteOneAction()
    {
        // 取出队列首Action
        Action CurrentAction = BattleActionList[0];
        BattleActionList[0] = null;
        BattleActionList.Remove(BattleActionList[0]);

        if(CurrentAction.GetExcuteUnit().GetAliveState() == true)
        {
            // 更新战斗序列当前时间
            CurrentTime = CurrentAction.GetCumulativeCooldownTime();

            // Action具体执行
            if (RelativeBattleInfoSet == null) Debug.Log("RelativeBattleInfoSet");
            CurrentAction.ExcuteAction(RelativeBattleInfoSet, this);

            // Action增加一个单位的冷却时间
            CurrentAction.AddOneInterval();

            // 再将更新后的Action插入回战斗Action队列中
            InsertAction(CurrentAction);
        }
        else
        {

        }
        

    }

    // 判断棋子ParaPiece在当前时刻是否决定撤退
    public bool PieceDecideRetreat(GameObject ParaPiece)
    {
        PieceStateController ParaPieceStateController = ParaPiece.GetComponentInChildren<PieceStateController>();
        float LostHealthInBattle = ParaPieceStateController.GetCurrentPieceHealthBeforeBattle() - ParaPieceStateController.GetPieceCurrentHealth();
        float CurrentHealthInBattle = ParaPieceStateController.GetPieceCurrentHealth();

        if (LostHealthInBattle > 0)
        {
            // 如果在本场战斗中已经损失的生命值超过了该棋子 (最大生命值 X 棋子的撤退阈值(默认撤退阈值为25%)) 则触发撤退机制
            if(LostHealthInBattle / ParaPieceStateController.GetPieceMaxHealth() >= ParaPieceStateController.GetRetreatThreshold())
            {
                float RandomFloat = Random.Range(0, 100);

                float X = LostHealthInBattle / (LostHealthInBattle + CurrentHealthInBattle);

                //TODO: 此处应该使用棋子自带的撤退策略来判断是否撤退

                // 在本场战斗损失生命值已经超过棋子生命值25%的前提下, 本场战斗损失生命值越多，撤退概率越大
                if (RandomFloat <= 50 + 50 * X)
                {
                    Debug.Log("player:" + ParaPieceStateController.GetOwner() + "retreat.");
                    return true;
                }
            }
        }

        // 重要TODO
        // 执行具体的撤退操作并且移除战斗单位数组中的对应单位引用以及战斗序列中的对应操作

        // 棋子死亡算作撤退
        if(CurrentHealthInBattle == 0)
        {
            return true;
        }


        return false;
    }

    // 判断玩家PlayerID在当前是否决定撤退(如果玩家的进攻/防御棋子决定撤退则玩家决定撤退)
    bool PlayerDecideRetreat(int PlayerID)
    {
        bool PlayerRetreat = false;
        // 获取战斗中属于PlaerID玩家的以及PlayerID玩家盟友的所有棋子(助战棋子)
        List<GameObject> PlayerBattlePieceList = RelativeBattleInfoSet.GetPlayerAndAllyBattlePiece(PlayerID);

        // 遍历每个参与战斗的棋子并检查他们是否要撤退
        for(int i=0;i< PlayerBattlePieceList.Count;i++)
        {
            if(PieceDecideRetreat(PlayerBattlePieceList[i]) == true)
            {
                // 如果该棋子决定撤退并且该棋子为进攻棋子或者防御棋子则将玩家撤退标旗修改为true
                if (PlayerBattlePieceList[i] == MainAttackPiece || PlayerBattlePieceList[i] == MainDefendPiece)
                {
                    PlayerRetreat = true;
                }
            }

            if (PlayerBattlePieceList[i] == MainAttackPiece && PlayerBattlePieceList[i].GetComponentInChildren<PieceStateController>().GetAliveState() == false)
            {
                //Debug.Log("111");
                PlayerRetreat = true;
            }

            if(PlayerBattlePieceList[i] == MainDefendPiece && PlayerBattlePieceList[i].GetComponentInChildren<PieceStateController>().GetAliveState() == false)
            {
                //Debug.Log("222");
                PlayerRetreat = true;
            }
        }

        int AlivePieceNum = 0;

        for (int i = 0; i < PlayerBattlePieceList.Count; i++)
        {
            if(PlayerBattlePieceList[i].GetComponentInChildren<PieceStateController>().GetAliveState() == true)
            {
                AlivePieceNum++;
            }
        }

        // 如果战斗中属于PlaerID玩家的以及PlayerID玩家盟友的所有棋子全部阵亡则该玩家决定撤退
        if (AlivePieceNum == 0)
        {
            //Debug.Log("333");
            PlayerRetreat = true;
        }

        return PlayerRetreat;
    }

    // 开始执行战斗
    public void BattleExcute()
    {
        MainAttackPiece.GetComponentInChildren<PieceStateController>().UpdateBeforeBattle();
        MainDefendPiece.GetComponentInChildren<PieceStateController>().UpdateBeforeBattle();
        
        //while(!PreBattleActionList.Empty())
        //{
        //
        //}
        
        // 如果进攻棋子不决定撤退并且防御棋子不决定撤退则战斗序列继续推进
        while(PlayerDecideRetreat(AttackPlayerID) == false && PlayerDecideRetreat(DefendPlayerID) == false)
        {
            Debug.Log("execuate a action");
            ExcuteOneAction();
        }
        
        //while(!LastBattleActionList.empty())
        //{
        //
        //}
        
        
        MainAttackPiece.GetComponentInChildren<PieceStateController>().UpdateAfterBattle();
        MainDefendPiece.GetComponentInChildren<PieceStateController>().UpdateAfterBattle();
    }

}
