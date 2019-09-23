using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HintTargetStateController : MonoBehaviour
{
    // 当前 提示目标 的类型
    // HintType = 0 默认类型提示目标
    // HintType = 1 可移动类型提示目标
    // HintType = 2 可攻击类型提示目标
    public int HintType = 0;
    // 当前 提示目标 在棋盘中的位置
    public Vector3 ChessBoardInternalPosition = new Vector3();

    // 当前 提示目标 在棋盘中的坐标(内棋盘坐标)
    public Vector2Int ChessBoardInternalCoordinate = new Vector2Int();

    // 当前 提示目标 所在棋盘的坐标(外棋盘坐标)
    public Vector2Int ChessBoardOuterCoordinate = new Vector2Int();

    // 当前 提示目标 所在棋盘的索引
    public GameObject LocateChessBoard = null;

    // 设置当前 可操作目标 的内棋盘坐标和外棋盘坐标
    public void SetCoordinate(Vector2Int InterCoord, Vector3 InterPos, Vector2Int OuterCoord)
    {
        ChessBoardInternalCoordinate = InterCoord;
        ChessBoardOuterCoordinate = OuterCoord;

        ChessBoardInternalPosition = InterPos;
    }

    public void Initialize(int type)
    {
        HintType = type;

        if(HintType == 0)
        {

        }
        else if(HintType == 1)
        {
            this.GetComponent<MeshRenderer>().material = (Material)Resources.Load("Material/HintTargetMove");
        }
        else if(HintType == 2)
        {
            this.GetComponent<MeshRenderer>().material = (Material)Resources.Load("Material/HintTargetAttack");
        }
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
