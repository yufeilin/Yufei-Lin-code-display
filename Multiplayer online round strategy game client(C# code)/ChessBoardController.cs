using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChessBoardController : MonoBehaviour
{
    // 游戏控制器引用
    GameController MainGameController = null;

    // 世界控制器引用
    WorldManager MainWorldManager = null;

    // 单个棋盘格边长
    public float CheckerBoardSize = 1.0f;

    // 当前棋盘每个棋盘格所能容纳的棋子个数
    public int MaxPiecePerChecker = 4;

    // 当前棋盘的尺寸，默认值为8
    public int ChessBoardSize = 8;

    // 当前棋盘在棋盘阵列中的坐标(人为定义的位置参数)
    public Vector2Int ChessBoardOuterCoordinate = new Vector2Int();

    // 当前棋盘在棋盘阵列中的位置(在世界空间中的transform.position)
    public Vector2 ChessBoardOuterPosition = new Vector2();

    // 当前棋盘的坐标(0,0)棋盘格的中心点位置
    public Vector3 InitPosition = new Vector3();

    public Vector3[,] CheckerBoardPosition = null;

    // 棋盘格预制体
    public GameObject CheckerBoardBasePrefab;

    // 黑色棋盘格和白色棋盘格材质
    public Material[] CheckerBoardMaterials = null;


    // 当前棋盘每个棋盘格所容纳棋子的引用数组
    // CheckerPiece[i, j, k] 代表在当前棋盘格坐标为(i, j)处的第k个棋子
    // k = 0 为棋盘格最顶层棋子，当一个棋子想要攻击一个容纳多个棋子的棋盘格时相当于与该棋盘格的最顶层棋子进行一次战斗结算
    //public GameObject[,,] CheckerPiece = null;

    public Dictionary<int, List<GameObject>>[,] CheckerBoardPlayerAlivePiece = null;

    public Dictionary<int, List<GameObject>>[,] CheckerBoardPlayerDeadPiece = null;

    int PlayerNum = 2;

    // 当前棋盘每个棋盘格的引用数组
    public GameObject[,] CheckerBoardArray = null;

    public bool[,] CheckerBoardEnable = null;

    // 当前棋盘每个棋盘格所容纳棋子的数目
    int[,] CheckerPieceNum = null;

    //public ChessBoardController(WorldManager ParaWorldManager)
    //{
    //    // 控制器引用赋值
    //    MainWorldManager = ParaWorldManager;
    //    MainGameController = ParaWorldManager.GetGameController();
    //    PlayerNum = MainGameController.GetPlayerNum();
    //
    //    // 全局参数拷贝
    //    CheckerBoardSize = MainWorldManager.GetCheckerBoardSize();
    //    MaxPiecePerChecker = MainWorldManager.GetMaxPiecePerChecker();
    //    ChessBoardSize = MainWorldManager.GetChessBoardSize();
    //
    //    CheckerBoardPosition = new Vector3[ChessBoardSize, ChessBoardSize];
    //
    //    ChessBoardOuterPosition.x = ChessBoardOuterCoordinate.x * ChessBoardSize * CheckerBoardSize;
    //    ChessBoardOuterPosition.y = ChessBoardOuterCoordinate.y * ChessBoardSize * CheckerBoardSize;
    //
    //    if (ChessBoardSize % 2 == 0)
    //    {
    //        InitPosition = new Vector3(ChessBoardOuterPosition.x - (ChessBoardSize / 2.0f - 0.5f) * CheckerBoardSize, 0.0f, ChessBoardOuterPosition.y - (ChessBoardSize / 2.0f - 0.5f) * CheckerBoardSize);
    //    }
    //    else
    //    {
    //        InitPosition = new Vector3(ChessBoardOuterPosition.x - (ChessBoardSize / 2.0f - 0.5f) * CheckerBoardSize, 0.0f, ChessBoardOuterPosition.y - (ChessBoardSize / 2.0f - 0.5f) * CheckerBoardSize);
    //    }
    //
    //    Vector3 CurrentPosition = new Vector3();
    //
    //    for (int i = 0; i < ChessBoardSize; i++)
    //    {
    //        for (int j = 0; j < ChessBoardSize; j++)
    //        {
    //            CurrentPosition = new Vector3(InitPosition.x + i * CheckerBoardSize, InitPosition.y, InitPosition.z + j * CheckerBoardSize);
    //
    //            CheckerBoardPosition[i, j] = CurrentPosition;
    //        }
    //    }
    //
    //    // 初始化棋盘格预制体
    //    CheckerBoardBasePrefab = (GameObject)Resources.Load("Prefabs/CheckerBoardBase");
    //
    //    // 初始化棋盘格材质
    //    CheckerBoardMaterials[0] = (Material)Resources.Load("Material/CheckerBoardBlack");
    //    CheckerBoardMaterials[1] = (Material)Resources.Load("Material/CheckerBoardWhite");
    //
    //    // 初始化棋盘格玩家存活棋子数组
    //    CheckerBoardPlayerAlivePiece = new Dictionary<int, List<GameObject>>[ChessBoardSize, ChessBoardSize];
    //    // 初始化棋盘格玩家阵亡棋子数组
    //    CheckerBoardPlayerDeadPiece = new Dictionary<int, List<GameObject>>[ChessBoardSize, ChessBoardSize];
    //
    //    //==============================================================================
    //
    //    // 初始化棋盘格引用数组
    //    CheckerBoardArray = new GameObject[ChessBoardSize, ChessBoardSize];
    //
    //    // 初始化棋盘格状态数组
    //    CheckerBoardEnable = new bool[ChessBoardSize, ChessBoardSize];
    //
    //    // 在每个棋盘格分别初始化玩家存活棋子数组和玩家阵亡棋子数组
    //    for (int i = 0; i < ChessBoardSize; i++)
    //    {
    //        for (int j = 0; j < ChessBoardSize; j++)
    //        {
    //            CheckerBoardEnable[i, j] = true;
    //
    //            CheckerBoardPlayerAlivePiece[i, j] = new Dictionary<int, List<GameObject>>();
    //            CheckerBoardPlayerDeadPiece[i, j] = new Dictionary<int, List<GameObject>>();
    //            for (int k = 0; k < PlayerNum; k++)
    //            {
    //                CheckerBoardPlayerAlivePiece[i, j].Add(k, new List<GameObject>());
    //                CheckerBoardPlayerDeadPiece[i, j].Add(k, new List<GameObject>());
    //            }
    //        }
    //    }
    //
    //
    //}

    public void Initialize(WorldManager ParaWorldManager, Vector2Int NewChessBoardOuterCoordinate)
    {
        // 控制器引用赋值
        MainWorldManager = ParaWorldManager;
        ChessBoardOuterCoordinate = NewChessBoardOuterCoordinate;
        MainGameController = ParaWorldManager.GetGameController();
        PlayerNum = MainGameController.GetPlayerNum();

        // 全局参数拷贝
        CheckerBoardSize = MainWorldManager.GetCheckerBoardSize();
        MaxPiecePerChecker = MainWorldManager.GetMaxPiecePerChecker();
        ChessBoardSize = MainWorldManager.GetChessBoardSize();

        CheckerBoardPosition = new Vector3[ChessBoardSize, ChessBoardSize];

        ChessBoardOuterPosition.x = ChessBoardOuterCoordinate.x * ChessBoardSize * CheckerBoardSize;
        ChessBoardOuterPosition.y = ChessBoardOuterCoordinate.y * ChessBoardSize * CheckerBoardSize;

        if (ChessBoardSize % 2 == 0)
        {
            InitPosition = new Vector3(ChessBoardOuterPosition.x - (ChessBoardSize / 2.0f - 0.5f) * CheckerBoardSize, 0.0f, ChessBoardOuterPosition.y - (ChessBoardSize / 2.0f - 0.5f) * CheckerBoardSize);
        }
        else
        {
            InitPosition = new Vector3(ChessBoardOuterPosition.x - (ChessBoardSize / 2.0f - 0.5f) * CheckerBoardSize, 0.0f, ChessBoardOuterPosition.y - (ChessBoardSize / 2.0f - 0.5f) * CheckerBoardSize);
        }

        Vector3 CurrentPosition = new Vector3();

        for (int i = 0; i < ChessBoardSize; i++)
        {
            for (int j = 0; j < ChessBoardSize; j++)
            {
                CurrentPosition = new Vector3(InitPosition.x + i * CheckerBoardSize, InitPosition.y, InitPosition.z + j * CheckerBoardSize);

                CheckerBoardPosition[i, j] = CurrentPosition;
            }
        }

        // 初始化棋盘格预制体
        CheckerBoardBasePrefab = (GameObject)Resources.Load("Prefabs/CheckerBoardBase");

        CheckerBoardMaterials = new Material[2];

        // 初始化棋盘格材质
        CheckerBoardMaterials[0] = (Material)Resources.Load("Material/CheckerBoardBlack");
        CheckerBoardMaterials[1] = (Material)Resources.Load("Material/CheckerBoardWhite");

        // 初始化棋盘格玩家存活棋子数组
        CheckerBoardPlayerAlivePiece = new Dictionary<int, List<GameObject>>[ChessBoardSize, ChessBoardSize];
        // 初始化棋盘格玩家阵亡棋子数组
        CheckerBoardPlayerDeadPiece = new Dictionary<int, List<GameObject>>[ChessBoardSize, ChessBoardSize];

        //==============================================================================

        // 初始化棋盘格引用数组
        CheckerBoardArray = new GameObject[ChessBoardSize, ChessBoardSize];

        // 初始化棋盘格状态数组
        CheckerBoardEnable = new bool[ChessBoardSize, ChessBoardSize];

        // 在每个棋盘格分别初始化玩家存活棋子数组和玩家阵亡棋子数组
        for (int i = 0; i < ChessBoardSize; i++)
        {
            for (int j = 0; j < ChessBoardSize; j++)
            {
                CheckerBoardEnable[i, j] = true;

                CheckerBoardPlayerAlivePiece[i, j] = new Dictionary<int, List<GameObject>>();
                CheckerBoardPlayerDeadPiece[i, j] = new Dictionary<int, List<GameObject>>();
                for (int k = 0; k < PlayerNum; k++)
                {
                    CheckerBoardPlayerAlivePiece[i, j].Add(k, new List<GameObject>());
                    CheckerBoardPlayerDeadPiece[i, j].Add(k, new List<GameObject>());
                }
            }
        }
    }

    public WorldManager GetWorldManager() { return MainWorldManager; }

    // 获取当前棋盘中内坐标为InterCoord的棋盘格的位置
    public Vector3 GetCheckerboardPosition(Vector2 InterCoord)
    {
        return CheckerBoardPosition[(int)InterCoord.x, (int)InterCoord.y];
    }

    // 获取当前棋盘中内坐标为InterCoord的棋盘格的状态
    public bool GetCheckerBoardEnable(Vector2Int InterCoord)
    {
        return CheckerBoardEnable[InterCoord.x, InterCoord.y];
    }

    // 获取当前棋盘坐标为InterCoord的棋盘格
    public GameObject GetCheckerBoard(Vector2Int InterCoord)
    {
        return CheckerBoardArray[InterCoord.x, InterCoord.y];
    }

    // 获取当前棋盘坐标为InterCoord棋盘格的存活的属于玩家PlayerID的第layer层的棋子
    public GameObject GetCheckerBoardPlayerAlivePiece(Vector2Int InterCoord, int PlayerID, int layer = 0)
    {
        if (CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y].ContainsKey(PlayerID))
        {
            if (CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Count > layer)
            {
                return CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID][layer];
            }
        }
        else
        {
            CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y].Add(PlayerID, new List<GameObject>());
        }

        return null;

        //return CheckerPiece[InterCoord.x, InterCoord.y, layer];
    }

    // 获取当前棋盘坐标为InterCoord棋盘格的阵亡的的属于玩家PlayerID的第layer层的棋子
    public GameObject GetCheckerBoardPlayerDeadPiece(Vector2Int InterCoord, int PlayerID, int layer = 0)
    {
        if (CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y].ContainsKey(PlayerID))
        {
            if (CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y][PlayerID].Count > layer)
            {
                return CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y][PlayerID][layer];
            }
        }
        else
        {
            CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y].Add(PlayerID, new List<GameObject>());
        }

        return null;
    }

    // 获取当前棋盘坐标为(InterCoord.x, InterCoord.y)处棋盘格所容纳的存活的属于玩家PlayerID的棋子数目
    public int GetCheckerBoardPlayerAlivePieceNum(Vector2Int InterCoord, int PlayerID)
    {
        if (CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y].ContainsKey(PlayerID))
        {
            return CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Count;
        }
        else
        {
            CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y].Add(PlayerID, new List<GameObject>());
        }

        return 0;
        //return CheckerPieceNum[(int)InterCoord.x, (int)InterCoord.y];
    }

    // 获取当前棋盘坐标为(InterCoord.x, InterCoord.y)处棋盘格所容纳的阵亡的属于玩家PlayerID的棋子数目
    public int GetCheckerBoardPlayerDeadPieceNum(Vector2Int InterCoord, int PlayerID)
    {
        if (CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y].ContainsKey(PlayerID))
        {
            return CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y][PlayerID].Count;
        }
        else
        {
            CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y].Add(PlayerID, new List<GameObject>());
        }

        return 0;
        //return CheckerPieceNum[(int)InterCoord.x, (int)InterCoord.y];
    }

    // 获取进攻棋子AttackPiece所在棋盘格其他的具有协同进攻标旗的棋子
    public List<GameObject> GetAttackPieces(GameObject AttackPiece)
    {
        List<GameObject> Res = new List<GameObject>();

        Vector2Int AttackPieceInterCoord = AttackPiece.GetComponentInChildren<PieceStateController>().ChessBoardInternalCoordinate;
        int PlayerID = AttackPiece.GetComponentInChildren<PieceStateController>().GetOwner();

        int PieceNum = GetCheckerBoardPlayerAlivePieceNum(AttackPieceInterCoord, PlayerID);

        // 这里应该遍历所有玩家在此处因为盟友的棋子也要包括

        for (int i = 0; i < PieceNum; i++)
        {
            if (GetCheckerBoardPlayerAlivePiece(AttackPieceInterCoord, PlayerID, i) == AttackPiece)
            {
                continue;
            }
            else if (GetCheckerBoardPlayerAlivePiece(AttackPieceInterCoord, PlayerID, i).GetComponentInChildren<PieceInfoController>().GetAssistAttackFlag() == true)
            {
                Res.Add(GetCheckerBoardPlayerAlivePiece(AttackPieceInterCoord, PlayerID, i));
            }
        }

        return Res;
    }

    // 获取防御棋子DefendPiece所在棋盘格其他的具有协同防御标旗的棋子
    public List<GameObject> GetDefendPieces(GameObject DefendPiece)
    {
        List<GameObject> Res = new List<GameObject>();

        Vector2Int DefendPieceInterCoord = DefendPiece.GetComponentInChildren<PieceStateController>().ChessBoardInternalCoordinate;
        int PlayerID = DefendPiece.GetComponentInChildren<PieceStateController>().GetOwner();

        int PieceNum = GetCheckerBoardPlayerAlivePieceNum(DefendPieceInterCoord, PlayerID);

        // 这里应该遍历所有玩家在此处因为盟友的棋子也要包括

        for (int i = 0; i < PieceNum; i++)
        {
            if (GetCheckerBoardPlayerAlivePiece(DefendPieceInterCoord, PlayerID, i) == DefendPiece)
            {
                continue;
            }
            else if (GetCheckerBoardPlayerAlivePiece(DefendPieceInterCoord, PlayerID, i).GetComponentInChildren<PieceInfoController>().GetAssistDefendFlag() == true)
            {
                Res.Add(GetCheckerBoardPlayerAlivePiece(DefendPieceInterCoord, PlayerID, i));
            }
        }

        return Res;
    }

    // 在棋盘状态更新完毕后用于更新棋盘格上显示的棋子(对于玩家PlayID)
    public void RefreshPieceOnCheckerBoard(Vector2Int InterCoord, Vector2Int OuterCoord, int PlayerID)
    {
        // 在坐标为OuterCoord的棋盘上坐标为InterCoord的棋盘格上
        // 将0层的棋子设为显示，其余层的棋子设为不显示
        GameObject LocateChessBoard = this.GetComponent<ChessBoardController>().MainWorldManager.GetChessBoard(OuterCoord);

        ChessBoardController LocateChessBoardController = LocateChessBoard.GetComponent<ChessBoardController>();

        for (int i = 0; i < LocateChessBoardController.GetCheckerBoardPlayerAlivePieceNum(InterCoord, PlayerID); i++)
        {
            //Debug.Log("((((" + LocateChessBoardStateController.GetCheckerPiece(InterCoord, i).name + "))))");
            if (i == 0)
            {
                if (LocateChessBoardController.GetCheckerBoardPlayerAlivePiece(InterCoord, PlayerID, i) == null)
                {
                    Debug.Log("get piece is null");
                    Debug.Log(LocateChessBoardController.GetCheckerBoardPlayerAlivePieceNum(InterCoord, PlayerID));
                }
                //LocateChessBoardStateController.GetCheckerPiece(InterCoord, i).SetActive(true);
                LocateChessBoardController.GetCheckerBoardPlayerAlivePiece(InterCoord, PlayerID, i).GetComponentInChildren<PieceOperationController>().DisplayPiece();
            }
            else
            {
                //LocateChessBoardStateController.GetCheckerPiece(InterCoord, i).SetActive(false);
                LocateChessBoardController.GetCheckerBoardPlayerAlivePiece(InterCoord, PlayerID, i).GetComponentInChildren<PieceOperationController>().HidePiece();
            }
        }
    }

    public void UpdateCheckerBoardPlayerAlivePieceLayer(Vector2Int InterCoord, int PlayerID)
    {
        for (int i = 0; i < CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Count; i++)
        {
            CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID][i].GetComponentInChildren<PieceStateController>().SetPieceLayer(i);
        }
    }

    // 在当前棋盘坐标为(InterCoord.x, InterCoord.y)处棋盘格最顶层layer = 0生成一个棋子并将所有原有棋子依次向底层移动一层
    // 参数 Piece 为棋子的最顶层父物体而不是父物体下的 PieceMainframe 物体
    public void PieceInit(GameObject Piece)
    {
        // 从Piece物体下方的子物体PieceMainframe中获取棋子的相关信息
        Vector2Int InterCoord = Piece.GetComponentInChildren<PieceStateController>().ChessBoardInternalCoordinate;
        Vector2Int OuterCoord = Piece.GetComponentInChildren<PieceStateController>().ChessBoardOuterCoordinate;
        int PlayerID = Piece.GetComponentInChildren<PieceStateController>().GetOwner();

        if (!CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y].ContainsKey(PlayerID))
        {
            CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y].Add(PlayerID, new List<GameObject>());
        }

        // 将新单位插入list的0位置
        CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Insert(0, Piece);
        // 更新list中所有棋子的层级信息
        UpdateCheckerBoardPlayerAlivePieceLayer(InterCoord, PlayerID);

        RefreshPieceOnCheckerBoard(InterCoord, OuterCoord, PlayerID);
    }

    public void PieceInit(GameObject Piece, Vector2Int InterCoord, Vector2Int OuterCoord)
    {
        int PlayerID = Piece.GetComponentInChildren<PieceStateController>().GetOwner();

        if (!CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y].ContainsKey(PlayerID))
        {
            CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y].Add(PlayerID, new List<GameObject>());
        }

        // 将新单位插入list的0位置
        CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Insert(0, Piece);
        // 更新list中所有棋子的层级信息
        UpdateCheckerBoardPlayerAlivePieceLayer(InterCoord, PlayerID);

        RefreshPieceOnCheckerBoard(InterCoord, OuterCoord, PlayerID);
    }

    // 将棋子Piece从其当前所在位置移动到当前棋盘坐标为(NewInterCoord.x, NewInterCoord.y)处棋盘格的NewLayer层
    // 参数 Piece 为棋子的最顶层父物体而不是父物体下的 PieceMainframe 物体
    public void PieceMove(GameObject Piece, Vector2Int NewInterCoord, Vector2Int NewOuterCoord, int NewLayer)
    {
        // 从Piece物体下方的子物体PieceMainframe中获取棋子的相关信息
        Vector2Int InterCoord = Piece.GetComponentInChildren<PieceStateController>().ChessBoardInternalCoordinate;
        Vector2Int OuterCoord = Piece.GetComponentInChildren<PieceStateController>().ChessBoardOuterCoordinate;
        int PlayerID = Piece.GetComponentInChildren<PieceStateController>().GetOwner();
        int PieceLayer = Piece.GetComponentInChildren<PieceStateController>().GetPieceLayer();

        int CoordX = InterCoord.x;
        int CoordY = InterCoord.y;

        GameObject MovePiece = Piece;

        CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Remove(Piece);

        Debug.Log("origin checkerboard piece num is " + CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Count);

        // 更新list中所有棋子的层级信息
        UpdateCheckerBoardPlayerAlivePieceLayer(InterCoord, PlayerID);

        RefreshPieceOnCheckerBoard(InterCoord, OuterCoord, PlayerID);
        //Debug.Log(CheckerPieceNum[CoordX, CoordY]);

        GameObject NewChessBoard = this.GetComponent<ChessBoardController>().MainWorldManager.GetChessBoard(NewOuterCoord);

        NewChessBoard.GetComponent<ChessBoardController>().PieceInit(MovePiece, NewInterCoord, NewOuterCoord);

        RefreshPieceOnCheckerBoard(NewInterCoord, NewOuterCoord, PlayerID);
    }

    public void PieceChangeOwner(GameObject Piece, int NewOwnerID)
    {
        Vector2Int InterCoord = Piece.GetComponentInChildren<PieceStateController>().ChessBoardInternalCoordinate;
        Vector2Int OuterCoord = Piece.GetComponentInChildren<PieceStateController>().ChessBoardOuterCoordinate;
        int PlayerID = Piece.GetComponentInChildren<PieceStateController>().GetOwner();

        CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Remove(Piece);
        UpdateCheckerBoardPlayerAlivePieceLayer(InterCoord, PlayerID);

        CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][NewOwnerID].Add(Piece);
        UpdateCheckerBoardPlayerAlivePieceLayer(InterCoord, NewOwnerID);

    }

    // 从棋子Piece当前所在位置移除该棋子并将所有其他棋子依次向顶层移动一层
    // 参数 Piece 为棋子的最顶层父物体而不是父物体下的 PieceMainframe 物体
    public void PieceDie(GameObject Piece)
    {
        // 从Piece物体下方的子物体PieceMainframe中获取棋子的相关信息
        Vector2Int InterCoord = Piece.GetComponentInChildren<PieceStateController>().ChessBoardInternalCoordinate;
        Vector2Int OuterCoord = Piece.GetComponentInChildren<PieceStateController>().ChessBoardOuterCoordinate;
        int PlayerID = Piece.GetComponentInChildren<PieceStateController>().GetOwner();

        CheckerBoardPlayerAlivePiece[InterCoord.x, InterCoord.y][PlayerID].Remove(Piece);

        UpdateCheckerBoardPlayerAlivePieceLayer(InterCoord, PlayerID);

        CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y][PlayerID].Add(Piece);

        Debug.Log("dead piece list count:" + CheckerBoardPlayerDeadPiece[InterCoord.x, InterCoord.y][PlayerID].Count);

        RefreshPieceOnCheckerBoard(InterCoord, OuterCoord, PlayerID);
    }

    // 在当前棋盘生成棋盘格
    public void CreateCheckerBoard()
    {
        //GameObject CheckerBoardBasePrefab = ThisChessBoardInfoController.CheckerBoardBasePrefab;

        Vector3 CurrentPosition = new Vector3();

        for (int i = 0; i < ChessBoardSize; i++)
        {
            for (int j = 0; j < ChessBoardSize; j++)
            {
                CurrentPosition = GetCheckerboardPosition(new Vector2(i, j));

                GameObject CurrentCheckerBoard = Instantiate(CheckerBoardBasePrefab, CurrentPosition, Quaternion.identity);

                CurrentCheckerBoard.GetComponent<Transform>().localScale = new Vector3(CheckerBoardSize, 0.1f, CheckerBoardSize);

                CurrentCheckerBoard.GetComponent<MeshRenderer>().material = CheckerBoardMaterials[(i + j) % 2];

                CurrentCheckerBoard.transform.parent = this.gameObject.transform;

                CurrentCheckerBoard.GetComponent<CheckerBoardInfoController>().SetCoordinate(new Vector2Int(i, j), CurrentPosition, new Vector2Int((int)ChessBoardOuterCoordinate.x, (int)ChessBoardOuterCoordinate.y));

                CurrentCheckerBoard.GetComponent<CheckerBoardInfoController>().LocateChessboard = this.gameObject;

                CheckerBoardArray[i, j] = CurrentCheckerBoard;
            }
        }
    }

    // 在当前棋盘坐标为(InterCoord.x, InterCoord.y)处的棋盘格最顶层layer = 0 创建一个ID为Piece的棋子并返回该棋子的引用
    public GameObject CreatePieceOnChessBoard(Vector2Int InterCoord, Piece PieceTamplate)
    {
        // TODO 我觉得这里需要修改的emmmm
        //GameObject GameManager = GameObject.Find("GameManager");

        GameObject PiecePrefab = (GameObject)Resources.Load("Prefabs/PieceEmpty");

        GameObject CurrentPiece = Instantiate(PiecePrefab, GetCheckerboardPosition(InterCoord), Quaternion.identity);

        CurrentPiece.GetComponentInChildren<PieceStateController>().Initialize(this.gameObject, InterCoord, PieceTamplate);

        PieceInit(CurrentPiece);

        return CurrentPiece;
    }

    public void DeployPieceOnChessBoard(GameObject ParaPiece, Vector2Int InterCoord)
    {
        ParaPiece.GetComponentInChildren<PieceStateController>().DeployPiece(this.gameObject, InterCoord);

        PieceInit(ParaPiece);
    }

    public bool CheckerBoardAvaliable(Vector2Int InterCoord, out Vector2Int OutInterCoord, out Vector2Int OutOuterCoord)
    {
        Vector2 Tmp = this.GetComponent<ChessBoardController>().ChessBoardOuterCoordinate;
        Vector2Int OuterCoord = new Vector2Int((int)Tmp.x, (int)Tmp.y);

        if (this.GetComponent<ChessBoardController>().MainWorldManager.GetCheckerBoard(InterCoord, OuterCoord, out OutInterCoord, out OutOuterCoord) != null)
        {
            return true;
        }
        else
        {
            return false;
        }
    }


}
