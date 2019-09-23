using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Weapon : BattleOperation
{
    // 武器ID(每个武器都不相同)
    string WeaponID = "";

    // 武器名称(可以重复)
    string WeaponName = "";

    // 武器伤害
    public int Damage = 0;

    // 武器射程
    float WeaponRange = 1.0f;

    // 武器开火间隔
    float WeaponAttackInterval = 1.0f;

    // 武器开火效果
    Effect WeaponEffect = null;

    // 武器开火单位锁定数目
    int TargetNum = 1;

    // 武器开火次数
    int MultipleAttacks = 1;

    // 武器的单位类型选择器
    UnitTypeSelector WeaponTargetUnitTypeSelector = new UnitTypeSelector();

    // 武器的单位选择器
    UnitSelector WeaponTargetUnitSelector = new UnitSelector();

    public string GetWeaponID()
    {
        return WeaponID;
    }

    public void SetWeaponID(string NewID)
    {
        WeaponID = NewID;
    }

    public string GetWeaponName()
    {
        return WeaponName;
    }
    
    public void SetWeaponName(string NewName)
    {
        WeaponName = NewName;
    }

    public float GetWeaponRange()
    {
        return WeaponRange;
    }

    public void SetWeaponRange(float NewRange)
    {
        WeaponRange = NewRange;
    }

    public float GetAttackInterval()
    {
        return WeaponAttackInterval;
    }

    public void SetAttackInterval(float NewInterval)
    {
        WeaponAttackInterval = NewInterval;
    }

    public Effect GetWeaponAttackEffect()
    {
        return WeaponEffect;
    }

    public void SetWeaponAttackEffect(Effect NewWeaponAttackEffect)
    {
        WeaponEffect = NewWeaponAttackEffect;
    }

    public bool ValidTargetUnit(Unit TargetUnit)
    {
        // TODO: 根据武器单位筛选标旗来判断潜在攻击目标是否为合法攻击目标
        return true;
    }

    public override void ExcuteBattleOperation(Unit ExecuteUnit, BattleInfoSet ParaBattleInfoSet, BattleSequence ParaBattleSequence)
    {
        // 先锁定若干攻击单位
        for(int i=0;i< TargetNum;i++)
        {
            // 将武器使用单位传入根据单位所有者来寻找敌对单位
            // 将武器本身引用传入是要根据武器的目标筛选标旗来选择合适的攻击单位类型
            if (ParaBattleInfoSet == null) Debug.Log("ParaBattleInfoSet");
            if (ExecuteUnit == null) Debug.Log("ExecuteUnit");
            

            List<UnitTypeInfo> EnemyUnitTypeList = ParaBattleInfoSet.FindAvaliableEnemyUnitTypes(ExecuteUnit, this);

            if (EnemyUnitTypeList == null) Debug.Log("EnemyUnitTypeList");

            if (EnemyUnitTypeList.Count > 0)
            {
                // 使用武器的单位类型选择器先选择一个单位类型
                UnitTypeInfo TargetUnitType = WeaponTargetUnitTypeSelector.ExecuteSingleSelection( EnemyUnitTypeList );

                // 再使用武器的单位选择器选择一个攻击目标单位
                Unit TargetUnit = WeaponTargetUnitSelector.ExecuteSingleSelection(TargetUnitType.GetContainUnit());

                // 再对攻击单位执行若干次武器开火效果
                for (int j = 0; j < MultipleAttacks; j++)
                {

                    Debug.Log(ExecuteUnit.GetUnitOwner() + " attack " + TargetUnit.GetUnitOwner());
                    WeaponEffect.ExcuteEffect(ExecuteUnit, TargetUnit, ParaBattleInfoSet, ParaBattleSequence);
                }
            }
            else
            {
                //此时没有敌方单位可以被攻击了
            }            
        }  
        
        // 可以再添加先进行多重攻击再进行多目标锁定的攻击模式
    }

    public void WeaponActivate(Unit AttackUnit, Unit TargetUnit)
    {
        TargetUnit.ReceiveDamage(AttackUnit, Damage);
    }

    public Action GetAction()
    {
        Action Res = new Action();

        Res.SetActionName(WeaponName);
        Res.SetActionInterval(WeaponAttackInterval);
        Res.SetActionRange(WeaponRange);

        return Res;
    }
}
