using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// 在战斗中某个玩家的所有参战单位和行动(武器，攻击技能和辅助技能)信息集合
public class PlayerBattleInfoSet
{
    // 该信息集合所属玩家ID
    int Owner = -1;

    // 当前玩家在本次战斗中的所有行动中最远的行动射程
    float MaxRange = -1;

    // 该信息集合中包含的单位种类数目(单位词典和行动词典中Key的数目)
    int UnitTypeNum = 0;

    // 遍历辅助词典
    Dictionary<int, string> UnitNameOfIndex = new Dictionary<int, string>();

    // 当前玩家在本次战斗中的所有参战单位
    Dictionary<string, List<Unit>> UnitsOfUnitName = new Dictionary<string, List<Unit>>();
    // 当前玩家在本次战斗中的所有行动(武器，攻击技能和辅助技能)
    Dictionary<string, List<Action>> ActionsOfUnitName = new Dictionary<string, List<Action>>();

    // 当前玩家在本次战斗中的所有棋子(进攻/防御棋子以及所有的助战棋子)
    List<GameObject> PlayerBattlePiece = new List<GameObject>();

    public int GetOwner()
    {
        return Owner;
    }

    public void SetOwner(int NewOwner)
    {
        Owner = NewOwner;
    }

    public void AddNewActionFromBattleUnit(Unit BattleUnit)
    {
        // 获取该单位所有的行动
        List<Action> BattleUnitActions = BattleUnit.GetActions();

        for(int i=0;i< BattleUnitActions.Count;i++)
        {
            //Debug.Log(BattleUnitActions[i].GetActionName() + "==" + BattleUnitActions[i].GetExcuteUnit().GetUnitOwner());

            // 将每个该单位的行动添加到信息集合的行动词典数组当中
            string UnitIDOfCurrentAction = BattleUnitActions[i].GetUnitID();
            if (ActionsOfUnitName.ContainsKey(UnitIDOfCurrentAction) ==true)
            {
                ActionsOfUnitName[UnitIDOfCurrentAction].Add(BattleUnitActions[i]);
            }
            else
            {
                // 其实讲道理这里是不应该被进入的
                ActionsOfUnitName.Add(UnitIDOfCurrentAction, new List<Action>());
                ActionsOfUnitName[UnitIDOfCurrentAction].Add(BattleUnitActions[i]);
                Debug.Log("进入不该进入的分支");
            }

            // 更新信息集合中包含的最大行动射程
            if (BattleUnitActions[i].GetActionRange() > MaxRange)
            {
                MaxRange = BattleUnitActions[i].GetActionRange();
            }
        }       
    }

    // 向信息集合中添加一个单位
    public void AddNewBattleUnit(Unit NewUnit)
    {
        string NewUnitID = NewUnit.GetUnitID();
        // 检查该类型的单位是否在两个词典中是第一次出现
        if (UnitsOfUnitName.ContainsKey(NewUnitID) ==true)
        {
            // 如果该类型的单位已经在两个词典中出现则直接执行添加操作即可

            // 在单位词典中添加该单位的引用
            UnitsOfUnitName[NewUnitID].Add(NewUnit);

            // 在行动词典添加该单位的所有行动
            AddNewActionFromBattleUnit(NewUnit);
        }
        else
        {
            // 如果该类型单位尚未在两个词典中出现则在两个词典中同时添加该类型单位Key值并递增索引

            UnitsOfUnitName.Add(NewUnitID, new List<Unit>());
            // 在单位词典中添加该单位的引用
            UnitsOfUnitName[NewUnitID].Add(NewUnit);

            ActionsOfUnitName.Add(NewUnitID, new List<Action>());
            // 在行动词典添加该单位的所有行动
            AddNewActionFromBattleUnit(NewUnit);

            // 因为这里只用了一个int来计数词典中Key的数目所以要保证单位词典和行动词典中Key个数时刻相同
            UnitNameOfIndex.Add(UnitTypeNum, NewUnitID);
            UnitTypeNum++;
        }
    }

