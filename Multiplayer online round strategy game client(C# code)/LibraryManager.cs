using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Xml;     //xml操作
using System.IO;     //file操作

public class LibraryManager
{
    //

    // 游戏控制器引用
    GameController MainGameController = null;

    // 游戏棋子底座模板库
    Dictionary<string, PiecePedestalInfo> PiecePedestalInfoLibrary = new Dictionary<string, PiecePedestalInfo>();

    // 游戏棋子雕像模板库
    Dictionary<string, PieceStatusInfo> PieceStatusInfoLibrary = new Dictionary<string, PieceStatusInfo>();

    // 游戏效果库
    Dictionary<string, Effect> EffectLibrary = new Dictionary<string, Effect>();

    // 游戏单位模板库
    Dictionary<string, Unit> UnitLibrary = new Dictionary<string, Unit>();

    // 游戏棋子模板库
    Dictionary<string, Piece> PieceLibrary = new Dictionary<string, Piece>();

    // 游戏武器模板库
    Dictionary<string, Weapon> WeaponLibrary = new Dictionary<string, Weapon>();

    // 游戏卡牌模板库
    Dictionary<string, Card> CardLibrary = new Dictionary<string, Card>();

    // 游戏卡牌ID库(使用卡牌的四位CTN来获得卡牌的ID)
    Dictionary<string, string> CardIDLibrary = new Dictionary<string, string>();


    public LibraryManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
    } 

    public void EstablishReference()
    {

    }

    // 向棋子底座模板库中添加一个新棋子底座模板
    public void AddPiecePedestalInfo(PiecePedestalInfo NewPiecePedestalInfo)
    {
        if(!PiecePedestalInfoLibrary.ContainsKey(NewPiecePedestalInfo.GetPiecePedestalID()))
        {
            PiecePedestalInfoLibrary.Add(NewPiecePedestalInfo.GetPiecePedestalID(), NewPiecePedestalInfo);
        }
        else
        {
            Debug.LogError("Same PiecePedestalInfo has existed in library.");
        }
        
    }

    // 向棋子雕像模板库中添加一个新棋子雕像模板
    public void AddPieceStatusInfo(PieceStatusInfo NewPieceStatusInfo)
    {
        if(!PieceStatusInfoLibrary.ContainsKey(NewPieceStatusInfo.GetPieceStatusID()))
        {
            PieceStatusInfoLibrary.Add(NewPieceStatusInfo.GetPieceStatusID(), NewPieceStatusInfo);
        }
        else
        {
            Debug.LogError("Same PieceStatusInfo has existed in library.");
        }
        
    }

    // 向效果库中添加一个新效果
    public void AddEffect(Effect NewEffect)
    {
        if(!EffectLibrary.ContainsKey((string)NewEffect.GetParameter("EffectID"))) { EffectLibrary.Add((string)NewEffect.GetParameter("EffectID"), NewEffect); }
        else { Debug.LogError("Same Effect has existed in library."); }       
    }

    // 向单位模板库中添加一个新单位模板
    public void AddUnit(Unit NewUnit)
    {
        if(!UnitLibrary.ContainsKey(NewUnit.GetUnitID())) { UnitLibrary.Add(NewUnit.GetUnitID(), NewUnit); }
        else { Debug.LogError("Same Unit has existed in library."); }
    }

    // 向棋子模板库中添加一个新棋子模板
    public void AddPiece(Piece NewPiece)
    {
        if (!PieceLibrary.ContainsKey(NewPiece.GetPieceID())) { PieceLibrary.Add(NewPiece.GetPieceID(), NewPiece); }
        else { Debug.LogError("Same Piece has existed in library."); }
    }

    // 向武器库中添加一个新武器
    public void AddWeapon(Weapon NewWeapon)
    {
        if (!WeaponLibrary.ContainsKey(NewWeapon.GetWeaponID())) { WeaponLibrary.Add(NewWeapon.GetWeaponID(), NewWeapon); }
        else { Debug.LogError("Same Weapon has existed in library."); }        
    }

    // 向卡牌模板库中添加一个新卡牌模板
    public void AddCard(Card NewCard)
    {
        if (!CardLibrary.ContainsKey((string)NewCard.GetParameter("CardID")))
        {
            string NewCardID = (string)NewCard.GetParameter("CardID");
            string NewCardCTN = (string)NewCard.GetParameter("CardCTN");
            CardLibrary.Add(NewCardID, NewCard);
            CardIDLibrary.Add(NewCardCTN, NewCardID);
        }
        else { Debug.LogError("Same Card has existed in library."); }
    }

    // 根据棋子底座ID获取棋子底座模板
    public PiecePedestalInfo GetPiecePedestalInfo(string PiecePedestalInfoID) { return PiecePedestalInfoLibrary[PiecePedestalInfoID]; }

    // 根据棋子雕像ID获取棋子雕像模板
    public PieceStatusInfo GetPieceStatusInfo(string PieceStatusInfoID) { return PieceStatusInfoLibrary[PieceStatusInfoID]; }

    // 根据武器ID获取武器
    public Weapon GetWeapon(string WeaponID) { return WeaponLibrary[WeaponID]; }

    // 根据单位ID获取单位模板
    public Unit GetUnit(string UnitID) { return UnitLibrary[UnitID]; }

    // 根据棋子ID获取棋子模板
    public Piece GetPiece(string PieceID) { return PieceLibrary[PieceID]; }

    // 根据效果ID获取效果
    public Effect GetEffect(string EffectID) { return EffectLibrary[EffectID]; }

    // 根据卡牌模板ID获取卡牌模板
    public Card GetCard(string CardID) { return CardLibrary[CardID]; }

    // 根据卡牌模板CTN获取卡牌模板
    public Card GetCardByCardCTN(string CardCTN) { return CardLibrary[CardIDLibrary[CardCTN]]; }

    // 从XML中载入棋子底座模板信息
    public void LoadPedestalFromXML()
    {
        string filePath = Application.dataPath + "/Resources/Pedestal.xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("PiecePedestal").ChildNodes;
            foreach (XmlElement PiecePedestalInfo in node)
            {
                PiecePedestalInfo NewPiecePedestal = new PiecePedestalInfo();

                foreach (XmlElement PiecePedestalItem in PiecePedestalInfo.ChildNodes)
                {
                    if (PiecePedestalItem.Name == "PiecePedestalID")
                    {
                        //Debug.Log(PiecePedestalItem.InnerText);
                        NewPiecePedestal.SetPiecePedestalID(PiecePedestalItem.InnerText);
                        NewPiecePedestal.SetPedestalPrefab(PiecePedestalItem.InnerText);
                    }
                    else if (PiecePedestalItem.Name == "PiecePedestalName")
                    {
                        NewPiecePedestal.SetPiecePedestalName(PiecePedestalItem.InnerText);
                    }
                    else if (PiecePedestalItem.Name == "Position")
                    {
                        NewPiecePedestal.SetLocalPosition(PublicUtility.StringToVector3(PiecePedestalItem.InnerText));
                    }
                    else if (PiecePedestalItem.Name == "Scale")
                    {
                        NewPiecePedestal.SetLocalScale(PublicUtility.StringToVector3(PiecePedestalItem.InnerText));
                        //Debug.Log(StringToVector3(PiecePedestalItem.InnerText));
                    }
                    else if (PiecePedestalItem.Name == "Rotation")
                    {
                        NewPiecePedestal.SetLocalRotation(PublicUtility.StringToVector3(PiecePedestalItem.InnerText));
                    }
                }

                AddPiecePedestalInfo(NewPiecePedestal);
            }
        }
    }

    // 从XML中载入棋子雕像模板信息
    public void LoadStatusFromXML()
    {
        string filePath = Application.dataPath + "/Resources/Status.xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("PieceStatus").ChildNodes;
            foreach (XmlElement PieceStatusInfo in node)
            {
                PieceStatusInfo NewPieceStatus = new PieceStatusInfo();

                foreach (XmlElement PieceStatusItem in PieceStatusInfo.ChildNodes)
                {
                    if (PieceStatusItem.Name == "PieceStatusID")
                    {
                        //Debug.Log(PieceStatusItem.InnerText);
                        NewPieceStatus.SetPieceStatusID(PieceStatusItem.InnerText);
                        NewPieceStatus.SetStatusPrefab(PieceStatusItem.InnerText);
                    }
                    else if (PieceStatusItem.Name == "PieceStatusName")
                    {
                        NewPieceStatus.SetPieceStatusName(PieceStatusItem.InnerText);
                    }
                    else if (PieceStatusItem.Name == "Position")
                    {
                        //NewPieceStatus.SetLocalPosition(StringToVector3(PieceStatusItem.InnerText));
                    }
                    else if (PieceStatusItem.Name == "Scale")
                    {
                        NewPieceStatus.SetLocalScale(PublicUtility.StringToVector3(PieceStatusItem.InnerText));
                        //Debug.Log(StringToVector3(PieceStatusItem.InnerText));
                    }
                    else if (PieceStatusItem.Name == "Rotation")
                    {
                        NewPieceStatus.SetLocalRotation(PublicUtility.StringToVector3(PieceStatusItem.InnerText));
                    }
                }

                AddPieceStatusInfo(NewPieceStatus);
            }
        }
    }

    // 从XML中载入效果信息
    public void LoadEffectFromXML()
    {
        // 为Effect类以及全部子类初始化静态变量
        Effect InitEffect = new Effect();
        InitEffect.SetMainGameController(MainGameController);

        string filePath = Application.dataPath + "/Resources/Effect.xml";

        EffectLoader NewEffectLoader = new EffectLoader();

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("Effect").ChildNodes;
            foreach (XmlElement EffectInfo in node)
            {
                Effect NewEffect = null;

                foreach (XmlElement EffectInfoItem in EffectInfo.ChildNodes)
                {
                    if (EffectInfoItem.Name == "EffectType")
                    {
                        NewEffect = NewEffectLoader.GetNewEffectByType(EffectInfoItem.InnerText);
                    }
                    else if (EffectInfoItem.Name == "EffectSubInfo")
                    {
                        NewEffect.LoadInfoFromXML(EffectInfoItem);
                    }

                }
                AddEffect(NewEffect);
            }
        }
    }

    // 从XML中载入卡牌模板信息
    public void LoadCardFromXML()
    {
        Card InitCard = new Card();
        InitCard.SetMainGameController(MainGameController);

        CardLoader NewCardLoader = new CardLoader();

        string filePath = Application.dataPath + "/Resources/Card.xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("Card").ChildNodes;
            foreach (XmlElement CardInfo in node)
            {
                Card NewCard = null;

                foreach (XmlElement CardInfoItem in CardInfo.ChildNodes)
                {
                    if (CardInfoItem.Name == "CardType")
                    {
                        NewCard = NewCardLoader.GetNewCardByType(CardInfoItem.InnerText);
                    }
                    else if (CardInfoItem.Name == "CardSubInfo")
                    {
                        NewCard.LoadInfoFromXML(CardInfoItem);
                    }
                }
                AddCard(NewCard);
            }
        }
    }

    // 从XML中载入武器信息
    public void LoadWeaponInfoFromXML()
    {
        string filePath = Application.dataPath + "/Resources/Weapon.xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("Weapon").ChildNodes;
            foreach (XmlElement WeaponInfo in node)
            {
                //Debug.Log("333");
                //1. 在内存中创建一个新的Weapon实例
                Weapon NewWeapon = new Weapon();

                //2. 将XML中的信息拷贝到该Weapon实例中
                foreach (XmlElement WeaponInfoItem in WeaponInfo.ChildNodes)
                {
                    if (WeaponInfoItem.Name == "WeaponID")
                    {
                        // 读取并设置新武器的武器ID
                        NewWeapon.SetWeaponID(WeaponInfoItem.InnerText);
                    }
                    else if (WeaponInfoItem.Name == "WeaponName")
                    {
                        // 读取并设置新武器的武器射程
                        NewWeapon.SetWeaponName(WeaponInfoItem.InnerText);
                    }
                    else if (WeaponInfoItem.Name == "WeaponRange")
                    {
                        // 读取并设置新武器的武器射程
                        NewWeapon.SetWeaponRange(PublicUtility.StringToFloat(WeaponInfoItem.InnerText));
                    }
                    else if (WeaponInfoItem.Name == "WeaponAttackInterval")
                    {
                        // 读取并设置新武器的武器开火间隔
                        NewWeapon.SetAttackInterval(PublicUtility.StringToFloat(WeaponInfoItem.InnerText));
                    }
                    else if (WeaponInfoItem.Name == "WeaponAttackEffect")
                    {
                        NewWeapon.SetWeaponAttackEffect(GetEffect(WeaponInfoItem.InnerText));
                    }
                }
                //3. 将新武器添加到武器字典中
                AddWeapon(NewWeapon);
            }
        }
    }

    // 从XML中载入单位模板信息
    public void LoadUnitInfoFromXML()
    {
        string filePath = Application.dataPath + "/Resources/Unit.xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("Unit").ChildNodes;
            foreach (XmlElement UnitInfo in node)
            {
                //1. 在内存中创建一个新的Unit实例
                Unit NewUnit = new Unit();

                //2. 将XML中的信息拷贝到该Unit实例中
                foreach (XmlElement UnitInfoItem in UnitInfo.ChildNodes)
                {
                    if (UnitInfoItem.Name == "UnitID")
                    {
                        // 读取并设置新单位的单位名称
                        NewUnit.SetUnitID(UnitInfoItem.InnerText);
                    }
                    else if (UnitInfoItem.Name == "UnitName")
                    {
                        // 读取并设置新单位的单位名称
                        NewUnit.SetUnitName(UnitInfoItem.InnerText);
                    }
                    else if (UnitInfoItem.Name == "UnitMaxHealth")
                    {
                        // 读取并设置新单位的单位最大生命值和当前生命值
                        NewUnit.SetMaxHealth(PublicUtility.StringToInt(UnitInfoItem.InnerText));
                        NewUnit.SetCurrentHealth(PublicUtility.StringToInt(UnitInfoItem.InnerText));
                    }
                    else if (UnitInfoItem.Name == "UnitSize")
                    {
                        // 读取并设置新单位的单位尺寸
                        //Debug.Log(UnitInfoItem.InnerText);
                        NewUnit.SetUnitSize(PublicUtility.StringToFloat(UnitInfoItem.InnerText));
                    }
                    else if (UnitInfoItem.Name == "UnitWeaponList")
                    {
                        // 读取并设置新单位的单位武器列表
                        foreach (XmlElement WeaponListItem in UnitInfoItem.ChildNodes)
                        {
                            foreach (XmlElement WeaponInfoItem in WeaponListItem.ChildNodes)
                            {
                                if (WeaponInfoItem.Name == "WeaponID")
                                {
                                    //Debug.Log(WeaponInfoItem.InnerText);
                                    NewUnit.AddWeapon(GetWeapon(WeaponInfoItem.InnerText));
                                }
                            }
                        }
                    }
                }
                //3. 将该Unit实例的引用添加到Library引用数组当中
                AddUnit(NewUnit);
            }
        }
    }

    // 从XML中载入棋子模板信息
    public void LoadPieceInfoFromXML()
    {
        string filePath = Application.dataPath + "/Resources/Piece.xml";

        if (File.Exists(filePath))
        {
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(filePath);
            XmlNodeList node = xmlDoc.SelectSingleNode("Piece").ChildNodes;
            foreach (XmlElement PieceInfo in node)
            {
                //item下面的节点
                //Debug.Log(ele.Name);

                Piece NewPiece = new Piece();

                NewPiece.SetPiecePedestalInfo(GetPiecePedestalInfo("TerrainePedestal1"));

                NewPiece.SetPieceStatusInfo(GetPieceStatusInfo("BanelingCommando"));

                foreach (XmlElement PieceInfoItem in PieceInfo.ChildNodes)
                {
                    //Debug.Log(i1.Name);
                    if (PieceInfoItem.Name == "PieceID")
                    {
                        NewPiece.SetPieceID(PieceInfoItem.InnerText);
                    }
                    else if (PieceInfoItem.Name == "PieceName")
                    {
                        NewPiece.SetPieceName(PieceInfoItem.InnerText);
                    }
                    else if (PieceInfoItem.Name == "PieceMineralCost")
                    {
                        NewPiece.SetPieceMineralCost(PublicUtility.StringToInt(PieceInfoItem.InnerText));
                    }
                    else if (PieceInfoItem.Name == "PieceGasCost")
                    {
                        NewPiece.SetPieceGasCost(PublicUtility.StringToInt(PieceInfoItem.InnerText));
                    }
                    else if (PieceInfoItem.Name == "PieceRetreatThreshold")
                    {
                        // 设置棋子撤退阈值
                        NewPiece.SetRetreatThreshold(PublicUtility.StringToFloat(PieceInfoItem.InnerText));
                    }
                    else if (PieceInfoItem.Name == "PiecePedestalID")
                    {
                        //Debug.Log(PieceInfoItem.InnerText);
                        NewPiece.SetPiecePedestalInfo(GetPiecePedestalInfo(PieceInfoItem.InnerText));
                    }
                    else if (PieceInfoItem.Name == "PieceStatusID")
                    {
                        //Debug.Log(PieceInfoItem.InnerText);
                        NewPiece.SetPieceStatusInfo(GetPieceStatusInfo(PieceInfoItem.InnerText));
                    }
                    else if (PieceInfoItem.Name == "PieceDeathEffectID")
                    {
                        //Debug.Log(PieceInfoItem.InnerText);
                        NewPiece.SetPieceDeathEffect(GetEffect(PieceInfoItem.InnerText));
                    }
                    else if (PieceInfoItem.Name == "PieceIncludeUnit")
                    {
                        string ContainUnitID = "";
                        int ContainUnitNum = 0;

                        foreach (XmlElement IncludeUnitItem in PieceInfoItem.ChildNodes)
                        {
                            if (IncludeUnitItem.Name == "UnitID")
                            {
                                //Debug.Log(IncludeUnitItem.InnerText);
                                ContainUnitID = IncludeUnitItem.InnerText;
                            }
                            else if (IncludeUnitItem.Name == "UnitNum")
                            {
                                //Debug.Log(IncludeUnitItem.InnerText);
                                ContainUnitNum = PublicUtility.StringToInt(IncludeUnitItem.InnerText);
                            }
                        }
                        NewPiece.LoadUnitInfo(GetUnit(ContainUnitID), ContainUnitNum);
                    }
                    else if (PieceInfoItem.Name == "PieceDescription")
                    {
                        foreach (XmlElement PieceDescriptionItem in PieceInfoItem.ChildNodes)
                        {
                            if (PieceDescriptionItem.Name == "PieceDescriptionItem")
                            {
                                //Debug.Log(PieceDescriptionItem.InnerText);
                                NewPiece.AddCardDescriptionItem(PieceDescriptionItem.InnerText);
                            }
                        }
                    }

                }
                // 将新加载的棋子添加到游戏棋子模板库当中
                AddPiece(NewPiece);
            }

        }
    }

    public void LoadGameDataFromXML()
    {
        LoadPedestalFromXML();

        LoadStatusFromXML();

        LoadEffectFromXML();

        LoadWeaponInfoFromXML();

        LoadUnitInfoFromXML();

        LoadPieceInfoFromXML();

        LoadCardFromXML();
    }


}
