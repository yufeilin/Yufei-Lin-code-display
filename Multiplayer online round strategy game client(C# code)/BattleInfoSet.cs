using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BattleInfoSet
{
    static DiplomacyManager MainDiplomacyManager = null;

    Dictionary<int, PlayerBattleInfoSet> PlayerBattleInfoSetOfPlayID = new Dictionary<int, PlayerBattleInfoSet>();

    Dictionary<int, int> PlayerIDOfIndex = new Dictionary<int, int>();

    int PlayerNum = 0;

    public BattleInfoSet()
    {
        if(MainDiplomacyManager == null)
        {
            MainDiplomacyManager = GameObject.Find("GameManager").GetComponent<GameController>().GetDiplomacyManager();
        }
    }

    public void AddPlayerBattleInfoSet(PlayerBattleInfoSet NewPlayerBattleInfoSet)
    {
        if(PlayerBattleInfoSetOfPlayID.ContainsKey(NewPlayerBattleInfoSet.GetOwner())== true)
        {
            Debug.Log("Add PlayerBattleInfoSet Fail~ There already have one with same key value.");
        }
        else
        {
            PlayerBattleInfoSetOfPlayID.Add(NewPlayerBattleInfoSet.GetOwner(), new PlayerBattleInfoSet());
            PlayerBattleInfoSetOfPlayID[NewPlayerBattleInfoSet.GetOwner()] = NewPlayerBattleInfoSet;

            PlayerIDOfIndex.Add(PlayerNum, NewPlayerBattleInfoSet.GetOwner());
            PlayerNum++;
        }
    }

    public PlayerBattleInfoSet GetPlayerBattleInfoSetByPlayerID(int PlayerID)
    {
        if(PlayerBattleInfoSetOfPlayID.ContainsKey(PlayerID) == true)
        {
            return PlayerBattleInfoSetOfPlayID[PlayerID];
        }
        else
        {
            PlayerBattleInfoSet NewPlayerBattleInfoSet = new PlayerBattleInfoSet();
            NewPlayerBattleInfoSet.SetOwner(PlayerID);

            AddPlayerBattleInfoSet(NewPlayerBattleInfoSet);
            return PlayerBattleInfoSetOfPlayID[PlayerID];
        }        
    }

    // 获取本场战斗的最大行动射程
    public float GetMaxRangeInAllAction()
    {
        float Res = -1;

        for(int i=0;i<PlayerNum;i++)
        {
            float CurrentPlayerMaxRange = PlayerBattleInfoSetOfPlayID[PlayerIDOfIndex[i]].GetMaxRange();
            if(CurrentPlayerMaxRange > Res)
            {
                Res = CurrentPlayerMaxRange;
            }
        }

        return Res;
    }

    // 为所有玩家的所有行动计算初始冷却时间
    public void InitializeCumulativeCooldownTime()
    {
        float MaxRange = GetMaxRangeInAllAction();

        for (int i = 0; i < PlayerNum; i++)
        {
            PlayerBattleInfoSet CurrentPlayerBattleInfoSet = PlayerBattleInfoSetOfPlayID[PlayerIDOfIndex[i]];

            CurrentPlayerBattleInfoSet.InitializeCumulativeCooldownTime(MaxRange);
        }
    }

    // 将所有玩家的所有行动加载到战斗序列当中
    public void FetchActionFromBattleInfoSet(BattleSequence ParaBattleSequence)
    {
        for (int i = 0; i < PlayerNum; i++)
        {
            PlayerBattleInfoSet CurrentPlayerBattleInfoSet = PlayerBattleInfoSetOfPlayID[PlayerIDOfIndex[i]];

            CurrentPlayerBattleInfoSet.FetchActionFromPlayerBattleInfoSet(ParaBattleSequence);
        }
    }

    // 根据执行单位和执行单位使用的武器获取所有可攻击的单位类型(属于敌方的并且符合武器目标筛选标旗的单位类型)
    public List<UnitTypeInfo> FindAvaliableEnemyUnitTypes(Unit ExecuteUnit, Weapon ExecuteUnitWeapon)
    {
        List<UnitTypeInfo> Res = new List<UnitTypeInfo>();

        for (int i = 0; i < PlayerNum; i++)
        {
            // 如果执行单位的所有者视当前玩家为敌人
            if(MainDiplomacyManager.GetDiplomacy(ExecuteUnit.GetUnitOwner(), PlayerIDOfIndex[i]) == 0)
            {
                PlayerBattleInfoSet CurrentPlayerBattleInfoSet = PlayerBattleInfoSetOfPlayID[PlayerIDOfIndex[i]];

                CurrentPlayerBattleInfoSet.FindAvaliableEnemyUnitTypes(ExecuteUnitWeapon, Res);
            }
           
        }

        return Res;
    }

    public List<GameObject> GetPlayerAndAllyBattlePiece(int PlayerID)
    {
        List<GameObject> Res = new List<GameObject>();

        for (int i = 0; i < PlayerNum; i++)
        {
            // 如果玩家PlayerID视当前玩家为盟友
            if (MainDiplomacyManager.GetDiplomacy(PlayerID, PlayerIDOfIndex[i]) == 1)
            {
                // 将该玩家在本次战斗中的所有棋子加入到结果数组当中
                PlayerBattleInfoSetOfPlayID[PlayerIDOfIndex[i]].FetchPlayerBattlePieces(Res);
            }
        }

        return Res;
    }

    public void DubugDisplay()
    {
        Debug.Log("**********************************************************");
        for (int i = 0; i < PlayerNum; i++)
        {
            PlayerBattleInfoSet CurrentPlayerBattleInfoSet = PlayerBattleInfoSetOfPlayID[PlayerIDOfIndex[i]];

            Debug.Log("Player " + PlayerIDOfIndex[i] + " MaxRange " + PlayerBattleInfoSetOfPlayID[PlayerIDOfIndex[i]].GetMaxRange());

            CurrentPlayerBattleInfoSet.DebugDisplay();
        }
        Debug.Log("**********************************************************");
    }
}
