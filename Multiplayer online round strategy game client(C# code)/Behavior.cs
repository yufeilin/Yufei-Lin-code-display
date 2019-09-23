using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Behavior
{
    static LibraryManager MainLibraryManager = null;

    public void SetLibraryInfoController(LibraryManager NewLibraryManager)
    {
        MainLibraryManager = NewLibraryManager;
    }

    public LibraryManager GetLibraryInfoController()
    {
        return MainLibraryManager;
    }

    public virtual void LoadBehaviorOnPiece(GameObject ParaPiece)
    {

    }

    public virtual void LoadBehaviorOnUnit(Unit ParaUnit)
    {

    }

    public virtual void UnLoadBehaviorOnUnit(Unit ParaUnit)
    {

    }
}


public class BuffBehavior:Behavior
{

    Dictionary<string, Parameter> ParameterOfName = new Dictionary<string, Parameter>();

    Dictionary<int, string> NameOfIndex = new Dictionary<int, string>();

    int ParameterNum = 0;

    public BuffBehavior()
    {
        ParameterOfName.Add("MaxHealth", new IntParameter());
        ParameterOfName.Add("CurrentHealth", new IntParameter());
        ParameterOfName.Add("MaxEnergy", new IntParameter());
        ParameterOfName.Add("CurrentEnergy", new IntParameter());
        ParameterOfName.Add("MaxShield", new IntParameter());
        ParameterOfName.Add("CurrentShield", new IntParameter());

        NameOfIndex.Add(0, "MaxHealth");
        NameOfIndex.Add(1, "CurrentHealth");
        NameOfIndex.Add(2, "MaxEnergy");
        NameOfIndex.Add(3, "CurrentEnergy");
        NameOfIndex.Add(4, "MaxShield");
        NameOfIndex.Add(5, "CurrentShield");


        ParameterOfName["MaxHealth"].SetParameter("0");
        ParameterOfName["CurrentHealth"].SetParameter("0");
        ParameterOfName["MaxEnergy"].SetParameter("0");
        ParameterOfName["CurrentEnergy"].SetParameter("0");
        ParameterOfName["MaxShield"].SetParameter("0");
        ParameterOfName["CurrentShield"].SetParameter("0");

        ParameterNum = 6;
    }

    // 当行为被加载到单位身上时对单位的参数进行修改
    public override void LoadBehaviorOnUnit(Unit ParaUnit)
    {
        for(int i=0;i< ParameterNum;i++)
        {
            ParaUnit.GetParameterObject(NameOfIndex[i]).AddParameter(ParameterOfName[NameOfIndex[i]]);
        }
    }

    // 当行为从单位身上卸载时还原对单位参数的修改
    public override void UnLoadBehaviorOnUnit(Unit ParaUnit)
    {
        for (int i = 0; i < ParameterNum; i++)
        {
            ParaUnit.GetParameterObject(NameOfIndex[i]).MinusParameter(ParameterOfName[NameOfIndex[i]]);
        }
    }

}
