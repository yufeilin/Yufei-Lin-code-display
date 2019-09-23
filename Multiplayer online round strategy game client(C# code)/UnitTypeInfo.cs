using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnitTypeInfo
{
    Dictionary<string, Parameter> ParameterOfName = new Dictionary<string, Parameter>();

    List<Unit> UnitsOfCurrentType = new List<Unit>();

    public UnitTypeInfo()
    {
        ParameterOfName.Add("UnitID", new StringParameter());
        ParameterOfName.Add("TargetProbability", new IntParameter());
        ParameterOfName.Add("UnitTypeRange", new IntParameter());
        ParameterOfName.Add("UnitTypeSize", new IntParameter());
        ParameterOfName.Add("UnitTypeAliveNum", new IntParameter());


        ParameterOfName["UnitID"].SetParameter("");
        ParameterOfName["TargetProbability"].SetParameter("0");
        ParameterOfName["UnitTypeRange"].SetParameter("0");
        ParameterOfName["UnitTypeSize"].SetParameter("0");
        ParameterOfName["UnitTypeAliveNum"].SetParameter("0");
    }

    public object GetParameter(string ParaName)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            return ParameterOfName[ParaName].GetParameter();
        }
        else
        {
            Debug.Log("Cannot Find Parameters");
            return null;
        }
    }

    public void SetParameter(string ParaName, string ParaValue)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            ParameterOfName[ParaName].SetParameter(ParaValue);
        }
        else
        {
            Debug.Log("Cannot Find Parameters");
        }
    }

    public List<Unit> GetContainUnit()
    {
        return UnitsOfCurrentType;
    }

    // 根据单位数组初始化单位类型信息
    public void InitializeByUnitList(List<Unit> ParaAttackableUnits)
    {
        float UnitRangeSum = 0.0f;
        float UnitSizeSum = 0.0f;
        float UnitAliveNum = 0.0f;

        for(int i=0;i< ParaAttackableUnits.Count;i++)
        {
            UnitsOfCurrentType.Add(ParaAttackableUnits[i]);
            UnitRangeSum += ParaAttackableUnits[i].CalculateUnitRange();
            UnitSizeSum += ParaAttackableUnits[i].GetUnitSize();
            UnitAliveNum++;
        }

        SetParameter("UnitTypeRange", ((int)(UnitRangeSum / ParaAttackableUnits.Count)).ToString());
        SetParameter("UnitTypeSize", ((int)(UnitSizeSum / ParaAttackableUnits.Count)).ToString());
        SetParameter("UnitTypeAliveNum", ((int)UnitAliveNum).ToString());
    }

    public void DebugDisplay()
    {
        for(int i=0;i< UnitsOfCurrentType.Count;i++)
        {
            Debug.Log("===" + UnitsOfCurrentType[i].GetUnitOwner() + "===");
        }
        
    }

}
