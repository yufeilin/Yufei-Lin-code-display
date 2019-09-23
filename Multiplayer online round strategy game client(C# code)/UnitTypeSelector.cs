using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnitTypeSelector
{
    // 单位选择模式
    // SelectMode = 2 具有较高SelectKey属性的单位一定会排在选择结果List的前面
    // SelectMode = 1 具有较高SelectKey属性的单位有较高可能会排在选择结果List的前面
    // SelectMode = 0 随机选择
    // SelectMode = -1 具有较低SelectKey属性的单位有较高可能会排在选择结果List的前面
    // SelectMode = -2 具有较低SelectKey属性的单位一定会排在选择结果List的前面
    int SelectMode = 0;

    // 用来排序的单位属性
    string SelectKey = "";

    public int GetSelectMode()
    {
        return SelectMode;
    }

    public void SetSelectMode(int NewSelectMode)
    {
        SelectMode = NewSelectMode;
    }

    public string GetSelectKey()
    {
        return SelectKey;
    }

    public void SetSelectKey(string NewSelectKey)
    {
        SelectKey = NewSelectKey;
    }

    public UnitTypeInfo ExecuteSingleSelection(List<UnitTypeInfo> ParaUnitTypeInfoList)
    {
        for(int i=0;i< ParaUnitTypeInfoList.Count;i++)
        {
            ParaUnitTypeInfoList[i].DebugDisplay();
        }

        //Debug.Log("((((" + ParaUnitTypeInfoList.Count + "))))");

        // TODO: 完善其他的选择模式
        // 为啥这里貌似右边界是不包括的
        int RandomIndex = Random.Range(0, ParaUnitTypeInfoList.Count);

        

        return ParaUnitTypeInfoList[RandomIndex];
    }

}
