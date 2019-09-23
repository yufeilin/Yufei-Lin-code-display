using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PieceAnimationController : MonoBehaviour
{
    GameObject LocateChessBoard = null;

    Vector2Int InterCoord = new Vector2Int();

    // 执行棋子层级置顶动画标旗
    bool IsLiftingToTop = false;

    Vector3 TargetPosition = new Vector3();

    float LiftToTopInterval = 0.0f;

    float LiftHeight = 4.0f;

    float LiftTime = 20.0f;

    GameObject SecondLayerPiece = null;

    //============================================

    bool IsMoveToNewPosition = false;

    Vector3 MoveInterval = new Vector3();

    float MoveFrame = 10.0f;

    //============================================

    // 棋子被设置为当前棋盘格顶层棋子时播放的动画
    public void SetLiftToTop()
    {
        LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;
        InterCoord = this.GetComponent<PieceStateController>().ChessBoardInternalCoordinate;

        int PlayerID = this.GetComponent<PieceStateController>().GetOwner();

        int CheckerPieceNum = LocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePieceNum(InterCoord, PlayerID);

        if(CheckerPieceNum > 1)
        {
            SecondLayerPiece = LocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePiece(InterCoord, PlayerID, 1);
            SecondLayerPiece.GetComponentInChildren<PieceOperationController>().DisplayPiece();

            IsLiftingToTop = true;

            TargetPosition = this.transform.parent.position;

            Vector3 CurrentPosition = new Vector3(TargetPosition.x, TargetPosition.y + LiftHeight, TargetPosition.z);

            this.transform.parent.position = CurrentPosition;

            LiftToTopInterval = (TargetPosition.y - CurrentPosition.y) / LiftTime;
        }        
    }

    // 棋子部署时播放的动画
    public void DeployAnimationStart()
    {
        LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;
        InterCoord = this.GetComponent<PieceStateController>().ChessBoardInternalCoordinate;

        int PlayerID = this.GetComponent<PieceStateController>().GetOwner();

        int CheckerPieceNum = LocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePieceNum(InterCoord, PlayerID);

        if (CheckerPieceNum > 1)
        {
            SecondLayerPiece = LocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePiece(InterCoord, PlayerID, 1);
            SecondLayerPiece.GetComponentInChildren<PieceOperationController>().DisplayPiece();            
        }
        else
        {
            SecondLayerPiece = null;            
        }

        IsLiftingToTop = true;

        TargetPosition = this.transform.parent.position;

        Vector3 CurrentPosition = new Vector3(TargetPosition.x, TargetPosition.y + LiftHeight, TargetPosition.z);

        this.transform.parent.position = CurrentPosition;

        LiftToTopInterval = (TargetPosition.y - CurrentPosition.y) / LiftTime;

    }

    public void MoveAnimationStart(Vector3 NewPosition)
    {
        LocateChessBoard = this.GetComponent<PieceStateController>().LocateChessboard;
        InterCoord = this.GetComponent<PieceStateController>().ChessBoardInternalCoordinate;

        int PlayerID = this.GetComponent<PieceStateController>().GetOwner();

        int CheckerPieceNum = LocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePieceNum(InterCoord, PlayerID);

        if (CheckerPieceNum > 1)
        {
            SecondLayerPiece = LocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePiece(InterCoord, PlayerID, 1);
            SecondLayerPiece.GetComponentInChildren<PieceOperationController>().DisplayPiece();
        }
        else
        {
            SecondLayerPiece = null;
        }

        IsMoveToNewPosition = true;

        TargetPosition = NewPosition;

        MoveInterval = (TargetPosition - this.transform.parent.position) / MoveFrame;
    }

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
		if(IsLiftingToTop == true)
        {
            Vector3 NewPosition = new Vector3(this.transform.parent.position.x, this.transform.parent.position.y + LiftToTopInterval, this.transform.parent.position.z);

            this.transform.parent.position = NewPosition;

            if(Mathf.Abs(this.transform.parent.position.y - TargetPosition.y) <= 0.1f)
            {
                IsLiftingToTop = false;

                if(SecondLayerPiece != null)
                {
                    SecondLayerPiece.GetComponentInChildren<PieceOperationController>().HidePiece();
                }                
            }
        }

        if(IsMoveToNewPosition)
        {
            Vector3 NewPosition = this.transform.parent.position + MoveInterval;

            this.transform.parent.position = NewPosition;

            if(Vector3.Distance(this.transform.parent.position, TargetPosition) <= 0.1f)
            {
                IsMoveToNewPosition = false;

                if (SecondLayerPiece != null)
                {
                    SecondLayerPiece.GetComponentInChildren<PieceOperationController>().HidePiece();
                }
            }
        }
	}
}
