using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Piece
{
    // 棋子ID(每个棋子各不相同)
    string PieceID = "";

    // 棋子名称(可以重复)
    string PieceName = "";

    // 棋子晶体矿花费
    int PieceMineralCost = 0;

    // 棋子气体花费
    int PieceGasCost = 0;

    // 棋子撤退阈值
    float RetreatThreshold = 0.25f;

    // 当前棋子内所有单位的引用字典
    //Dictionary<string, List<Unit>> PieceIncludeUnit = null;

    Dictionary<int, string> UnitNameOfIndex = null;

    Dictionary<string, Unit> UnitTamplate = null;

    Dictionary<string, int> UnitNum = null;

    int UnitTypeNum = 0;

    PiecePedestalInfo MainPiecePedestalInfo = null;

    PieceStatusInfo MainPieceStatusInfo = null;

    Effect PieceDeathEffect = null;

    // 棋子对应的卡牌上使用的图片(230 X 170)
    Texture CardMainImage = null;

    // 棋子在侧边棋子栏使用的图片(70 X 70)
    Texture UnitIconImage = null;

    List<string> CardDescriptionItemList = null;

    public Piece()
    {
        UnitNameOfIndex = new Dictionary<int, string>();
        UnitTamplate = new Dictionary<string, Unit>();
        UnitNum = new Dictionary<string, int>();
        CardDescriptionItemList = new List<string>();
    }

    public void SetPieceID(string ParaPieceID)
    {
        PieceID = ParaPieceID;
        CardMainImage = (Texture)Resources.Load("CardMainImage/" + PieceID);
        UnitIconImage = (Texture)Resources.Load("CardMainImage/" + PieceID);
    }

    public string GetPieceID()
    {
        return PieceID;
    }

    public Texture GetCardMainImage()
    {
        return CardMainImage;
    }

    public Texture GetUnitIconImage()
    {
        return UnitIconImage;
    }



    public void SetPieceName(string ParaPieceName)
    {
        PieceName = ParaPieceName;
    }

    public void SetPieceMineralCost(int NewPieceMineralCost)
    {
        PieceMineralCost = NewPieceMineralCost;
    }

    public void SetPieceGasCost(int NewPieceGasCost)
    {
        PieceGasCost = NewPieceGasCost;
    }

    // 获取棋子撤退阈值
    public float GetRetreatThreshold()
    {
        return RetreatThreshold;
    }

    // 设置棋子撤退阈值
    public void SetRetreatThreshold(float NewRetreatThreshold)
    {
        RetreatThreshold = NewRetreatThreshold;
    }

    public void SetPiecePedestalInfo(PiecePedestalInfo NewPiecePedestalInfo)
    {
        MainPiecePedestalInfo = NewPiecePedestalInfo;
    }

    public void SetPieceStatusInfo(PieceStatusInfo NewPieceStatusInfo)
    {
        MainPieceStatusInfo = NewPieceStatusInfo;
    }

    public void SetPieceDeathEffect(Effect NewPieceDeathEffect)
    {
        PieceDeathEffect = NewPieceDeathEffect;
    }

    public void AddCardDescriptionItem(string NewCardDescriptionItem)
    {
        CardDescriptionItemList.Add(NewCardDescriptionItem);
    }

    public List<string> GetCardDescriptionItemList()
    {
        return CardDescriptionItemList;
    }

    public string GetPieceName()
    {
        return PieceName;
    }

    public int GetPieceMineralCost()
    {
        return PieceMineralCost;
    }

    public int GetPieceGasCost()
    {
        return PieceGasCost;
    }

    public int GetUnitTypeNum()
    {
        return UnitTypeNum;
    }

    public int GetUnitNum(string ParaUnitName)
    {
        return UnitNum[ParaUnitName];
    }

    public int GetUnitNum(int index)
    {
        return UnitNum[UnitNameOfIndex[index]];
    }

    public Unit GetUnitTamplate(string ParaUnitName)
    {
        return UnitTamplate[ParaUnitName];
    }

    public Unit GetUnitTamplate(int index)
    {
        return UnitTamplate[UnitNameOfIndex[index]];
    }

    public void LoadUnitInfo(Unit ParaUnit, int ParaUnitNum)
    {
        string ParaUnitID = ParaUnit.GetUnitID();

        UnitNameOfIndex.Add(UnitTypeNum, ParaUnitID);

        UnitTamplate.Add(ParaUnitID, ParaUnit);
        UnitNum.Add(ParaUnitID, ParaUnitNum);

        UnitTypeNum++;
    }

    public PiecePedestalInfo GetPiecePedestalInfo()
    {
        return MainPiecePedestalInfo;
    }

    public PieceStatusInfo GetPieceStatusInfo()
    {
        return MainPieceStatusInfo;
    }

    public Effect GetPieceDeathEffect()
    {
        return PieceDeathEffect;
    }
}
