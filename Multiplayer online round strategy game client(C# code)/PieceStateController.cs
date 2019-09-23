using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PieceStateController : MonoBehaviour
{
    // 棋子当前是否被允许移动(一些状态如冰冻会使得该棋子在当前回合中无法移动)
    public bool MoveEnable = true;

    public bool IsAttackTarget = false;

    // 棋子朝向(绝大多数棋子是全向的，但是有部分特殊棋子是有朝向设定的)
    // 朝向 = -1 该棋子为全向棋子，不具有朝向设定
    // 朝向 = 1 为世界坐标 Z轴 正方向
    // 朝向 = 2 为世界坐标 X轴 正方向
    // 朝向 = 3 为世界坐标 Z轴 负方向
    // 朝向 = 4 为世界坐标 X轴 负方向
    public int Orientation = -1;

    // 棋子所有者
    // Owner = 0 棋子所有者为中立(玩家0)
    // Owner = 1 棋子所有者为玩家1
    // 以此类推
    int Owner = 0;

    // 棋子所使用的材质
    public Material PieceMaterial;

    public Vector3 ChessBoardInternalPosition = new Vector3();

    public Vector2Int ChessBoardInternalCoordinate = new Vector2Int();

    public Vector2Int ChessBoardOuterCoordinate = new Vector2Int();

    // 当前棋子所在的棋盘的引用
    public GameObject LocateChessboard = null;

    // 当前棋子中所包含的单位引用数组
    //List<Unit> PieceContainUnit = new List<Unit>();

    Dictionary<string, List<Unit>> ContainUnitsOfUnitName = new Dictionary<string, List<Unit>>();
    Dictionary<int, string> UnitNameOfIndex = new Dictionary<int, string>();
    int UnitTypeNum = 0;

    // 棋子在它所在棋盘格的层数
    public int Layer = 0;

    // 棋子当前是否处于战斗中
    bool InBattle = false;

    // 棋子的撤退阈值
    float RetreatThreshold = 0.25f;

    // 该棋子对应的手牌实例地引用
    Card RelativeCard = null;

    // 棋子使用的棋子底座配置信息
    PiecePedestalInfo MainPiecePedestalInfo = null;

    // 棋子使用的底座物体引用
    GameObject MainPiecePedestal = null;

    // 棋子使用的棋子雕像配置信息
    PieceStatusInfo MainPieceStatusInfo = null;

    // 棋子使用的雕像物体引用
    GameObject MainPieceStatus = null;

    Effect PieceDeathEffect = null;

    // 棋子对应的卡牌上使用的图片(230 X 170)
    Texture CardMainImage = null;

    // 棋子在侧边棋子栏使用的图片(70 X 70)
    Texture UnitIconImage = null;

    List<string> CardDescriptionItemList = null;

    // 当前棋子是否存活
    bool IsAlive = true;

    // 战斗临时变量(这些变量会在战斗序列开始和结束的时候进行更新)
    //---------------------------------------------------

    float CurrentPieceHealthBeforeBattle = 0;

    //---------------------------------------------------

    public void SetPiecePedestalInfo(PiecePedestalInfo NewPiecePedestalInfo)
    {
        MainPiecePedestalInfo = NewPiecePedestalInfo;
    }

    // 通过一个棋子模板初始化一个棋子物体的state组件
    public void Initialize(GameObject ParaChessBoard, Vector2Int InterCoord, Piece PieceTamplate)
    {
        // 为棋子物体的state组件初始化所有位置信息
        ChessBoardInternalCoordinate = InterCoord;
        ChessBoardInternalPosition = ParaChessBoard.GetComponent<ChessBoardController>().GetCheckerboardPosition(InterCoord);
        ChessBoardOuterCoordinate = ParaChessBoard.GetComponent<ChessBoardController>().ChessBoardOuterCoordinate;
        LocateChessboard = ParaChessBoard;

        PieceInfoController ThisPieceInfoController = this.GetComponent<PieceInfoController>();

        // 为棋子物体的state组件初始化所有基础属性
        ThisPieceInfoController.SetPieceID(PieceTamplate.GetPieceID());
        ThisPieceInfoController.SetPieceName(PieceTamplate.GetPieceName());
        ThisPieceInfoController.SetPieceCostMineral(PieceTamplate.GetPieceMineralCost());
        ThisPieceInfoController.SetPieceCostGas(PieceTamplate.GetPieceGasCost());
        SetRetreatThreshold(PieceTamplate.GetRetreatThreshold());
        SetCardMainImage(PieceTamplate.GetCardMainImage());
        SetUnitIconImage(PieceTamplate.GetUnitIconImage());
        SetCardDescriptionItemList(PieceTamplate.GetCardDescriptionItemList());
        PieceDeathEffect = PieceTamplate.GetPieceDeathEffect();

        // 为棋子物体的state组件初始化所有棋子应该包含的单位
        for (int i = 0; i < PieceTamplate.GetUnitTypeNum(); i++)
        {
            for (int j = 0; j < PieceTamplate.GetUnitNum(i); j++)
            {
                Unit NewUnit = new Unit();

                // 根据单位模板将单位初始化为预定单位
                NewUnit.InitialUnit(PieceTamplate.GetUnitTamplate(i));

                // 设置单位所在的棋子引用
                NewUnit.SetLocatePiece(this.transform.parent.gameObject);

                // 将新生成的单位添加入棋子单位字典中
                InitialUnit(NewUnit);
            }
        }

        // 加载棋子默认棋子底座
        MainPiecePedestalInfo = PieceTamplate.GetPiecePedestalInfo();

        if (MainPiecePedestalInfo != null)
        {
            MainPiecePedestal = MainPiecePedestalInfo.LoadPedestal(this.transform.parent.gameObject);
        }

        MainPieceStatusInfo = PieceTamplate.GetPieceStatusInfo();

        if(MainPieceStatusInfo != null)
        {
            MainPieceStatus = MainPieceStatusInfo.LoadStatus(this.transform.parent.gameObject, MainPiecePedestal);
        }

        // 初始化棋子UI
        this.GetComponent<PieceUIController>().InitializePieceUI();
    }

    // 通过一个棋子模板初始化一个位于棋盘外(尚未被部署到棋盘上的)棋子物体的state组件
    public void Initialize(Piece PieceTamplate)
    {
        PieceInfoController ThisPieceInfoController = this.GetComponent<PieceInfoController>();

        // 为棋子物体的state组件初始化所有基础属性
        ThisPieceInfoController.SetPieceID(PieceTamplate.GetPieceID());
        ThisPieceInfoController.SetPieceName(PieceTamplate.GetPieceName());
        ThisPieceInfoController.SetPieceCostMineral(PieceTamplate.GetPieceMineralCost());
        ThisPieceInfoController.SetPieceCostGas(PieceTamplate.GetPieceGasCost());
        SetRetreatThreshold(PieceTamplate.GetRetreatThreshold());
        SetCardMainImage(PieceTamplate.GetCardMainImage());
        SetUnitIconImage(PieceTamplate.GetUnitIconImage());
        SetCardDescriptionItemList(PieceTamplate.GetCardDescriptionItemList());
        PieceDeathEffect = PieceTamplate.GetPieceDeathEffect();

        this.transform.parent.name = PieceTamplate.GetPieceName();

        // 为棋子物体的state组件初始化所有棋子应该包含的单位
        for (int i = 0; i < PieceTamplate.GetUnitTypeNum(); i++)
        {
            for (int j = 0; j < PieceTamplate.GetUnitNum(i); j++)
            {
                Unit NewUnit = new Unit();

                // 根据单位模板将单位初始化为预定单位
                NewUnit.InitialUnit(PieceTamplate.GetUnitTamplate(i));

                // 设置单位所在的棋子引用
                NewUnit.SetLocatePiece(this.transform.parent.gameObject);

                // 将新生成的单位添加入棋子单位字典中
                InitialUnit(NewUnit);
            }
        }

        // 加载棋子默认棋子底座
        MainPiecePedestalInfo = PieceTamplate.GetPiecePedestalInfo();

        if (MainPiecePedestalInfo != null)
        {
            MainPiecePedestal = MainPiecePedestalInfo.LoadPedestal(this.transform.parent.gameObject);
        }

        MainPieceStatusInfo = PieceTamplate.GetPieceStatusInfo();

        if (MainPieceStatusInfo != null)
        {
            MainPieceStatus = MainPieceStatusInfo.LoadStatus(this.transform.parent.gameObject, MainPiecePedestal);
        }

        // 取消当前棋子的碰撞体并且将整体隐藏(棋子本体以及棋子UI)
        this.GetComponent<CapsuleCollider>().enabled = false;
        this.transform.parent.gameObject.SetActive(false);
        //this.GetComponent<PieceUIController>().HidePieceUIBase();

        // 初始化棋子UI
        //this.GetComponent<PieceUIController>().InitializePieceUI();

        // 隐藏棋子UI
        //this.GetComponent<PieceUIController>().HidePieceUIBase();
    }

    public void DeployPiece(GameObject ParaChessBoard, Vector2Int InterCoord)
    {
        // 为棋子物体的state组件初始化所有位置信息
        ChessBoardInternalCoordinate = InterCoord;
        ChessBoardInternalPosition = ParaChessBoard.GetComponent<ChessBoardController>().GetCheckerboardPosition(InterCoord);
        ChessBoardOuterCoordinate = ParaChessBoard.GetComponent<ChessBoardController>().ChessBoardOuterCoordinate;
        LocateChessboard = ParaChessBoard;
        
        this.GetComponent<CapsuleCollider>().enabled = true;

        this.GetComponent<PieceUIController>().InitializePieceUI();
    }

    public void DisplayPiece()
    {
        // 取消当前棋子的碰撞体并且将整体隐藏(棋子本体以及棋子UI)
        this.GetComponent<CapsuleCollider>().enabled = false;
        //this.transform.parent.gameObject.SetActive(false);
        this.GetComponent<PieceUIController>().HidePieceUIBase();
    }


    public void InitialUnit(Unit ParaUnit)
    {
        if(ContainUnitsOfUnitName.ContainsKey(ParaUnit.GetUnitID()) == true)
        {
            ParaUnit.SetUnitOwner(Owner);
            ContainUnitsOfUnitName[ParaUnit.GetUnitID()].Add(ParaUnit);
        }
        else
        {
            ParaUnit.SetUnitOwner(Owner);
            ContainUnitsOfUnitName.Add(ParaUnit.GetUnitID(), new List<Unit>());
            ContainUnitsOfUnitName[ParaUnit.GetUnitID()].Add(ParaUnit);
            UnitNameOfIndex.Add(UnitTypeNum, ParaUnit.GetUnitID());
            UnitTypeNum++;
        }

        //PieceContainUnit.Add(ParaUnit);
    }

    // 棋子亡语效果触发函数
    // 参数为击杀该棋子的单位
    public void PieceDead(Unit AttackUnit)
    {
        IsAlive = false;

        // 隐藏掉棋子对应的棋子UI
        this.GetComponent<PieceUIController>().HidePieceUIBase();

        // 隐藏棋子物体本身
        this.transform.parent.gameObject.SetActive(false);

        // 执行棋子亡语效果(源单位为当前阵亡棋子中的一个随机单位，目标单位为击杀当前棋子最后一个单位的敌方单位)
        PieceDeathEffect.ExcuteEffect(GetRandomPieceUnit(), AttackUnit);

        // 将棋子对应的卡牌移入该玩家的坟场卡牌区中
        if(RelativeCard != null)
        {
            RelativeCard.CardEnterGrave();
        }       

        // 通知棋盘 将该棋子从存活棋子引用数组中移动到阵亡棋子引用数组
        LocateChessboard.GetComponent<ChessBoardController>().PieceDie(this.transform.parent.gameObject);

        Debug.Log("棋子亡语启动~");

    }

    // 返回卡牌描述项数组
    public List<string> GetCardDescriptionItemList()
    {
        return CardDescriptionItemList;
    }

    // 设置卡牌描述项数组
    public void SetCardDescriptionItemList(List<string> NewCardDescriptionItemList)
    {
        CardDescriptionItemList = new List<string>();

        for(int i=0;i< NewCardDescriptionItemList.Count;i++)
        {
            CardDescriptionItemList.Add(NewCardDescriptionItemList[i]);
        }
    }

    // 战斗前更新方法
    public void UpdateBeforeBattle()
    {
        CurrentPieceHealthBeforeBattle = GetPieceCurrentHealth();
    }

    // 战斗后更新方法
    public void UpdateAfterBattle()
    {
        CurrentPieceHealthBeforeBattle = 0;
    }

    public float GetCurrentPieceHealthBeforeBattle()
    {
        return CurrentPieceHealthBeforeBattle;
    }

    public bool GetAliveState()
    {
        return IsAlive;
    }

    public int GetOwner()
    {
        return Owner;
    }

    public void SetCard(Card NewCard)
    {
        RelativeCard = NewCard;
    }

    public Card GetCard()
    {
        return RelativeCard;
    }

    public void SetCardMainImage(Texture NewCardMainImage)
    {
        CardMainImage = NewCardMainImage;
    }

    public Texture GetCardMainImage()
    {
        return CardMainImage;
    }

    public void SetUnitIconImage(Texture NewUnitIconImage)
    {
        UnitIconImage = NewUnitIconImage;
    }

    public Texture GetUnitIconImage()
    {
        return UnitIconImage;
    }

    // 获取当前棋子内的所有单位
    public List<Unit> GetAllAlivePieceUnit()
    {
        List<Unit> Res = new List<Unit>();

        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                if(CurrentUnitList[j].GetAliveState() == true)
                {
                    Res.Add(CurrentUnitList[j]);
                }               
            }
        }

        return Res;
    }

    // 设置棋子所有者
    public void SetOwner(int NewOwner)
    {
        // 1.通知棋盘更改所有者
        if(LocateChessboard != null)
        {
            LocateChessboard.GetComponent<ChessBoardController>().PieceChangeOwner(this.transform.parent.gameObject, NewOwner);
        }       

        // 2.修改棋子所有者
        Owner = NewOwner;

        // 3.修改棋子包含所有单位的所有者
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                CurrentUnitList[j].SetUnitOwner(NewOwner);
            }
        }
    }

    // 获取棋子中包含单位的类型数目
    public int GetUnitTypeNum()
    {
        return UnitTypeNum;
    }

    // 获取棋子中包含单位的总数
    public int GetContainUnitNum()
    {
        int ContainUnitNum = 0;

        for (int i = 0; i < UnitTypeNum; i++)
        {
            ContainUnitNum += ContainUnitsOfUnitName[UnitNameOfIndex[i]].Count;            
        }

        return ContainUnitNum;
    }

    // 通过单位名称获取棋子中包含的所有该种单位
    public List<Unit> GetContainUnitListByName(string UnitName)
    {
        if(ContainUnitsOfUnitName.ContainsKey(UnitName) == true)
        {
            return ContainUnitsOfUnitName[UnitName];
        }
        else
        {
            return null;
        }
    }

    // 获取棋子包含单位的平均尺寸
    public float GetUnitSize()
    {
        return GetPieceSize() / GetContainUnitNum();
    }

    // 获取棋子包含所有单位的尺寸和
    public float GetPieceSize()
    {
        float PieceSize = 0.0f;

        // 遍历每一种类型的单位
        for(int i=0;i< UnitTypeNum;i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for(int j=0;j<CurrentUnitList.Count;j++)
            {
                PieceSize += CurrentUnitList[j].GetUnitSize();
            }
        }

        Debug.Log("Piece Size is " + PieceSize);
        //for (int i = 0; i < PieceContainUnit.Count; i++)
        //{
        //    Debug.Log(PieceContainUnit[i].GetUnitSize());
        //    PieceSize += PieceContainUnit[i].GetUnitSize();
        //}
        return PieceSize;
    }

    // 获取棋子包含所有单位的最大生命值和
    public int GetPieceMaxHealth()
    {
        int PieceMaxHealth = 0;

        // 遍历每一种类型的单位
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                PieceMaxHealth += CurrentUnitList[j].GetMaxHealth();
            }
        }

        //for (int i = 0; i < PieceContainUnit.Count; i++)
        //{
        //    PieceMaxHealth += PieceContainUnit[i].GetMaxHealth();
        //}
        return PieceMaxHealth;
    }

    // 获取棋子包含所有单位的当前生命值和
    public int GetPieceCurrentHealth()
    {
        int PieceCurrentHealth = 0;
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                PieceCurrentHealth += CurrentUnitList[j].GetCurrentHealth();
            }
        }
        return PieceCurrentHealth;
    }

    public float GetRationOfHealth()
    {
        return (float)GetPieceCurrentHealth() / (float)GetPieceMaxHealth();
    }

    // 获取棋子机动能力系数(全部单位平均)
    public float GetPieceMobilityFactor()
    {
        float MobilityFactorSum = 0.0f;

        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                MobilityFactorSum += CurrentUnitList[j].GetMobilityFactor();
            }
        }

        return MobilityFactorSum / GetContainUnitNum();
    }

    // 获取某种类型单位的平均机动能力系数
    public float GetUnitsMobilityFactorByUnitName(string UnitName)
    {
        float MobilityFactorSum = 0.0f;

        List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitName];
        // 遍历每种类型中的每一个单位
        for (int j = 0; j < CurrentUnitList.Count; j++)
        {
            MobilityFactorSum += CurrentUnitList[j].GetMobilityFactor();
        }

        return MobilityFactorSum / CurrentUnitList.Count;
    }

    public int GetAliveUnitNum()
    {
        int AliveUnitNum = 0;

        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                if (CurrentUnitList[j].GetAliveState() == true)
                {
                    AliveUnitNum++;
                }
            }
        }
        return AliveUnitNum;
    }

    // 获取当前棋子包含的所有UnitName类型单位中存活单位的数目
    public int GetAliveCertainTypeUnitNum(string UnitName)
    {
        int AliveUnitNum = 0;

        List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitName];
        // 遍历该种类型中的每一个单位
        for (int j = 0; j < CurrentUnitList.Count; j++)
        {
            if (CurrentUnitList[j].GetAliveState() == true)
            {
                AliveUnitNum++;
            }
        }

        return AliveUnitNum;
    }

    // 获取当前棋子包含的所有UnitName类型单位的平均战斗射程
    public float GetCertainTypeUnitRange(string UnitName)
    {
        float UnitRangeSum = 0.0f;
        int AliveUnitNum = 0;

        List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitName];
        // 遍历该种类型中的每一个单位
        for (int j = 0; j < CurrentUnitList.Count; j++)
        {
            if (CurrentUnitList[j].GetAliveState() == true)
            {
                AliveUnitNum++;
                UnitRangeSum += CurrentUnitList[j].CalculateUnitRange();
            }
        }

        if(AliveUnitNum == 0)
        {
            return 0;
        }
        else
        {
            return UnitRangeSum / AliveUnitNum;
        }       
    }

    // 获取当前棋子包含的所有UnitName类型单位的平均机动能力系数
    public float GetCertainTypeUnitMobilityFactor(string UnitName)
    {
        float UnitMobilityFactorSum = 0.0f;
        int AliveUnitNum = 0;

        List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitName];
        // 遍历该种类型中的每一个单位
        for (int j = 0; j < CurrentUnitList.Count; j++)
        {
            if (CurrentUnitList[j].GetAliveState() == true)
            {
                AliveUnitNum++;
                UnitMobilityFactorSum += CurrentUnitList[j].GetMobilityFactor();
            }
        }

        if (AliveUnitNum == 0)
        {
            return 0;
        }
        else
        {
            return UnitMobilityFactorSum / AliveUnitNum;
        }
    }

    // 获取当前棋子包含的所有UnitName类型单位的平均尺寸
    public float GetCertainTypeUnitSize(string UnitName)
    {
        float UnitMobilitySize = 0.0f;
        int AliveUnitNum = 0;

        List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitName];
        // 遍历该种类型中的每一个单位
        for (int j = 0; j < CurrentUnitList.Count; j++)
        {
            if (CurrentUnitList[j].GetAliveState() == true)
            {
                AliveUnitNum++;
                UnitMobilitySize += CurrentUnitList[j].GetUnitSize();
            }
        }

        if (AliveUnitNum == 0)
        {
            return 0;
        }
        else
        {
            return UnitMobilitySize / AliveUnitNum;
        }
    }

    // 从当前棋子包含的所有UnitName类型单位中随机挑选BattleUnitNum个单位
    public List<Unit> GetCertainTypeBattleUnitByTypeName(string UnitName, int BattleUnitNum)
    {
        List<Unit> Res = new List<Unit>();

        UnitSelector NewUnitSelector = new UnitSelector();
        NewUnitSelector.SetSelectMode(0);
        NewUnitSelector.SetSelectKey("CurrentHealth");

        //if(UnitName == "Baneling")
        //{
        //    ContainUnitsOfUnitName[UnitName][0].SetCurrentHealth(30);
        //    ContainUnitsOfUnitName[UnitName][1].SetCurrentHealth(25);
        //    ContainUnitsOfUnitName[UnitName][2].SetCurrentHealth(20);
        //    ContainUnitsOfUnitName[UnitName][3].SetCurrentHealth(15);
        //}

        Res = NewUnitSelector.ExecuteSelection(ContainUnitsOfUnitName[UnitName], BattleUnitNum);
        //int DeleteUnitNum = 0;
        //int AliveUnitNum = GetAliveCertainTypeUnitNum(UnitName);
        //if (BattleUnitNum >= AliveUnitNum)
        //{
        //    DeleteUnitNum = 0;
        //}
        //else
        //{
        //    DeleteUnitNum = AliveUnitNum - BattleUnitNum;
        //}
        //
        //List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitName];
        //
        //for (int i = 0; i < CurrentUnitList.Count; i++)
        //{
        //    if (CurrentUnitList[i].GetAliveState() == true)
        //    {
        //        Res.Add(CurrentUnitList[i]);
        //    }
        //}
        //
        //for (int i = 0; i < DeleteUnitNum; i++)
        //{
        //    int DeleteIndex = (int)Random.Range(0, Res.Count);
        //    while (DeleteIndex == Res.Count)
        //    {
        //        DeleteIndex = (int)Random.Range(0, Res.Count);
        //    }
        //    Res[DeleteIndex] = null;
        //    Res.Remove(Res[DeleteIndex]);
        //}

        for(int i=0;i< Res.Count;i++)
        {
            //Debug.Log("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
            //Debug.Log(Res[i].GetCurrentHealth());
            //Debug.Log("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
        }

        return Res;

    }

    // 返回一个至多含有BattleUnitNum个单位的参战单位数组
    public List<Unit> GetRandomBattleUnitArray(int BattleUnitNum)
    {
        List<Unit> Res = new List<Unit>();
        int DeleteUnitNum = 0;
        int AliveUnitNum = GetAliveUnitNum();
        if (BattleUnitNum >= AliveUnitNum)
        {
            DeleteUnitNum = 0;
        }
        else
        {
            DeleteUnitNum = AliveUnitNum - BattleUnitNum;
        }

        for (int i = 0; i < ContainUnitsOfUnitName[UnitNameOfIndex[0]].Count; i++)
        {
            if (ContainUnitsOfUnitName[UnitNameOfIndex[0]][i].GetAliveState() == true)
            {
                Res.Add(ContainUnitsOfUnitName[UnitNameOfIndex[0]][i]);
            }
        }

        for(int i=0;i< DeleteUnitNum; i++)
        {
            int DeleteIndex = (int)Random.Range(0, Res.Count);
            while(DeleteIndex == Res.Count)
            {
                DeleteIndex = (int)Random.Range(0, Res.Count);
            }
            Res[DeleteIndex] = null;
            Res.Remove(Res[DeleteIndex]);
        }

        return Res;
    }

    public int CalculateBattleUnitNum(float BUnitSize, float YUnitSize, float Range, float MobilityFactor)
    {
        // 计算y = kx + b 距离攻击目标Range处 对于 具有机动因子MobilityFactor的单位 所能使用的空间总数
        float YSizeSum = MobilityFactor * Range + BUnitSize;

        // 返回y大小的总size空间中能够容纳的单位总数
        return (int)(YSizeSum / YUnitSize);
    }

    // 根据目标单位尺寸和计算该棋子的所有作战单位并加入玩家作战信息集合
    public void FetchBattleUnitFromPiece(float TartSize, BattleInfoSet ParaBattleInfoSet)
    {
        // 将棋子本身的引用加入到玩家作战信息集合当中
        ParaBattleInfoSet.GetPlayerBattleInfoSetByPlayerID(Owner).AddNewBattlePiece(this.transform.parent.gameObject);
        // 遍历该棋子中的每一种单位
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];

            // 1获取当前种类单位的平均尺寸
            float CurrentTypeUnitSize = GetCertainTypeUnitSize(UnitNameOfIndex[i]);

            // 2获取当前种类单位的平均战斗射程
            float CurrentTypeUnitRange = GetCertainTypeUnitRange(UnitNameOfIndex[i]);

            // 3获取当前种类单位的平均机动能力系数
            float CurrentTypeUnitMobilityFactor = GetCertainTypeUnitMobilityFactor(UnitNameOfIndex[i]);

            // 4计算当前种类单位的参战单位数目
            int CurrentTypeBattleUnitNum = CalculateBattleUnitNum(TartSize, CurrentTypeUnitSize, CurrentTypeUnitRange, CurrentTypeUnitMobilityFactor);

            List<Unit> CurrentTypeBattleUnit = GetCertainTypeBattleUnitByTypeName(UnitNameOfIndex[i], CurrentTypeBattleUnitNum);

            // 5选择参战单位并将所有参战单位加入玩家作战信息集合
            ParaBattleInfoSet.GetPlayerBattleInfoSetByPlayerID( Owner ).AddNewBattleUnits( CurrentTypeBattleUnit );
        }
    }

    public void DisplayAllUnit()
    {
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                //Debug.Log(CurrentUnitList[j].GetUnitID());
            }
        }

    }

    public void SetCoordinate(Vector2Int InterCoord, Vector3 InterPos, Vector2Int OuterCoord)
    {
        ChessBoardInternalCoordinate = InterCoord;
        ChessBoardOuterCoordinate = OuterCoord;

        ChessBoardInternalPosition = InterPos;


    }

    // 设置棋子的层级
    public void SetPieceLayer(int NewLayer)
    {
        Layer = NewLayer;
    }

    // 获取棋子的层级
    public int GetPieceLayer()
    {
        return Layer;
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

    public void SerInternalCoordinate(Vector2 Coord)
    {

    }

    // 棋子战斗开始更新函数(所有参战棋子会在战斗开始前各自调用该函数更新棋子状态)
    public void PieceStateUpdateBeforeBattle()
    {
        // 更新棋子的战斗状态标旗
        InBattle = true;
        // 更新棋子所有包含单位的战斗状态标旗
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                CurrentUnitList[i].UnitStateUpdateBeforeBattle();
            }
        }

        // 此更新函数中不检查棋子的存活状态
        // 遍历行为list检查是否存在战斗结束后就应该移除的行为并且移除这些行为
    }

    // 棋子战斗开始存活状态更新函数(所有参战棋子会在战斗开始前各自调用该函数检查自己的存活状态)
    public void PieceAliveStateUpdateBeforeBattle()
    {
        // 检查棋子是否已经不再存活，如果hp == 0 则将棋子的存活状态标旗修改为false
    }

    // 棋子战斗结束更新函数(所有参战棋子会在战斗结束后各自调用该函数更新棋子状态)
    public void PieceStateUpdateAfterBattle()
    {
        // 更新棋子的战斗状态标旗
        InBattle = false;
        // 更新棋子所有包含单位的战斗状态标旗
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                CurrentUnitList[i].UnitStateUpdateBeforeBattle();
            }
        }

        // 此更新函数中不检查棋子的存活状态
        // 遍历行为list检查是否存在战斗结束后就应该移除的行为并且移除这些行为
    }

    // 棋子战斗结束存活状态更新函数(所有参战棋子会在战斗结束后各自调用该函数检查自己的存活状态)
    public void PieceAliveStateUpdateAfterBattle()
    {
        // 检查棋子是否已经不再存活，如果hp == 0 则调用death函数移除棋子
    }

    // 棋子回合开始更新函数(所有棋子会在每个玩家的回合开始前各自调用该函数更新棋子状态)
    public void PieceStateUpdateBeforeTurn()
    {
        // 遍历行为list检查是否存在玩家回合开始前就应该移除的行为并且移除这些行为
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                CurrentUnitList[i].UnitStateUpdateBeforeTurn();
            }
        }
    }

    // 棋子回合结束更新函数(所有棋子会在每个玩家的回合结束后各自调用该函数更新棋子状态)
    public void PieceStateUpdateAfterTurn()
    {
        // 遍历行为list检查是否存在玩家回合结束后就应该移除的行为并且移除这些行为
        for (int i = 0; i < UnitTypeNum; i++)
        {
            List<Unit> CurrentUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[i]];
            // 遍历每种类型中的每一个单位
            for (int j = 0; j < CurrentUnitList.Count; j++)
            {
                CurrentUnitList[i].UnitStateUpdateAfterTurn();
            }
        }
    }

    public Unit GetRandomPieceUnit()
    {
        int RandomIndex = Random.Range(0, UnitTypeNum - 1);

        List<Unit> SelectTypeUnitList = ContainUnitsOfUnitName[UnitNameOfIndex[RandomIndex]];

        RandomIndex = Random.Range(0, SelectTypeUnitList.Count - 1);

        return SelectTypeUnitList[RandomIndex];
    }

    // Use this for initialization
    void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
		
	}
}
