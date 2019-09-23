using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckerBoardInfoController : MonoBehaviour {

    public Vector3 CheckerBoardInternalPosition = new Vector3();

    public Vector2Int CheckerBoardInternalCoordinate = new Vector2Int();

    public Vector2Int CheckerBoardOuterCoordinate = new Vector2Int();

    // 当前棋子所在的棋盘的引用
    public GameObject LocateChessboard = null;

    public void SetCoordinate(Vector2Int InterCoord, Vector3 InterPos, Vector2Int OuterCoord)
    {
        CheckerBoardInternalCoordinate = InterCoord;
        CheckerBoardOuterCoordinate = OuterCoord;

        CheckerBoardInternalPosition = InterPos;
    }

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
