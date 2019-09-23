using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Xml;     //xml操作

public class Effect
{
    public int id = 0;

    static GameObject GameManager = null;

    static GameController MainGameController = null;

    static LibraryManager MainLibraryManager = null;

    static ResourceManager MainResourceManager = null;
    static CardManager MainCardManager = null;

    public void SetMainGameController(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
        MainLibraryManager = MainGameController.GetLibraryManager();
        MainResourceManager = MainGameController.GetResourceManager();
        MainCardManager = MainGameController.GetCardManager();
    }

    public LibraryManager GetLibraryInfoController()
    {
        return MainLibraryManager;
    }

    public ResourceManager GetResourceManager()
    {
        return MainResourceManager;
    }

    public CardManager GetCardManager()
    {
        return MainCardManager;
    }

    public int StringToInt(string s)
    {
        float Tmp = float.Parse(s);

        return (int)Tmp;
    }

    public Effect()
    {
        id = 0;
    }

    public virtual Effect ReloadNewEffect()
    {
        Debug.Log("Effect");

        return new Effect();
    }

    public virtual void LoadInfoFromXML(XmlElement XMLInfo)
    {

    }

    public virtual void ExcuteEffect()
    {
        Debug.Log("ExcuteEffect");
    }

    public virtual void ExcuteEffect(Unit DamageSource, Unit DamageTarget, BattleInfoSet ParaBattleInfoSet = null, BattleSequence ParaBattleSequence = null)
    {
        Debug.Log("ExcuteEffect");
    }

    public virtual void ExcuteEffect(Unit SourceUnit, GameObject SourcePiece, GameObject SourceCheckerBoard, Unit TargetUnit, GameObject TargetPiece, GameObject TargetCheckerBoard, BattleInfoSet ParaBattleInfoSet = null, BattleSequence ParaBattleSequence = null)
    {

    }

    public virtual object GetParameter(string ParaName)
    {
        return null;
    }

    public virtual void SetParameter(string ParaName, string ParaValue)
    {

    }

    public virtual void DebugDisplay()
    {

    }
}

public class DealDamage : Effect
{

    Dictionary<string, Parameter> ParameterOfName = new Dictionary<string, Parameter>();

    public DealDamage()
    {
        ParameterOfName.Add("EffectID", new StringParameter());
        ParameterOfName.Add("EffectName", new StringParameter());
        ParameterOfName.Add("DamageAmount", new FloatParameter());
        ParameterOfName.Add("ExecutionProbability", new FloatParameter());

        ParameterOfName["EffectID"].SetParameter("");
        ParameterOfName["EffectName"].SetParameter("");
        ParameterOfName["DamageAmount"].SetParameter("0");
        ParameterOfName["ExecutionProbability"].SetParameter("1");
    }

    public override Effect ReloadNewEffect()
    {
        //Debug.Log("DealDamage");

        return new DealDamage();
    }

    public override void LoadInfoFromXML(XmlElement XMLInfo)
    {
        foreach (XmlElement EffectSubInfoItem in XMLInfo.ChildNodes)
        {
            SetParameter(EffectSubInfoItem.Name, EffectSubInfoItem.InnerText);
        }
    }

    // 效果执行
    public override void ExcuteEffect(Unit DamageSource, Unit DamageTarget, BattleInfoSet ParaBattleInfoSet = null, BattleSequence ParaBattleSequence = null)
    {
        Debug.Log("ExcuteDealDamage");

        float tmp = (float)ParameterOfName["DamageAmount"].GetParameter();

        int DamageAmount = (int)tmp;

        DamageTarget.ReceiveDamage(DamageSource, DamageAmount);
    }

    public override object GetParameter(string ParaName)
    {
        if(ParameterOfName.ContainsKey(ParaName) == true)
        {
            return ParameterOfName[ParaName].GetParameter();
        }
        else
        {
            return null;
        }       
    }

    public override void SetParameter(string ParaName, string ParaValue)
    {
        if(ParameterOfName.ContainsKey(ParaName) == true)
        {
            ParameterOfName[ParaName].SetParameter(ParaValue);
        }
    }

