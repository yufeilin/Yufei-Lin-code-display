using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnitSelector
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

    public Unit ExecuteSingleSelection(List<Unit> ParaUnitList)
    {
        // TODO: 完善其它选择模式
        int RandomIndex = Random.Range(0, ParaUnitList.Count - 1);

        return ParaUnitList[RandomIndex];
    }

    public List<Unit> ExecuteSelection(List<Unit> ParaUnitList, int UnitNum)
    {
        List<Unit> Res = new List<Unit>();

        List<Unit> Tmp = new List<Unit>();

        if(SelectMode == 2)
        {
            // 从大到小排序
            for (int i = 0; i < ParaUnitList.Count; i++)
            {
                Res.Add(ParaUnitList[i]);
            }

            Res.Sort(delegate (Unit X, Unit Y)
            {
                return ((int)Y.GetParameter(SelectKey)).CompareTo((int)X.GetParameter(SelectKey));
            });

            if (Res.Count <= UnitNum)
            {
                return Res;
            }
            else
            {
                for (int i = UnitNum; i < Res.Count; i++)
                {
                    Res[i] = null;
                    Res.Remove(Res[i]);
                }

                return Res;
            }
        }
        else if(SelectMode == 1)
        {
            // TODO 概率型降序排列
            // 从大到小排序
            for (int i = 0; i < ParaUnitList.Count; i++)
            {
                Res.Add(ParaUnitList[i]);
            }

            Res.Sort(delegate (Unit X, Unit Y)
            {
                return ((int)Y.GetParameter(SelectKey)).CompareTo((int)X.GetParameter(SelectKey));
            });

            if (Res.Count <= UnitNum)
            {
                return Res;
            }
            else
            {
                for (int i = UnitNum; i < Res.Count; i++)
                {
                    Res[i] = null;
                    Res.Remove(Res[i]);
                }

                return Res;
            }

        }
        else if(SelectMode == 0)
        {
            for (int i = 0; i < ParaUnitList.Count; i++)
            {
                Tmp.Add(ParaUnitList[i]);
            }

            int RandomUnitNum = (UnitNum > ParaUnitList.Count) ? ParaUnitList.Count : UnitNum;

            for(int i=0;i<RandomUnitNum;i++)
            {
                int RandomIndex = Random.Range(0, Tmp.Count - 1);

                Res.Add(Tmp[RandomIndex]);

                Tmp[RandomIndex] = null;
                Tmp.Remove(Tmp[RandomIndex]);
            }

            return Res;
        }
        else if(SelectMode == -1)
        {
            // TODO:概率型升序排列
            // 从小到大排序
            for (int i = 0; i < ParaUnitList.Count; i++)
            {
                Res.Add(ParaUnitList[i]);
            }

            Res.Sort(delegate (Unit X, Unit Y)
            {
                return ((int)X.GetParameter(SelectKey)).CompareTo((int)Y.GetParameter(SelectKey));
            });

            if (Res.Count <= UnitNum)
            {
                return Res;
            }
            else
            {
                for (int i = UnitNum; i < Res.Count; i++)
                {
                    Res[i] = null;
                    Res.Remove(Res[i]);
                }

                return Res;
            }
        }
        else if(SelectMode == -2)
        {
            // 从小到大排序
            for (int i = 0; i < ParaUnitList.Count; i++)
            {
                Res.Add(ParaUnitList[i]);
            }

            Res.Sort(delegate (Unit X, Unit Y)
            {
                return ((int)X.GetParameter(SelectKey)).CompareTo((int)Y.GetParameter(SelectKey));
            });

            if (Res.Count <= UnitNum)
            {
                return Res;
            }
            else
            {
                for (int i = UnitNum; i < Res.Count; i++)
                {
                    Res[i] = null;
                    Res.Remove(Res[i]);
                }

                return Res;
            }
        }

        return Res;
       
    }
	
}