    // 向信息集合中添加一组单位
    public void AddNewBattleUnits(List<Unit> NewUnits)
    {
        for(int i=0;i<NewUnits.Count;i++)
        {
            AddNewBattleUnit(NewUnits[i]);
        }
    }

    public float GetMaxRange()
    {
        return MaxRange;
    }

    public void InitializeCumulativeCooldownTime(float GlobalMaxRange)
    {
        for (int i = 0; i < UnitTypeNum; i++)
        {
            for (int j = 0; j < ActionsOfUnitName[UnitNameOfIndex[i]].Count; j++)
            {
                ActionsOfUnitName[UnitNameOfIndex[i]][j].InitializeCumulativeCooldownTime(GlobalMaxRange);
            }
        }
    }

    public void DebugDisplay()
    {
        for(int i=0;i< UnitTypeNum;i++)
        {
            for(int j=0;j< UnitsOfUnitName[UnitNameOfIndex[i]].Count;j++)
            {
                Debug.Log(UnitsOfUnitName[UnitNameOfIndex[i]][j].GetUnitID());
            }

            for (int j = 0; j < ActionsOfUnitName[UnitNameOfIndex[i]].Count; j++)
            {
                Debug.Log(ActionsOfUnitName[UnitNameOfIndex[i]][j].GetActionName() + " "+ ActionsOfUnitName[UnitNameOfIndex[i]][j].GetCumulativeCooldownTime());
            }
        }
    }

    public void FetchActionFromPlayerBattleInfoSet(BattleSequence ParaBattleSequence)
    {
        for (int i = 0; i < UnitTypeNum; i++)
        {
            for (int j = 0; j < ActionsOfUnitName[UnitNameOfIndex[i]].Count; j++)
            {
                ParaBattleSequence.InsertAction(ActionsOfUnitName[UnitNameOfIndex[i]][j]);

                //Debug.Log(ActionsOfUnitName[UnitNameOfIndex[i]][j].GetActionName() + "==" + ActionsOfUnitName[UnitNameOfIndex[i]][j].GetExcuteUnit().GetUnitOwner());
            }
        }
    }

    public void FindAvaliableEnemyUnitTypes(Weapon ExecuteUnitWeapon, List<UnitTypeInfo> ParaUnitTypeInfoList)
    {
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> AttackableUnits = new List<Unit>();

            // 遍历该类型单位的每个单位并检查是否为当前攻击武器的合法攻击目标
            for (int j = 0; j < UnitsOfUnitName[UnitNameOfIndex[i]].Count; j++)
            {
                if(ExecuteUnitWeapon.ValidTargetUnit(UnitsOfUnitName[UnitNameOfIndex[i]][j]) == true && UnitsOfUnitName[UnitNameOfIndex[i]][j].GetAliveState() == true)
                {
                    AttackableUnits.Add(UnitsOfUnitName[UnitNameOfIndex[i]][j]);

                    
                }
            }

            // 如果该类型单位中至少有一个合法的攻击单位则将该单位生成为一个单位类型信息的实例
            if (AttackableUnits.Count > 0)
            {
                UnitTypeInfo NewUnitTypeInfo = new UnitTypeInfo();

                NewUnitTypeInfo.SetParameter("UnitID", UnitNameOfIndex[i]);

                NewUnitTypeInfo.InitializeByUnitList(AttackableUnits);

                if (ParaUnitTypeInfoList == null) Debug.Log("ParaUnitTypeInfoList");
                ParaUnitTypeInfoList.Add(NewUnitTypeInfo);
            }
        }
    }

    public void AddNewBattlePiece(GameObject NewBattelPiece)
    {
        PlayerBattlePiece.Add(NewBattelPiece);
    }

    // 将当前玩家在本战斗中的所有棋子加入到参数数组当中
    public void FetchPlayerBattlePieces(List<GameObject> ParaPlayerBattlePieceList)
    {
        for(int i=0;i<PlayerBattlePiece.Count;i++)
        {
            ParaPlayerBattlePieceList.Add(PlayerBattlePiece[i]);
        }
    }

}
