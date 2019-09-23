using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Action
{
    // 执行该行动的单位引用
    Unit ExcuteUnit = null;

    // 使用该武器或者技能的单位的ID
    string UnitID = "";

    // 本次行动中被使用的武器或者技能名称
    string ActionName = "";

    // 本次行动中被使用的武器或者技能的引用
    BattleOperation RelativeBattleOperation = null;

    // 该行动的冷却时间
    float ActionInterval = 0.0f;

    // 该行动的射程
    float ActionRange = 0.0f;

    // 累计冷却时间
    float CumulativeCooldownTime = 0.0f;

    // 行动累加一个单位的冷却时间
    public void AddOneInterval()
    {
        CumulativeCooldownTime += ActionInterval;
    }

    public void InitializeCumulativeCooldownTime(float GlobalMaxRange)
    {
        CumulativeCooldownTime = (GlobalMaxRange - ActionRange) / ExcuteUnit.GetMobilityFactor() + (ExcuteUnit.GetUnitSize() / ExcuteUnit.GetMobilityFactor()) * Random.Range(-2.0f, 2.0f);
    }

    public void ExcuteAction(BattleInfoSet ParaBattleInfoSet, BattleSequence ParaBattleSequence)
    {
        // 调用武器或者技能的执行方法实际执行该行动
        if (ParaBattleInfoSet == null) Debug.Log("ParaBattleInfoSet");
        RelativeBattleOperation.ExcuteBattleOperation(ExcuteUnit, ParaBattleInfoSet, ParaBattleSequence);
        
    }

    public Unit GetExcuteUnit()
    {
        return ExcuteUnit;
    }

    public void SetExcuteUnit(Unit NewExcuteUnit)
    {
        ExcuteUnit = NewExcuteUnit;
    }

    public string GetUnitID()
    {
        return UnitID;
    }

    public void SetUnitID(string NewUnitID)
    {
        UnitID = NewUnitID;
    }

    public string GetActionName()
    {
        return ActionName;
    }

    public BattleOperation GetRelativeBattleOperation()
    {
        return RelativeBattleOperation;
    }

    public void SetRelativeBattleOperation(BattleOperation NewRelativeBattleOperation)
    {
        RelativeBattleOperation = NewRelativeBattleOperation;
    }

    public float GetActionInterval()
    {
        return ActionInterval;
    }

    public void SetActionInterval(float NewActionInterval)
    {
        ActionInterval = NewActionInterval;
    }

    public float GetActionRange()
    {
        return ActionRange;
    }

    public void SetActionRange(float NewActionRange)
    {
        ActionRange = NewActionRange;
    }

    public float GetCumulativeCooldownTime()
    {
        return CumulativeCooldownTime;
    }

    public void SetCumulativeCooldownTime(float NewCumulativeCooldownTime)
    {
        CumulativeCooldownTime = NewCumulativeCooldownTime;
    }

    public void SetActionName(string NewActionName)
    {
        ActionName = NewActionName;
    }
}
