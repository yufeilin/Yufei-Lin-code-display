using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PieceOperationController : MonoBehaviour
{
    public GameObject ClientManager = null;
    // 当前棋子原初状态所使用的材质
    public Material PieceOriginalMaterial = null;
    // 当前棋子被选中时所使用的材质
    public Material PieceSelectedMaterial = null;

    //GameObject ChessBoardManager = null;

    //GameObject PieceStateController = null;

    PieceStateController ThisPieceStateController = null;
    PieceInfoController ThisPieceInfoController = null;

    GameObject OperatbleTargetPrefab = null;

    GameObject HintTargetPrefab = null;

    // 可操作目标 引用数组
    GameObject[] OperatblePreviews = null;

    // 提示目标 引用数组
    GameObject[] HintPreviews = null;

    // 可攻击棋子 引用数组
    GameObject[] AttackTargets = null;

    int HintPreviewIndex = 0;

    int PreviewIndex = 0;

    int AttackTargetIndex = 0;

    // 棋子所在棋盘的单个棋盘格边长
    float CheckerBoardSize = 1.0f;

    // 棋子所在棋盘的所具有的行数或者列数
    int ChessBoardSize = 8;

    // 棋子所在棋盘的中心点位置
    //float GeneratePositionX = 0.0f;
    //float GeneratePositionZ = 0.0f;

    // 棋子所在棋盘的坐标(0,0)棋盘格的中心点位置
    //Vector3 InitPosition = new Vector3();


    int[] MoveDirectionX = new int[8];
    int[] MoveDirectionY = new int[8];

    GameObject ChessBoard = null;

    // Use this for initialization
    void Start ()
    {
        ClientManager = GameObject.Find("ClientManager");

        ThisPieceStateController = this.GetComponent<PieceStateController>();
        ThisPieceInfoController = this.GetComponent<PieceInfoController>();


        //ChessBoardManager = GameObject.Find("ChessBoardManager");

        //CheckerBoardSize = LocateChessboard.GetComponent<ChessBoardInfoController>().CheckerBoardSize;
        //ChessBoardSize = LocateChessboard.GetComponent<ChessBoardInfoController>().ChessBoardSize;
        //GeneratePositionX = LocateChessboard.GetComponent<ChessBoardInfoController>().ChessBoardOuterPosition.x;
        //GeneratePositionZ = LocateChessboard.GetComponent<ChessBoardInfoController>().ChessBoardOuterPosition.y;
        //InitPosition = LocateChessboard.GetComponent<ChessBoardInfoController>().InitPosition;

        // 载入可操作目标预制体
        OperatbleTargetPrefab = (GameObject)Resources.Load("Prefabs/OperatbleTarget");

        HintTargetPrefab = (GameObject)Resources.Load("Prefabs/HintTarget");

        //ThisPieceStateController = this.transform.parent.GetComponent<PieceStateController>();

        if (OperatbleTargetPrefab == null)
        {
            Debug.Log("OperatbleTargetPrefab load fail");
        }

        // TODO: 动态调整可操作目标数组的长度
        OperatblePreviews = new GameObject[10];

        HintPreviews = new GameObject[20];

        AttackTargets = new GameObject[20];

        MoveDirectionX[0] = 1; MoveDirectionY[0] = 1;
        MoveDirectionX[1] = 1; MoveDirectionY[1] = 0;
        MoveDirectionX[2] = 1; MoveDirectionY[2] = -1;
        MoveDirectionX[3] = 0; MoveDirectionY[3] = -1;
        MoveDirectionX[4] = -1; MoveDirectionY[4] = -1;
        MoveDirectionX[5] = -1; MoveDirectionY[5] = 0;
        MoveDirectionX[6] = -1; MoveDirectionY[6] = 1;
        MoveDirectionX[7] = 0; MoveDirectionY[7] = 1;
    }
	
	// Update is called once per frame
	void Update ()
    {

    }

    // 隐藏当前棋子的棋子底座模型，棋子模型以及棋子基本UI
    public void HidePiece()
    {
        foreach (Transform Child in this.transform.parent.transform)
        {
            if(Child.gameObject.name == "PieceMainframe")
            {
                // 将该棋子的碰撞体禁用以便该棋子不会与输入控制器互动
                Child.gameObject.GetComponent<CapsuleCollider>().enabled = false;
                continue;
            }
            else
            {
                Child.gameObject.SetActive(false);
            }
        }

        this.GetComponent<PieceUIController>().HidePieceUIBase();
    }

    // 显示当前棋子的棋子底座模型，棋子模型以及棋子基本UI
    public void DisplayPiece()
    {
        foreach (Transform Child in this.transform.parent.transform)
        {
            if (Child.gameObject.name == "PieceMainframe")
            {
                // 将该棋子的碰撞体启用以便该棋子能够与输入控制器互动
                Child.gameObject.GetComponent<CapsuleCollider>().enabled = true;
                continue;
            }
            else
            {
                Child.gameObject.SetActive(true);
            }
        }

        this.GetComponent<PieceUIController>().DisplayPieceUIBase();
    }

    public void jaja(out int aa)
    {
        aa = 1;
    }

    public bool CheckerBoardAvaliable(Vector2Int InterCoord, out Vector2Int OutInterCoord, out Vector2Int OutOuterCoord)
    {
        GameObject LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;

        if(LocateChessBoard.GetComponent<ChessBoardController>().CheckerBoardAvaliable(InterCoord, out OutInterCoord, out OutOuterCoord) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // 判断棋盘坐标(InternCoodX, InternCoodZ)的状态
    // 返回值 = -1 此处为棋盘边界之外的坐标
    // 返回值 = 1 此处为可安全移动
    // 返回值 = 2 此处有可攻击目标
    //public int CoordinateState(int InternCoodX, int InternCoodZ)
    //{
    //    ChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;
    //
    //    if(InternCoodX >= 0 && InternCoodX < ChessBoardSize && InternCoodZ >= 0 && InternCoodZ < ChessBoardSize)
    //    {
    //
    //    }
    //
    //    if (InternCoodX < 0 || InternCoodX >= ChessBoardSize || InternCoodZ < 0 || InternCoodZ >= ChessBoardSize)
    //    {
    //        // 棋盘越界判断
    //        return -1;
    //    }
    //    else if(ChessBoard.GetComponent<ChessBoardStateController>().GetCheckerPieceNum(new Vector2(InternCoodX, InternCoodZ)) > 0)
    //    {
    //        // 目标棋盘格棋子容量判断
    //        return -1;
    //    }
    //    else
    //    {
    //        return 1;
    //    }
    //}

    // 获取坐标为OuterCoord的棋盘上坐标为InterCoord的棋盘格上层数为0的棋子
    public GameObject GetCheckerPiece(Vector2Int InterCoord, Vector2Int OuterCoord, int layer = 0)
    {
        GameObject LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;
        WorldManager MainWorldManager = LocateChessBoard.GetComponent<ChessBoardController>().GetWorldManager();

        GameObject NewLocateChessBoard = MainWorldManager.GetChessBoard(OuterCoord);

        int PlayerID = this.GetComponent<PieceStateController>().GetOwner();

        PlayerID = 1;

        return NewLocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePiece(InterCoord, PlayerID);
    }

    // 在坐标为OuterCoord的棋盘的坐标为InterCoord的棋盘格生成一个 可移动类型的可操作目标
    public void GenerateOperatableTarget(Vector2Int InterCoord, Vector2Int OuterCoord)
    {
        GameObject LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;
        WorldManager MainWorldManager = LocateChessBoard.GetComponent<ChessBoardController>().GetWorldManager();

        GameObject NewLocateChessBoard = MainWorldManager.GetChessBoard(OuterCoord);

        Vector3 CurrentPosition = NewLocateChessBoard.GetComponent<ChessBoardController>().GetCheckerboardPosition(InterCoord);

        CurrentPosition.y += 0.5f;

        GameObject CurrentOperatableTarget = Instantiate(OperatbleTargetPrefab, CurrentPosition, Quaternion.identity);

        // 设置生成的可操作目标的尺寸
        CurrentOperatableTarget.transform.localScale = new Vector3((float)CheckerBoardSize * 0.75f, 0.5f, (float)CheckerBoardSize * 0.75f);
        // 设置生成的可操作目标的状态信息
        CurrentOperatableTarget.GetComponent<OperatableTargetStateController>().SetCoordinate(InterCoord, CurrentPosition, OuterCoord);

        CurrentOperatableTarget.GetComponent<OperatableTargetStateController>().LocateChessBoard = NewLocateChessBoard;

        OperatblePreviews[PreviewIndex++] = CurrentOperatableTarget;
    }

    public void GenerateHintTarget(Vector2Int InterCoord, Vector2Int OuterCoord, int type = 0)
    {
        GameObject LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;
        WorldManager MainWorldManager = LocateChessBoard.GetComponent<ChessBoardController>().GetWorldManager();

        GameObject NewLocateChessBoard = MainWorldManager.GetChessBoard(OuterCoord);

        Vector3 CurrentPosition = NewLocateChessBoard.GetComponent<ChessBoardController>().GetCheckerboardPosition(InterCoord);

        CurrentPosition.y += 0.25f;

        GameObject CurrentHintTarget = Instantiate(HintTargetPrefab, CurrentPosition, Quaternion.identity);

        // 设置生成的可操作目标的尺寸
        CurrentHintTarget.transform.localScale = new Vector3((float)CheckerBoardSize * 0.87f, 0.5f, (float)CheckerBoardSize * 0.87f);
        // 设置生成的可操作目标的状态信息
        CurrentHintTarget.GetComponent<HintTargetStateController>().SetCoordinate(InterCoord, CurrentPosition, OuterCoord);

        CurrentHintTarget.GetComponent<HintTargetStateController>().LocateChessBoard = NewLocateChessBoard;

        CurrentHintTarget.GetComponent<HintTargetStateController>().Initialize(type);

        HintPreviews[HintPreviewIndex++] = CurrentHintTarget;
    }

    // 鼠标左键点选棋子后在主场景中显示所有的 可移动位置 和 可攻击位置
    public void OperationPreview()
    {
        PreviewIndex = 0;

        ThisPieceStateController = this.GetComponent<PieceStateController>();
        ThisPieceInfoController = this.GetComponent<PieceInfoController>();

        ThisPieceStateController.DisplayAllUnit();

        GameObject LocateChessboard = ThisPieceStateController.LocateChessboard;

        CheckerBoardSize = LocateChessboard.GetComponent<ChessBoardController>().CheckerBoardSize;
        ChessBoardSize = LocateChessboard.GetComponent<ChessBoardController>().ChessBoardSize;
        //GeneratePositionX = LocateChessboard.GetComponent<ChessBoardInfoController>().ChessBoardOuterPosition.x;
        //GeneratePositionZ = LocateChessboard.GetComponent<ChessBoardInfoController>().ChessBoardOuterPosition.y;
        //InitPosition = LocateChessboard.GetComponent<ChessBoardInfoController>().InitPosition;

        //Debug.Log("233");
        this.GetComponent<MeshRenderer>().material = PieceSelectedMaterial;

        if(this.GetComponent<PieceInfoController>().HasOrientation == false)
        {
            // 获取该棋子在棋盘中的坐标
            Vector2Int CurrentCoordinate = ThisPieceStateController.ChessBoardInternalCoordinate;

            // 进行常规移动位置计算
            for(int i=0;i<8;i++)
            {
                for(int j=1;j<= ThisPieceInfoController.BaseMoveAbility[i]; j++)
                {
                    Vector2Int NewInterCoord = new Vector2Int();
                    Vector2Int NewOuterCoord = new Vector2Int();

                    Vector2Int ParaInterCoord = new Vector2Int(CurrentCoordinate.x + j * MoveDirectionX[i], CurrentCoordinate.y + j * MoveDirectionY[i]);

                    if(CheckerBoardAvaliable(ParaInterCoord, out NewInterCoord, out NewOuterCoord) == true)
                    {
                        GenerateHintTarget(NewInterCoord, NewOuterCoord,1);
                        GenerateOperatableTarget(NewInterCoord, NewOuterCoord);
                    }
                }
            }

            // 进行特殊移动位置计算

            // 进行常规攻击位置计算
            for (int i = 0; i < 8; i++)
            {
                for (int j = 1; j <= ThisPieceInfoController.BaseAttackAbility[i]; j++)
                {
                    Vector2Int NewInterCoord = new Vector2Int();
                    Vector2Int NewOuterCoord = new Vector2Int();

                    Vector2Int ParaInterCoord = new Vector2Int((int)CurrentCoordinate.x + j * MoveDirectionX[i], (int)CurrentCoordinate.y + j * MoveDirectionY[i]);

                    if (CheckerBoardAvaliable(ParaInterCoord, out NewInterCoord, out NewOuterCoord) == true)
                    {
                        GenerateHintTarget(NewInterCoord, NewOuterCoord, 2);
                        //GenerateOperatableTarget(NewInterCoord, NewOuterCoord);
                        GameObject AttackTargetPiece = GetCheckerPiece(NewInterCoord, NewOuterCoord);
                        if(AttackTargetPiece != null)
                        {
                            //Debug.Log(AttackTargetPiece.name);
                            AttackTargets[AttackTargetIndex++] = AttackTargetPiece;

                            AttackTargetPiece.GetComponentInChildren<PieceStateController>().IsAttackTarget = true;
                        }
                    }
                }
            }
        }
        else
        {

        }
    }

    // 在主场景中隐藏所有的 可移动位置 和 可攻击位置
    public void OperationPreviewOff()
    {
        // 恢复棋子原初材质
        this.GetComponent<MeshRenderer>().material = PieceOriginalMaterial;
        // 将当前棋子所有的可操作目标全部摧毁
        for (int i=0;i<PreviewIndex;i++)
        {
            Destroy(OperatblePreviews[i]);
            OperatblePreviews[i] = null;
        }

        // 索引值重置为0
        PreviewIndex = 0;
    }

    public void HintPreviewOff()
    {
        for (int i = 0; i < HintPreviewIndex; i++)
        {
            Destroy(HintPreviews[i]);
            HintPreviews[i] = null;
        }

        // 索引值重置为0
        HintPreviewIndex = 0;
    }

    public void AttackTargetOff()
    {
        //Debug.Log("AttackTargetIndex:" + AttackTargetIndex);

        for(int i=0;i<AttackTargetIndex;i++)
        {
            if(AttackTargets[i] != null)
            {
                AttackTargets[i].GetComponentInChildren<PieceStateController>().IsAttackTarget = false;
                AttackTargets[i] = null;
            }           
        }

        AttackTargetIndex = 0;
    }

    public void MoveToTareget(GameObject OpertableTarget)
    {
        //ClientManager.GetComponent<ClientController>().PieceMoveRequest();

        GameObject LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;

        // 获取可操作目标的内棋盘位置
        Vector3 NewPosition = OpertableTarget.GetComponent<OperatableTargetStateController>().ChessBoardInternalPosition;
        // 修正该位置以获得棋子的新位置
        NewPosition.y -= 0.5f;

        Vector2Int NewInterCoord = OpertableTarget.GetComponent<OperatableTargetStateController>().ChessBoardInternalCoordinate;
        Vector2Int TmpOuterCoord = OpertableTarget.GetComponent<OperatableTargetStateController>().ChessBoardOuterCoordinate;
        Vector2Int NewOuterCoord = new Vector2Int((int)TmpOuterCoord.x, (int)TmpOuterCoord.y);

        // 首先调用棋盘控状态控制器更新棋盘状态控制器中存储的棋子坐标信息
        LocateChessBoard.GetComponent<ChessBoardController>().PieceMove(this.transform.parent.gameObject, NewInterCoord, NewOuterCoord, 0);

        // 然后再将棋子的位置进行调整
        //this.transform.parent.position = NewPosition;      

        // 更新当前棋子状态控制器中的位置信息
        ThisPieceStateController.SetCoordinate(OpertableTarget.GetComponent<OperatableTargetStateController>().ChessBoardInternalCoordinate, OpertableTarget.GetComponent<OperatableTargetStateController>().ChessBoardInternalPosition, OpertableTarget.GetComponent<OperatableTargetStateController>(). ChessBoardOuterCoordinate);

        // 更新当前棋子状态控制器中的所在棋盘信息
        this.GetComponent<PieceStateController>().LocateChessboard = OpertableTarget.GetComponent<OperatableTargetStateController>().LocateChessBoard;

        // 启动当前棋子的移动动画
        this.GetComponent<PieceAnimationController>().MoveAnimationStart(NewPosition);

        OperationPreviewOff();
        HintPreviewOff();
        AttackTargetOff();
    }

    // 获取当前棋子（进攻棋子）所在棋盘格其他的具有协同进攻标旗的棋子
    public List<GameObject> GetAttackPieces()
    {
        List<GameObject> Res = new List<GameObject>();

        ChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;

        Res = ChessBoard.GetComponent<ChessBoardController>().GetAttackPieces(this.gameObject.transform.parent.gameObject);

        return Res;
    }

    // 获取防御棋子DefendPiece所在棋盘格其他的具有协同防御标旗的棋子
    public List<GameObject> GetDefendPieces(GameObject DefendPiece)
    {
        List<GameObject> Res = new List<GameObject>();

        ChessBoard = DefendPiece.GetComponentInChildren<PieceStateController>().LocateChessboard;

        Res = ChessBoard.GetComponent<ChessBoardController>().GetAttackPieces(DefendPiece);

        return Res;
    }

    // 获取参战方所有棋子的尺寸和
    public float GetAllBattleUnitSize(List<GameObject> AllBattlePieces)
    {
        float Res = 0.0f;

        for(int i=0;i< AllBattlePieces.Count;i++)
        {
            Res += AllBattlePieces[i].GetComponentInChildren<PieceStateController>().GetPieceSize();
        }

        return Res;
    }

    // 将MainPieces所有的参战单位加入战斗信息集合
    public void GetBattleUnits(List<GameObject> MainPieces, List<GameObject> TargetPieces, BattleInfoSet ParaBattleInfoSet)
    {
        float TargetSize = GetAllBattleUnitSize(TargetPieces);

        for(int i=0;i< MainPieces.Count;i++)
        {

            MainPieces[i].GetComponentInChildren<PieceStateController>().FetchBattleUnitFromPiece(TargetSize, ParaBattleInfoSet);

        }
    }

    //=========================================================================================================

    public int CalculateBattleUnitNum(float BUnitSize, float YUnitSize, float Range, float MobilityFactor)
    {
        // 计算y = kx + b 距离攻击目标Range处 对于 具有机动因子MobilityFactor的单位 所能使用的空间总数
        float YSizeSum = MobilityFactor * Range + BUnitSize;

        // 返回y大小的总size空间中能够容纳的单位总数
        return (int)(YSizeSum / YUnitSize);
    }

    public float CalculateAttackablePercentage(float RangeDifference)
    {
        return -0.04f * RangeDifference + 1.0f;
    }

    public void SetUnitsAttackableOff(List<Unit> ParaUnitList)
    {
        for(int i=0;i< ParaUnitList.Count;i++)
        {
            ParaUnitList[i].SetAttackableState(false);
        }
    }

    public List<Unit> SetUnitsAttackableOn(List<Unit> ParaUnitList, int ParaUnitNum)
    {
        List<Unit> Res = new List<Unit>();

        for(int i=0;i< ParaUnitNum;i++)
        {
            int SetIndex = (int)Random.Range(0, ParaUnitList.Count);
            while (SetIndex == Res.Count)
            {
                SetIndex = (int)Random.Range(0, ParaUnitList.Count);
            }

            ParaUnitList[SetIndex].SetAttackableState(true);
            Res.Add(ParaUnitList[SetIndex]);

            ParaUnitList[SetIndex] = null;
            ParaUnitList.Remove(ParaUnitList[SetIndex]);
        }

        for(int i=0;i< ParaUnitList.Count;i++)
        {
            Res.Add(ParaUnitList[i]);

            ParaUnitList[i] = null;
            ParaUnitList.Remove(ParaUnitList[i]);
        }

        return Res;
    }

    public void DebugDisplay(List<Unit> TestList)
    {
        Debug.Log("-----------------------------");
        Debug.Log(TestList.Count);
        for(int i=0;i< TestList.Count;i++)
        {
            Debug.Log(TestList[i].GetAttakableState());
        }
        Debug.Log("-----------------------------");
    }

    public void DisplayUnitHealth(List<Unit> TestList)
    {
        Debug.Log("=============================");
        Debug.Log(TestList.Count);
        for (int i = 0; i < TestList.Count; i++)
        {
            Debug.Log(TestList[i].GetCurrentHealth() + "/" + TestList[i].GetMaxHealth());
        }
        Debug.Log("=============================");
    }

    public int GetAliveUnitNum(List<Unit> ParaUnitList)
    {
        int AliveUnitNum = 0;
        for(int i=0;i< ParaUnitList.Count;i++)
        {
            if(ParaUnitList[i].GetAliveState() == true)
            {
                AliveUnitNum++;
            }           
        }
        return AliveUnitNum;
    }

    // 返回伤害造成方参战单位中一个攻击次数最小的单位
    public Unit SelectDamageDealer(List<Unit> DamageDealerArray)
    {
        Unit MinAttackCountUnit = null;

        // 将伤害造成方参战单位中第一个存活的单位赋值给返回值
        for (int i=0;i< DamageDealerArray.Count;i++)
        {
            if(DamageDealerArray[i].GetAliveState()==true)
            {
                MinAttackCountUnit = DamageDealerArray[i];
                break;
            }
        }

        // 遍历伤害造成方参战单位并找到一个存活的并且攻击次数最少的单位
        for (int i=1;i< DamageDealerArray.Count;i++)
        {
            if(DamageDealerArray[i].GetAliveState() == true && MinAttackCountUnit.GetAttackCount() > DamageDealerArray[i].GetAttackCount())
            {
                MinAttackCountUnit = DamageDealerArray[i];
            }
        }
        return MinAttackCountUnit;
    }

    // 返回伤害接收方参战单位中随机的一个可攻击单位
    public Unit SelectDamageReceiver(List<Unit> DamageReceiverArray)
    {
        List<Unit> AttackableUnits = new List<Unit>();

        // 在伤害接收方参战单位中筛选出所有的可被攻击并且存活的单位
        for (int i=0;i< DamageReceiverArray.Count;i++)
        {
            if(DamageReceiverArray[i].GetAttakableState() == true && DamageReceiverArray[i].GetAliveState() == true)
            {
                AttackableUnits.Add(DamageReceiverArray[i]);
            }
        }

        // 如果伤害接收方参战单位中所有可被攻击单位均已死亡则筛选剩余所有的存活单位
        if (AttackableUnits.Count == 0)
        {
            for (int i = 0; i < DamageReceiverArray.Count; i++)
            {
                if (DamageReceiverArray[i].GetAliveState() == true)
                {
                    AttackableUnits.Add(DamageReceiverArray[i]);
                }
            }
        }

        // 在筛选出的单位中随机选择一个单位返回
        int SelectIndex = Random.Range(0, AttackableUnits.Count);
        while (SelectIndex == AttackableUnits.Count)
        {
            SelectIndex = (int)Random.Range(0, AttackableUnits.Count);
        }

        return AttackableUnits[SelectIndex];
    }

    public void ExcuteOneAttack(List<Unit> DamageDealerArray, List<Unit> DamageReceiverArray, out int DealerAttackDiff, out int ReceiverAttackDiff)
    {
        Unit DamageDealerUnit = SelectDamageDealer(DamageDealerArray);
        Unit DamageReceiverUnit = SelectDamageReceiver(DamageReceiverArray);

        DamageDealerUnit.ExcuteAttack(DamageReceiverUnit);

        if(DamageReceiverUnit.GetAliveState() == false && DamageReceiverUnit.GetAttackCount() == 0)
        {
            ReceiverAttackDiff =  1;
        }
        else
        {
            ReceiverAttackDiff = 0;
        }

        DealerAttackDiff = 0;
    }

    public void AttackTarget(GameObject AttackTargetPiece)
    {
        //GameObject LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;

        //===========================================================================================
        // 获取进攻协战棋子组和防御协战棋子组
        List<GameObject> AssistAttackPieces = GetAttackPieces();
        List<GameObject> AssistDefendPieces = GetDefendPieces(AttackTargetPiece);

        List<GameObject> AllAttackPieces = new List<GameObject>();
        List<GameObject> AllDefendPieces = new List<GameObject>();

        AllAttackPieces.Add(this.transform.parent.gameObject);
        AllDefendPieces.Add(AttackTargetPiece);
       
        for(int i=0;i< AssistAttackPieces.Count;i++)
        {
            AllAttackPieces.Add(AssistAttackPieces[i]);
        }

        for (int i = 0; i < AssistDefendPieces.Count; i++)
        {
            AllDefendPieces.Add(AssistDefendPieces[i]);
        }       

        BattleInfoSet CurrentBattleInfoSet = new BattleInfoSet();

        // 获取进攻方所有棋子的所有参战单位
        GetBattleUnits(AllAttackPieces, AllDefendPieces, CurrentBattleInfoSet);
        // 获取防御方所有棋子的所有参战单位
        GetBattleUnits(AllDefendPieces, AllAttackPieces, CurrentBattleInfoSet);

        CurrentBattleInfoSet.InitializeCumulativeCooldownTime();

        //CurrentBattleInfoSet.DubugDisplay();

        BattleSequence CurrentBattleSequence = new BattleSequence();
        // 设置战斗执行器的相关参数
        CurrentBattleSequence.SetMainAttackPiece(this.transform.parent.gameObject);
        CurrentBattleSequence.SetMainDefendPiece(AttackTargetPiece);
        CurrentBattleSequence.SetRelativeBattleInfoSet(CurrentBattleInfoSet);

        CurrentBattleInfoSet.FetchActionFromBattleInfoSet(CurrentBattleSequence);

        // CurrentBattleSequence.DisplayAllActionInList();

        CurrentBattleSequence.BattleExcute();

        //===========================================================================================

        //// 0.获取进攻方和防御方的棋子状态机引用
        //PieceStateController DefenderPieceStateController = AttackTargetPiece.GetComponentInChildren<PieceStateController>();
        //PieceStateController AttackerPieceStateController = this.GetComponent<PieceStateController>();
        //
        //// 1.获取进攻方和防御方参与战斗的单位
        //float DefenderPieceSize = DefenderPieceStateController.GetPieceSize();
        //float AttackerPieceSize = AttackerPieceStateController.GetPieceSize();
        //
        ////Debug.Log("DefenderPieceSize:" + DefenderPieceSize);
        ////Debug.Log("AttackerPieceSize:" + AttackerPieceSize);
        //
        //float DefenderUnitSize = DefenderPieceStateController.GetUnitSize();
        //float AttackerUnitSize = AttackerPieceStateController.GetUnitSize();
        //
        //float DefenderPieceRange = 5.0f;
        //float AttackerPieceRange = 6.0f;
        //
        //float DefenderMobilityFactor = DefenderPieceStateController.GetPieceMobilityFactor();
        //float AttackerMobilityFactor = AttackerPieceStateController.GetPieceMobilityFactor();
        //
        //int DefenderBattleUnitNum = CalculateBattleUnitNum(AttackerPieceSize, DefenderUnitSize, DefenderPieceRange, DefenderMobilityFactor);
        //int AttackerBattleUnitNum = CalculateBattleUnitNum(DefenderPieceSize, AttackerUnitSize, AttackerPieceRange, AttackerMobilityFactor);
        //
        ////Debug.Log("DefenderBattleUnitNum:" + DefenderBattleUnitNum);
        ////Debug.Log("AttackerBattleUnitNum:" + AttackerBattleUnitNum);
        //
        //// 2.生成参战单位数组
        //List<Unit> DefenderUnitArray = new List<Unit>();
        //List<Unit> AttackerUnitArray = new List<Unit>();
        //
        //DefenderUnitArray = DefenderPieceStateController.GetRandomBattleUnitArray(DefenderBattleUnitNum);
        //AttackerUnitArray = AttackerPieceStateController.GetRandomBattleUnitArray(AttackerBattleUnitNum);
        //
        ////Debug.Log(DefenderUnitArray.Count);
        ////Debug.Log(AttackerUnitArray.Count);
        //
        //// 3.根据射程计算并标记可攻击单位
        //SetUnitsAttackableOff(DefenderUnitArray);
        //SetUnitsAttackableOff(AttackerUnitArray);
        //if (DefenderPieceRange > AttackerPieceRange)
        //{
        //    float RangeDifference = DefenderPieceRange - AttackerPieceRange;
        //
        //    DefenderUnitArray = SetUnitsAttackableOn(DefenderUnitArray, (int)(DefenderUnitArray.Count * CalculateAttackablePercentage(RangeDifference)));
        //    AttackerUnitArray = SetUnitsAttackableOn(AttackerUnitArray, AttackerUnitArray.Count);
        //}
        //else if(DefenderPieceRange < AttackerPieceRange)
        //{
        //    float RangeDifference = AttackerPieceRange - DefenderPieceRange;
        //
        //    Debug.Log("Percantage:" + AttackerUnitArray.Count * CalculateAttackablePercentage(RangeDifference));
        //
        //    DefenderUnitArray = SetUnitsAttackableOn(DefenderUnitArray, DefenderUnitArray.Count);
        //    AttackerUnitArray = SetUnitsAttackableOn(AttackerUnitArray, (int)(AttackerUnitArray.Count * CalculateAttackablePercentage(RangeDifference)));
        //}
        //else
        //{
        //    DefenderUnitArray = SetUnitsAttackableOn(DefenderUnitArray, DefenderUnitArray.Count);
        //    AttackerUnitArray = SetUnitsAttackableOn(AttackerUnitArray, AttackerUnitArray.Count);
        //}
        //
        //// 4.进行回合制单位攻击演算
        //DebugDisplay(DefenderUnitArray);
        //DebugDisplay(AttackerUnitArray);
        //
        //float DefenderWeaponInterval = 0.8f;
        //float AttackerWeaponInterval = 0.9f;
        //
        //int DefenderAttackCount = 0;
        //int AttackerAttackCount = 0;
        //
        //
        //if(DefenderWeaponInterval == AttackerWeaponInterval)
        //{
        //    // 如果攻击间隔相等则随机决定先手
        //    int TmpInt = Random.Range(1, 2);
        //    if(TmpInt == 1)
        //    {
        //        DefenderWeaponInterval -= 0.01f;
        //    }
        //    else
        //    {
        //        AttackerWeaponInterval -= 0.01f;
        //    }
        //}
        //
        //float DefenderAttackRemain = DefenderWeaponInterval;
        //float AttackerAttackRemain = AttackerWeaponInterval;
        //
        //float MinimumInterval = 0.0f;
        //
        //if(DefenderWeaponInterval < AttackerWeaponInterval)
        //{
        //    MinimumInterval = DefenderWeaponInterval;
        //    DefenderAttackRemain -= DefenderWeaponInterval;
        //    AttackerAttackRemain -= DefenderWeaponInterval;
        //}
        //else
        //{
        //    MinimumInterval = AttackerWeaponInterval;
        //    DefenderAttackRemain -= AttackerWeaponInterval;
        //    AttackerAttackRemain -= AttackerWeaponInterval;
        //}
        //
        //int DefenderAttackSum = DefenderUnitArray.Count;
        //int AttackerAttackSum = AttackerUnitArray.Count;
        //
        //int DealerAttackDiff = 0;
        //int ReceiverAttackDiff = 0;
        //
        //while (DefenderAttackCount < DefenderAttackSum || AttackerAttackCount < AttackerAttackSum)
        //{
        //    if(GetAliveUnitNum(DefenderUnitArray) == 0 || GetAliveUnitNum(AttackerUnitArray) == 0)
        //    {
        //        break;
        //    }
        //
        //    DealerAttackDiff = 0;
        //    ReceiverAttackDiff = 0;
        //
        //    if (DefenderAttackRemain <= 0.0f && AttackerAttackRemain > 0.0f)
        //    {
        //
        //        // 防御方的一个攻击次数最少的单位攻击随机的进攻方单位
        //        ExcuteOneAttack(DefenderUnitArray, AttackerUnitArray, out DealerAttackDiff, out ReceiverAttackDiff);
        //        // 如果死亡的单位是一个未攻击过的单位则 AttackerAttackSum 需要-1
        //        DefenderAttackSum -= DealerAttackDiff;
        //        AttackerAttackSum -= ReceiverAttackDiff;
        //
        //        DefenderAttackCount++;
        //        DefenderAttackRemain = DefenderWeaponInterval;
        //
        //
        //        if (AttackerAttackRemain <= MinimumInterval)
        //        {
        //            DefenderAttackRemain -= AttackerAttackRemain;
        //            AttackerAttackRemain = 0.0f;
        //        }
        //        else
        //        {
        //            AttackerAttackRemain -= MinimumInterval;
        //            DefenderAttackRemain -= MinimumInterval;
        //        }
        //    }
        //    else if(AttackerAttackRemain <= 0.0f && DefenderAttackRemain > 0.0f)
        //    {
        //        // 进攻方的一个攻击次数最少的单位攻击随机的防守方单位
        //        ExcuteOneAttack(AttackerUnitArray, DefenderUnitArray, out DealerAttackDiff, out ReceiverAttackDiff);
        //        // 如果死亡的单位是一个未攻击过的单位则 DefenderAttackSum 需要-1
        //        DefenderAttackSum -= ReceiverAttackDiff;
        //        AttackerAttackSum -= DealerAttackDiff;
        //
        //        AttackerAttackCount++;
        //        AttackerAttackRemain = AttackerWeaponInterval;
        //
        //        if(DefenderAttackRemain <= MinimumInterval)
        //        {
        //            AttackerAttackRemain -= DefenderAttackRemain;
        //            DefenderAttackRemain = 0.0f;
        //        }
        //        else
        //        {
        //            AttackerAttackRemain -= MinimumInterval;
        //            DefenderAttackRemain -= MinimumInterval;
        //        }
        //    }
        //    else if(DefenderAttackRemain <= 0.0f && AttackerAttackRemain <= 0.0f)
        //    {
        //        if(DefenderWeaponInterval > AttackerWeaponInterval)
        //        {
        //            // 进攻方的一个攻击次数最少的单位攻击随机的防守方单位
        //            ExcuteOneAttack(AttackerUnitArray, DefenderUnitArray, out DealerAttackDiff, out ReceiverAttackDiff);
        //            // 如果死亡的单位是一个未攻击过的单位则 DefenderAttackSum 需要-1
        //            DefenderAttackSum -= ReceiverAttackDiff;
        //            AttackerAttackSum -= DealerAttackDiff;
        //
        //            AttackerAttackCount++;
        //
        //            if (DefenderAttackCount >= DefenderAttackSum && AttackerAttackCount >= AttackerAttackSum) break;
        //            if (GetAliveUnitNum(DefenderUnitArray) == 0 || GetAliveUnitNum(AttackerUnitArray) == 0) break;
        //
        //                // 防御方的一个攻击次数最少的单位攻击随机的进攻方单位
        //                ExcuteOneAttack(DefenderUnitArray, AttackerUnitArray, out DealerAttackDiff, out ReceiverAttackDiff);
        //            // 如果死亡的单位是一个未攻击过的单位则 AttackerAttackSum 需要-1
        //            DefenderAttackSum -= DealerAttackDiff;
        //            AttackerAttackSum -= ReceiverAttackDiff;
        //
        //            DefenderAttackCount++;
        //        }
        //        else
        //        {
        //            // 防御方的一个攻击次数最少的单位攻击随机的进攻方单位
        //            ExcuteOneAttack(DefenderUnitArray, AttackerUnitArray, out DealerAttackDiff, out ReceiverAttackDiff);
        //            // 如果死亡的单位是一个未攻击过的单位则 AttackerAttackSum 需要-1
        //            DefenderAttackSum -= DealerAttackDiff;
        //            AttackerAttackSum -= ReceiverAttackDiff;
        //
        //            DefenderAttackCount++;
        //
        //            if (DefenderAttackCount >= DefenderAttackSum && AttackerAttackCount >= AttackerAttackSum) break;
        //            if (GetAliveUnitNum(DefenderUnitArray) == 0 || GetAliveUnitNum(AttackerUnitArray) == 0) break;
        //
        //            // 进攻方的一个攻击次数最少的单位攻击随机的防守方单位
        //            ExcuteOneAttack(AttackerUnitArray, DefenderUnitArray, out DealerAttackDiff, out ReceiverAttackDiff);
        //            // 如果死亡的单位是一个未攻击过的单位则 DefenderAttackSum 需要-1
        //            DefenderAttackSum -= ReceiverAttackDiff;
        //            AttackerAttackSum -= DealerAttackDiff;
        //
        //            AttackerAttackCount++;
        //        }
        //
        //
        //        AttackerAttackRemain = AttackerWeaponInterval;
        //        DefenderAttackRemain = DefenderWeaponInterval;
        //
        //    }
        //}

        //DisplayUnitHealth(AttackerUnitArray);

        // 5.根据战斗结果更新状态


        //LocateChessBoard.GetComponent<ChessBoardStateController>().PieceDie(AttackTargetPiece.transform.parent.gameObject);

        //Destroy(AttackTargetPiece.transform.parent.gameObject);


    }
}