    public override void DebugDisplay()
    {
        Debug.Log("&=============================================&");
        Debug.Log((string)GetParameter("EffectID") + "  " + (float)GetParameter("DamageAmount"));
        Debug.Log("&=============================================&");
    }
}

public class ApplyBehavior:Effect
{
    public override Effect ReloadNewEffect()
    {
        Debug.Log("Reload-ApplyBehavior");

        return new ApplyBehavior();
    }
}

public class RemoveBehavior:Effect
{
    public override Effect ReloadNewEffect()
    {
        Debug.Log("Reload-RemoveBehavior");

        return new RemoveBehavior();
    }
}

// 修改玩家效果
public class ModifyPlayer : Effect
{
    Dictionary<string, Parameter> ParameterOfName = new Dictionary<string, Parameter>();

    // 目前它只能修改玩家的资源和让玩家获取一张特定的卡牌
    // TODO: 完善此效果
    // 随机卡牌效果需要实现

    // 玩家卡牌修改列表字典
    Dictionary<string, List<string>> PlayerCardListOfType = new Dictionary<string, List<string>>();

    Dictionary<int, string> TypeOfIndex = new Dictionary<int, string>();

    public ModifyPlayer()
    {
        // 玩家手牌区增加的卡牌ID数组
        PlayerCardListOfType.Add("PlayerAddHandCard", new List<string>());
        TypeOfIndex.Add(0, "PlayerAddHandCard");
        // 玩家牌堆区增加的卡牌ID数组
        PlayerCardListOfType.Add("PlayerAddDeckCard", new List<string>());
        TypeOfIndex.Add(1, "PlayerAddDeckCard");
        // 玩家坟场区增加的卡牌ID数组
        PlayerCardListOfType.Add("PlayerAddGraveCard", new List<string>());
        TypeOfIndex.Add(2, "PlayerAddGraveCard");

        ParameterOfName.Add("EffectID", new StringParameter());
        ParameterOfName.Add("EffectName", new StringParameter());
        // 修改玩家选择 
        // TargetPlayer == 0 表示修改源单位所有者
        // TargetPlayer == 1 表示修改目标单位所有者
        ParameterOfName.Add("TargetPlayer", new IntParameter());
        ParameterOfName.Add("DeltaMineral", new IntParameter());
        ParameterOfName.Add("DeltaGas", new IntParameter());
        ParameterOfName.Add("DeltaMineralCapacity", new IntParameter());
        ParameterOfName.Add("DeltaGasCapacity", new IntParameter());
        ParameterOfName.Add("SourceUnitPercentage", new FloatParameter());
        ParameterOfName.Add("SourcePiecePercentage", new FloatParameter());
        ParameterOfName.Add("TargetUnitPercentage", new FloatParameter());
        ParameterOfName.Add("TargetPiecePercentage", new FloatParameter());

        ParameterOfName["EffectID"].SetParameter("");
        ParameterOfName["EffectName"].SetParameter("");
        ParameterOfName["TargetPlayer"].SetParameter("0");
        ParameterOfName["DeltaMineral"].SetParameter("0");
        ParameterOfName["DeltaGas"].SetParameter("0");
        ParameterOfName["DeltaMineralCapacity"].SetParameter("0");
        ParameterOfName["DeltaGasCapacity"].SetParameter("0");
        ParameterOfName["SourceUnitPercentage"].SetParameter("0");
        ParameterOfName["SourcePiecePercentage"].SetParameter("0");
        ParameterOfName["TargetUnitPercentage"].SetParameter("0");
        ParameterOfName["TargetPiecePercentage"].SetParameter("0");
    }

    public override Effect ReloadNewEffect()
    {
        Debug.Log("Reload-ModifyPlayer");

        return new ModifyPlayer();
    }

