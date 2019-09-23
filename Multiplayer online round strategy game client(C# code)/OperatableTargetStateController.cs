using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OperatableTargetStateController : MonoBehaviour
{
    // 当前 可操作目标 在棋盘中的位置
    public Vector3 ChessBoardInternalPosition = new Vector3();

    // 当前 可操作目标 在棋盘中的坐标(内棋盘坐标)
    public Vector2Int ChessBoardInternalCoordinate = new Vector2Int();

    // 当前 可操作目标 所在棋盘的坐标(外棋盘坐标)
    public Vector2Int ChessBoardOuterCoordinate = new Vector2Int();

    // 当前 可操作目标 所在棋盘的中心点位置
    public float ChessBoardOuterPositionX = 0.0f;
    public float ChessBoardOuterPositionZ = 0.0f;

    //GameObject ChessBoardManager = null;

    // 棋子所在棋盘的单个棋盘格边长
    //float CheckerBoardSize = 1.0f;

    // 棋子所在棋盘的所具有的行数或者列数
    //int ChessBoardSize = 8;

    // 棋子所在棋盘的坐标(0,0)棋盘格的中心点位置
    //Vector3 InitPosition = new Vector3();

    public GameObject LocateChessBoard = null;

    // 设置当前 可操作目标 的内棋盘坐标和外棋盘坐标
    public void SetCoordinate(Vector2Int InterCoord, Vector3 InterPos, Vector2Int OuterCoord)
    {
        ChessBoardInternalCoordinate = InterCoord;
        ChessBoardOuterCoordinate = OuterCoord;

        ChessBoardInternalPosition = InterPos;
    }

    // Use this for initialization
    void Start ()
    {
        //ChessBoardManager = GameObject.Find("ChessBoardManager");

        // 从棋盘管理器处获取棋盘尺寸和棋盘格尺寸
        //CheckerBoardSize = ChessBoardManager.GetComponent<ChessBoardGenerator>().CheckerBoardSize;
        //ChessBoardSize = ChessBoardManager.GetComponent<ChessBoardGenerator>().ChessBoardSize;
    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}
}
