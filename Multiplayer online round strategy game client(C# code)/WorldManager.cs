using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WorldManager
{
    // 游戏控制器引用
    GameController MainGameController = null;

    // 棋盘矩阵的尺寸
    public int WorldSize = 4;

    // 当前棋盘的尺寸，默认值为8
    public int ChessBoardSize = 16;

    // 每个棋盘格所能容纳的最大棋子数目
    public int MaxPiecePerChecker = 4;

    // 单个棋盘格边长
    public float CheckerBoardSize = 8.0f;

    // 棋盘状态阵列(在当前世界中坐标为(x,y)的棋盘的状态)
    // ChessBoardState[x, y] = 0 该坐标尚未初始化棋盘
    // ChessBoardState[x, y] = 1 该座标已初始化棋盘并且棋盘处于开启状态
    // ChessBoardState[x, y] = -1 该座标已初始化棋盘并且棋盘处于关闭状态
    int[,] ChessBoardState = null;

    // 棋盘引用阵列(在当前世界中坐标为(x,y)的棋盘的引用)
    GameObject[,] ChessBoardArray = null;

    // 棋盘预制体引用
    GameObject ChessBoardPrefab = null;

    // 游戏数据库引用
    LibraryManager MainLibraryManager = null;

    public WorldManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;

        ChessBoardPrefab = (GameObject)Resources.Load("Prefabs/ChessBoard");

        MainLibraryManager = MainGameController.GetLibraryManager();

        // 从XML载入信息(棋盘矩阵尺寸，棋盘尺寸，最大棋子容纳数目， 棋盘格边长)

        // 初始化棋盘状态阵列
        ChessBoardState = new int[WorldSize, WorldSize];
        for (int i = 0; i < WorldSize; i++)
        {
            for (int j = 0; j < WorldSize; j++)
            {
                ChessBoardState[i, j] = 0;
            }
        }

        // 初始化棋盘引用阵列
        ChessBoardArray = new GameObject[WorldSize, WorldSize];
        for (int i = 0; i < WorldSize; i++)
        {
            for (int j = 0; j < WorldSize; j++)
            {
                ChessBoardArray[i, j] = null;
            }
        }
    }

    public void EstablishReference()
    {

    }

    public GameController GetGameController() { return MainGameController; }

    public int GetWorldSize() { return WorldSize; }

    public int GetChessBoardSize() { return ChessBoardSize; }

    public int GetMaxPiecePerChecker() { return MaxPiecePerChecker; }

    public float GetCheckerBoardSize() { return CheckerBoardSize; }

    public int GetChessBoardState(Vector2Int OuterCoord) { return ChessBoardState[OuterCoord.x, OuterCoord.y]; }

    public GameObject GetChessBoard(Vector2 OuterCoord) { return ChessBoardArray[(int)OuterCoord.x, (int)OuterCoord.y]; }

    public void ChessBoardInit(GameObject NewChessBoard)
    {
        Vector2 OuterCoord = NewChessBoard.GetComponent<ChessBoardController>().ChessBoardOuterCoordinate;

        ChessBoardArray[(int)OuterCoord.x, (int)OuterCoord.y] = NewChessBoard;

        ChessBoardState[(int)OuterCoord.x, (int)OuterCoord.y] = 1;

        //Debug.Log(ChessBoardArray[(int)OuterCoord.x, (int)OuterCoord.y]);
        //Debug.Log(ChessBoardState[(int)OuterCoord.x, (int)OuterCoord.y]);
    }

    // 在当前世界OuterCoord坐标所代表的位置创建一个棋盘
    public GameObject CreateChessboard(Vector2Int OuterCoord)
    {
        // 使用棋盘预制体创建棋盘父物体
        GameObject CurrentChessBoard = GameObject.Instantiate(ChessBoardPrefab, new Vector3(OuterCoord.x, 0.0f, OuterCoord.y), Quaternion.identity);
        //GameObject ChessBoard = new GameObject();
        CurrentChessBoard.name = "ChessBoard(" + OuterCoord.x + ", " + OuterCoord.y + ")";
        //CurrentChessBoard.transform.position = new Vector3(OuterCoord.x, 0.0f, OuterCoord.y);

        // 初始化棋盘信息控制器中的常量
        ChessBoardController CurrentChessBoardController = CurrentChessBoard.GetComponent<ChessBoardController>();
        CurrentChessBoardController.Initialize(this, OuterCoord);


        //CurrentChessBoardInfoController.MaxPiecePerChecker = MaxPiecePerChecker;
        //CurrentChessBoardInfoController.CheckerBoardSize = CheckerBoardSize;
        //CurrentChessBoardInfoController.ChessBoardSize = ChessBoardSize;
        //CurrentChessBoardInfoController.ChessBoardOuterCoordinate = OuterCoord;
        //CurrentChessBoardInfoController.MainWorldManager = this;

        // 向世界状态控制器注册创建的新棋盘
        ChessBoardInit(CurrentChessBoard);

        // 初始化创建棋盘的信息控制器
        //CurrentChessBoardInfoController.Initialize();

        // 调用创建棋盘的操作控制器生成棋盘的棋盘格
        CurrentChessBoardController.CreateCheckerBoard();

        return CurrentChessBoard;
    }

    // 在坐标为OuterCoord的棋盘中坐标为InterCoord的棋盘格创建一个棋子名为PieceName的棋子并返回其引用
    public GameObject CreatePieceOnChessboard(string PieceName, Vector2Int InterCoord, Vector2Int OuterCoord)
    {
        Piece PieceTamplate = MainLibraryManager.GetPiece(PieceName);

        return GetChessBoard(OuterCoord).GetComponent<ChessBoardController>().CreatePieceOnChessBoard(InterCoord, PieceTamplate);
    }

    // 在棋盘外(该棋子尚未被部署到棋盘上)创建一个名为PieceName的棋子并返回其引用(这些棋子是隐藏的而且在随从卡牌没有施放之前都处于不可见状态)
    public GameObject CreatePieceForCard(string PieceName)
    {
        Piece PieceTamplate = MainLibraryManager.GetPiece(PieceName);

        GameObject PiecePrefab = (GameObject)Resources.Load("Prefabs/PieceEmpty");

        GameObject CurrentPiece = GameObject.Instantiate(PiecePrefab, new Vector3(0, 0, 0), Quaternion.identity);

        CurrentPiece.GetComponentInChildren<PieceStateController>().Initialize(PieceTamplate);

        return CurrentPiece;
    }

    public void DeployPieceOnChessBoard(GameObject ParaPiece, Vector2Int InterCoord, Vector2Int OuterCoord)
    {
        GetChessBoard(OuterCoord).GetComponent<ChessBoardController>().DeployPieceOnChessBoard(ParaPiece, InterCoord);
    }

    public GameObject GetCheckerBoard(Vector2Int InterCoord, Vector2Int OuterCoord)
    {
        if (InterCoord.x >= 0 && InterCoord.x < ChessBoardSize && InterCoord.y >= 0 && InterCoord.y < ChessBoardSize)
        {
            return GetChessBoard(OuterCoord).GetComponent<ChessBoardController>().GetCheckerBoard(InterCoord);
        }

        Vector2Int NewInterCoord = new Vector2Int();

        Vector2Int NewOuterCoord = new Vector2Int();

        Debug.Log(InterCoord + " " + OuterCoord);

        if (InterCoord.x > 0)
        {
            NewInterCoord.x = InterCoord.x % ChessBoardSize;
            NewOuterCoord.x = OuterCoord.x + InterCoord.x / ChessBoardSize;
        }
        else if (InterCoord.x < 0)
        {
            NewInterCoord.x = (((-(InterCoord.x + 1)) / ChessBoardSize) + 1) * ChessBoardSize + InterCoord.x;

            NewOuterCoord.x = OuterCoord.x - (((-(InterCoord.x + 1)) / ChessBoardSize) + 1);
        }

        if (InterCoord.y > 0)
        {
            NewInterCoord.y = InterCoord.y % ChessBoardSize;
            NewOuterCoord.y = OuterCoord.y + InterCoord.y / ChessBoardSize;
        }
        else if (InterCoord.y < 0)
        {
            Debug.Log(InterCoord.y);

            NewInterCoord.y = (((-(InterCoord.y + 1)) / ChessBoardSize) + 1) * ChessBoardSize + InterCoord.y;

            NewOuterCoord.y = OuterCoord.y - (((-(InterCoord.y + 1)) / ChessBoardSize) + 1);
        }

        Debug.Log(NewInterCoord + " " + NewOuterCoord);

        if (NewOuterCoord.x >= 0 && NewOuterCoord.x < WorldSize && NewOuterCoord.y >= 0 && NewOuterCoord.y < WorldSize)
        {
            if (GetChessBoardState(NewOuterCoord) == 1)
            {
                return GetChessBoard(NewOuterCoord).GetComponent<ChessBoardController>().GetCheckerBoard(NewInterCoord);
            }
        }

        return null;
    }

    public GameObject GetCheckerBoard(Vector2Int InterCoord, Vector2Int OuterCoord, out Vector2Int OutInterCoord, out Vector2Int OutOuterCoord)
    {
        if (InterCoord.x >= 0 && InterCoord.x < ChessBoardSize && InterCoord.y >= 0 && InterCoord.y < ChessBoardSize)
        {
            OutInterCoord = InterCoord;
            OutOuterCoord = OuterCoord;
            return GetChessBoard(OuterCoord).GetComponent<ChessBoardController>().GetCheckerBoard(InterCoord);
        }

        Vector2Int NewInterCoord = new Vector2Int();

        Vector2Int NewOuterCoord = new Vector2Int();

        //Debug.Log(InterCoord + " " + OuterCoord);

        if (InterCoord.x > 0)
        {
            NewInterCoord.x = InterCoord.x % ChessBoardSize;
            NewOuterCoord.x = OuterCoord.x + InterCoord.x / ChessBoardSize;
        }
        else if (InterCoord.x < 0)
        {
            NewInterCoord.x = (((-(InterCoord.x + 1)) / ChessBoardSize) + 1) * ChessBoardSize + InterCoord.x;

            NewOuterCoord.x = OuterCoord.x - (((-(InterCoord.x + 1)) / ChessBoardSize) + 1);
        }

        if (InterCoord.y > 0)
        {
            NewInterCoord.y = InterCoord.y % ChessBoardSize;
            NewOuterCoord.y = OuterCoord.y + InterCoord.y / ChessBoardSize;
        }
        else if (InterCoord.y < 0)
        {
            //Debug.Log(InterCoord.y);

            NewInterCoord.y = (((-(InterCoord.y + 1)) / ChessBoardSize) + 1) * ChessBoardSize + InterCoord.y;

            NewOuterCoord.y = OuterCoord.y - (((-(InterCoord.y + 1)) / ChessBoardSize) + 1);
        }

        //Debug.Log(NewInterCoord + " " + NewOuterCoord);

        if (NewOuterCoord.x >= 0 && NewOuterCoord.x < WorldSize && NewOuterCoord.y >= 0 && NewOuterCoord.y < WorldSize)
        {
            // 检查坐标为NewOuterCoord的棋盘是否存在
            if (GetChessBoardState(NewOuterCoord) == 1)
            {
                OutInterCoord = NewInterCoord;
                OutOuterCoord = NewOuterCoord;
                return GetChessBoard(NewOuterCoord).GetComponent<ChessBoardController>().GetCheckerBoard(NewInterCoord);
            }
        }

        OutInterCoord = new Vector2Int();
        OutOuterCoord = new Vector2Int();
        return null;
    }

    // 检查坐标为OuterCoord的棋盘上坐标为InterCoord的棋盘格是否存在
    public bool CheckerBoardExist(Vector2Int InterCoord, Vector2Int OuterCoord)
    {
        if (GetCheckerBoard(InterCoord, OuterCoord) != null)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // 检查坐标为OuterCoord的棋盘上坐标为InterCoord的棋盘格是否被开启
    public bool CheckerBoardEnable(Vector2Int InterCoord, Vector2Int OuterCoord)
    {
        Vector2Int NewInterCoord = new Vector2Int();

        Vector2Int NewOuterCoord = new Vector2Int();

        if (GetCheckerBoard(InterCoord, OuterCoord, out NewInterCoord, out NewOuterCoord) != null)
        {
            if (GetChessBoard(NewOuterCoord).GetComponent<ChessBoardController>().GetCheckerBoardEnable(NewInterCoord) == true)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
}