    public override void LoadInfoFromXML(XmlElement XMLInfo)
    {
        foreach (XmlElement EffectSubInfoItem in XMLInfo.ChildNodes)
        {
            SetParameter(EffectSubInfoItem.Name, EffectSubInfoItem.InnerText);

            // 读取玩家卡牌修改部分
            if(EffectSubInfoItem.Name == "PlayerAddHandCard" || EffectSubInfoItem.Name == "PlayerAddDeckCard" || EffectSubInfoItem.Name == "PlayerAddGraveCard")
            {
                string CardID = "";
                int CardNum = 0;
                foreach (XmlElement CardInfoItem in EffectSubInfoItem.ChildNodes)
                {
                    if (CardInfoItem.Name == "CardID")
                    {
                        CardID = CardInfoItem.InnerText;
                    }
                    else if(CardInfoItem.Name == "CardNum")
                    {
                        CardNum = StringToInt(CardInfoItem.InnerText);
                    }
                }

                for(int i=0;i<CardNum;i++)
                {
                    // 将卡牌ID加入到对应的卡牌ID数组当中
                    PlayerCardListOfType[EffectSubInfoItem.Name].Add(CardID);
                }

            }
            
        }
    }

    public override void ExcuteEffect(Unit DamageSource, Unit DamageTarget, BattleInfoSet ParaBattleInfoSet = null, BattleSequence ParaBattleSequence = null)
    {
        int PlayerID = -1;

        if ((int)GetParameter("TargetPlayer") == 0)
        {
            PlayerID = DamageSource.GetUnitOwner();
        }
        else
        {
            PlayerID = DamageTarget.GetUnitOwner();
        }
        
        int PlayerCurrentMineral = GetResourceManager().GetPlayerMineral(PlayerID);
        int PlayerCurrentGas = GetResourceManager().GetPlayerGas(PlayerID);

        int SourcePieceMineralCost = DamageSource.GetLocatePiece().GetComponentInChildren<PieceInfoController>().GetPieceCostMineral();
        int SourcePieceGasCost = DamageSource.GetLocatePiece().GetComponentInChildren<PieceInfoController>().GetPieceCostGas();

        int TargetPieceMineralCost = DamageTarget.GetLocatePiece().GetComponentInChildren<PieceInfoController>().GetPieceCostMineral();
        int TargetPieceGasCost = DamageTarget.GetLocatePiece().GetComponentInChildren<PieceInfoController>().GetPieceCostGas();

        PlayerCurrentMineral += (int)((float)GetParameter("SourcePiecePercentage") * SourcePieceMineralCost + (float)GetParameter("TargetPiecePercentage") * TargetPieceMineralCost + (int)GetParameter("DeltaMineral"));
        PlayerCurrentGas += (int)((float)GetParameter("SourcePiecePercentage") * SourcePieceGasCost + (float)GetParameter("TargetPiecePercentage") * TargetPieceGasCost + (int)GetParameter("DeltaGas"));

        // 更新玩家资源
        GetResourceManager().SetPlayerMineral(PlayerID, PlayerCurrentMineral);
        GetResourceManager().SetPlayerGas(PlayerID, PlayerCurrentGas);

        // 更新玩家卡牌
        // 修改玩家手牌区卡牌
        for(int i=0;i< PlayerCardListOfType["PlayerAddHandCard"].Count;i++)
        {
            Debug.Log("give me a hand card.");
            GetCardManager().PlayerGetAHandCard(PlayerID, GetCardManager().CreateACard(PlayerCardListOfType["PlayerAddHandCard"][i], PlayerID));
        }

        // 修改玩家牌堆区卡牌
        for (int i = 0; i < PlayerCardListOfType["PlayerAddDeckCard"].Count; i++)
        {
            GetCardManager().PlayerGetADeckCard(PlayerID, GetCardManager().CreateACard(PlayerCardListOfType["PlayerAddDeckCard"][i], PlayerID));
        }

        // 修改玩家坟场区卡牌
        for (int i = 0; i < PlayerCardListOfType["PlayerAddGraveCard"].Count; i++)
        {
            GetCardManager().PlayerGetAGraveCard(PlayerID, GetCardManager().CreateACard(PlayerCardListOfType["PlayerAddGraveCard"][i], PlayerID));
        }
    }

    public override object GetParameter(string ParaName)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            return ParameterOfName[ParaName].GetParameter();
        }
        else
        {
            return null;
        }
    }

    public override void SetParameter(string ParaName, string ParaValue)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            ParameterOfName[ParaName].SetParameter(ParaValue);
        }
    }
}

