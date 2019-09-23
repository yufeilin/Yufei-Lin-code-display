using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameLoadController : MonoBehaviour
{
    public GameObject GameManager = null;

    public GameObject WorldManager = null;

    public GameObject ResourceUI = null;

    public void LoadTestGame1()
    {
        //GameController MainGameController = GameManager.GetComponent<GameController>();
        //
        ////ChessBoard = GameManager.GetComponent<GameController>().WorldManager.GetComponent<WorldOperationController>().CreateChessboard(new Vector2(0, 0));
        //GameManager.GetComponent<GameController>().GetWorldManager().CreateChessboard(new Vector2Int(0, 0));
        //GameManager.GetComponent<GameController>().GetWorldManager().CreateChessboard(new Vector2Int(1, 0));
        //GameManager.GetComponent<GameController>().GetWorldManager().CreateChessboard(new Vector2Int(0, 1));
        //GameManager.GetComponent<GameController>().GetWorldManager().CreateChessboard(new Vector2Int(1, 1));
        //
        //for (int i=0;i<8;i++)
        //{
        //    GameObject NewPiece = GameManager.GetComponent<GameController>().GetWorldManager().CreatePieceOnChessboard("BanelingCommando", new Vector2Int(i, 0), new Vector2Int(0, 0));
        //
        //    if(i==6)
        //    {
        //        NewPiece.GetComponentInChildren<PieceStateController>().SetOwner(1);
        //        //NewPiece.GetComponentInChildren<PieceStateController>().Owner = 1;
        //    }
        //
        //    GameManager.GetComponent<GameController>().GetWorldManager().CreatePieceOnChessboard("LogisticsWarehouse", new Vector2Int(i, 1), new Vector2Int(0, 0));
        //}
        //
        //// 给玩家0初始化一个测试用卡组
        //List<string> Player00Deck = new List<string>();
        //
        //Player00Deck.Add("BanelingCommando");
        //Player00Deck.Add("RangerMarineSquad");
        //Player00Deck.Add("DominionSecurityForcesMarineSquad");
        //Player00Deck.Add("Raynor");
        //Player00Deck.Add("ArmoredTransportSquad");
        //Player00Deck.Add("CivilianRebelGroup");
        //
        //MainGameController.GetCardManager().AddPlayer(0);
        //MainGameController.GetCardManager().AddPlayer(1);
        //MainGameController.GetCardManager().InitialPlayerDeck(0, Player00Deck);
        //
        //
        //// 给玩家0和玩家1初始化资源
        //MainGameController.GetResourceManager().PlayerInitialize(0, 500, 400);
        //MainGameController.GetResourceManager().PlayerInitialize(1, 500, 400);
        //
        //// 初始化玩家回合序列
        //MainGameController.GetRoundManager().InsertPlayerRound(0);
        //MainGameController.GetRoundManager().InsertPlayerRound(1);
        //// 玩家0首先开始回合
        //MainGameController.GetRoundManager().GetNextRoundPlayerID();
        //MainGameController.GetRoundManager().PlayerBeginRound(0);

        // TODO: 启动所有玩家的所有UI以及UI更新
        // 这里只启动了玩家0的资源UI
        ResourceUI.GetComponent<PlayerResourceAreaController>().DisplayResourceUI();

    }

	// Use this for initialization
	void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (Input.GetKeyDown(KeyCode.Q))
        {
            LoadTestGame1();
        }
    }
}