// 棋子内单位搜索效果
public class PieceUnitSearch:Effect
{
    Dictionary<string, Parameter> ParameterOfName = new Dictionary<string, Parameter>();

    UnitSelector EffectUnitSelector = null;

    // 搜索结果单位效果 即以每个位于搜索结果中的单位执行的效果
    Effect ResultUnitEffect = null;

    public PieceUnitSearch()
    {
        ParameterOfName.Add("EffectID", new StringParameter());
        ParameterOfName.Add("EffectName", new StringParameter());
        // 修改对象选择 
        // TargetSelect == 0 表示在源棋子中进行单位搜索
        // TargetSelect == 1 表示在目标棋子中进行单位搜索
        ParameterOfName.Add("TargetSelect", new IntParameter());
        // 该效果中搜索单位的上限
        ParameterOfName.Add("SearchUnitMaxNum", new IntParameter());
        // 搜索尺寸
        ParameterOfName.Add("SearchDimension", new FloatParameter());

        ParameterOfName["EffectID"].SetParameter("");
        ParameterOfName["EffectName"].SetParameter("");
        ParameterOfName["TargetSelect"].SetParameter("0");
        ParameterOfName["SearchUnitMaxNum"].SetParameter("0");
        ParameterOfName["SearchDimension"].SetParameter("0.0");
    }

    public override Effect ReloadNewEffect()
    {
        Debug.Log("Reload-ModifyPlayer");

        return new ModifyPlayer();
    }

    public override void LoadInfoFromXML(XmlElement XMLInfo)
    {
        foreach (XmlElement EffectSubInfoItem in XMLInfo.ChildNodes)
        {
            SetParameter(EffectSubInfoItem.Name, EffectSubInfoItem.InnerText);            
        }
    }

    public int CalculateSearchUnitNum()
    {
        return 1;
    }

    public override void ExcuteEffect(Unit SourceUnit, GameObject SourcePiece, GameObject SourceCheckerBoard, Unit TargetUnit, GameObject TargetPiece, GameObject TargetCheckerBoard, BattleInfoSet ParaBattleInfoSet = null, BattleSequence ParaBattleSequence = null)
    {
        GameObject CurrentPiece = null;

        if ((int)GetParameter("TargetSelect") == 0)
        {
            if(SourcePiece != null)
            {
                CurrentPiece = SourcePiece;
            }
            else if(SourceUnit != null)
            {
                CurrentPiece = SourceUnit.GetLocatePiece();
            }
            else
            {
                Debug.Log("No valid Source.");
                return;
            }
        }
        else
        {
            if (TargetPiece != null)
            {
                CurrentPiece = TargetPiece;
            }
            else if (TargetUnit != null)
            {
                CurrentPiece = TargetUnit.GetLocatePiece();
            }
            else
            {
                Debug.Log("No valid Target.");
                return;
            }
        }

        int ActualSearchUnitNum = 0;
        int PieceAliveUnitNum = CurrentPiece.GetComponent<PieceStateController>().GetAliveUnitNum();
        int SearchUnitMaxNum = (int)GetParameter("SearchUnitMaxNum");
        float SearchDimension = (float)GetParameter("SearchDimension");

        // 计算实际需要搜索的单位数
        if (SearchUnitMaxNum == -1)
        {
            // 如果上限为-1则完全根据搜索尺寸进行单位实际搜索数目计算
            if(SearchDimension == -1)
            {
                // 如果搜索尺寸为-1则实际搜索单位数目为该棋子内所有存活单位数目
                ActualSearchUnitNum = PieceAliveUnitNum;
            }
            else
            {
                // 计算实际搜索单位数目
                ActualSearchUnitNum = CalculateSearchUnitNum();
            }
        }
        else
        {
            // 如果上限不为1则先计算搜索结果再与上限比较大小
            if((float)GetParameter("SearchDimension") == -1)
            {
                // 如果搜索尺寸为-1则实际搜索单位数目为该棋子内所有存活单位数目
                ActualSearchUnitNum = (PieceAliveUnitNum < SearchUnitMaxNum) ? PieceAliveUnitNum : SearchUnitMaxNum;
            }
            else
            {
                int TmpUnitNum = CalculateSearchUnitNum();

                ActualSearchUnitNum = (PieceAliveUnitNum < TmpUnitNum) ? PieceAliveUnitNum : TmpUnitNum;
                ActualSearchUnitNum = (ActualSearchUnitNum < SearchUnitMaxNum) ? ActualSearchUnitNum : SearchUnitMaxNum;
            }
        }


        // 使用效果链接的单位筛选器从操作棋子中选择 SearchUnitNum 数量的单位
        List<Unit> SearchUnitList = EffectUnitSelector.ExecuteSelection( CurrentPiece.GetComponent<PieceStateController>().GetAllAlivePieceUnit(), (int)GetParameter("SearchUnitNum") );

        // 对每个搜索结果中的棋子执行搜索结果单位效果
        for(int i=0;i< SearchUnitList.Count;i++)
        {
            ResultUnitEffect.ExcuteEffect(SourceUnit, SourcePiece, SourceCheckerBoard, SearchUnitList[i], TargetPiece, TargetCheckerBoard, ParaBattleInfoSet, ParaBattleSequence);
        }
    }

    public override object GetParameter(string ParaName)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            return ParameterOfName[ParaName].GetParameter();
        }
        else
        {
            return null;
        }
    }

    public override void SetParameter(string ParaName, string ParaValue)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            ParameterOfName[ParaName].SetParameter(ParaValue);
        }
    }
}

public class Parameter
{
    object Value;

    public virtual void SetParameter(object NewValue)
    {
        Value = NewValue;
    }

    public virtual object GetParameter()
    {
        return Value;
    }

    public virtual void AddParameter(Parameter ParaParameter)
    {

    }

    public virtual void MinusParameter(Parameter ParaParameter)
    {

    }
}

public class IntParameter:Parameter
{
    int IntValue = 0;

    public override void SetParameter(object NewValue)
    {
        // 这里必须保证输入参数NewValue是string
        string tmp = (string)NewValue;
        IntValue = (int)float.Parse(tmp);
    }

    public override object GetParameter()
    {
        return IntValue;
    }

    public override void AddParameter(Parameter ParaParameter)
    {
        // 这里必须保证输入参数NewValue是string
        int Tmp = (int)ParaParameter.GetParameter();

        IntValue += Tmp;
    }

    public override void MinusParameter(Parameter ParaParameter)
    {
        // 这里必须保证输入参数NewValue是string
        int Tmp = (int)ParaParameter.GetParameter();

        IntValue -= Tmp;
    }
}

public class FloatParameter : Parameter
{
    float FloatValue = 0.0f;

    public override void SetParameter(object NewValue)
    {
        // 这里必须保证输入参数NewValue是string
        string tmp = (string)NewValue;
        FloatValue = float.Parse(tmp);
    }

    public override object GetParameter()
    {
        return FloatValue;
    }

    public override void AddParameter(Parameter ParaParameter)
    {
        // 这里必须保证输入参数NewValue是string
        float Tmp = (float)ParaParameter.GetParameter();

        FloatValue += Tmp;
    }

    public override void MinusParameter(Parameter ParaParameter)
    {
        // 这里必须保证输入参数NewValue是string
        float Tmp = (float)ParaParameter.GetParameter();

        FloatValue -= Tmp;
    }
}

public class StringParameter : Parameter
{
    string StringValue = "";

    public override void SetParameter(object NewValue)
    {
        // 这里必须保证输入参数NewValue是string
        StringValue = (string)NewValue;
    }

    public override object GetParameter()
    {
        return StringValue;
    }
}

public class BoolParameter : Parameter
{
    bool BoolValue = false;

    public override void SetParameter(object NewValue)
    {
        // 这里必须保证输入参数NewValue是bool
        string ObjectString = (string)NewValue;
        if(ObjectString == "true")
        {
            BoolValue = true;
        }
        else if(ObjectString == "false")
        {
            BoolValue = false;
        }
        else
        {
            Debug.Log("Bool Value Set Error.");
        }
    }

    public override object GetParameter()
    {
        return BoolValue;
    }
}