using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PieceController : MonoBehaviour
{

    // 棋子标识符
    string PieceID = "";

    // 棋子名称
    string PieceName = "";

    // 棋子晶体矿花费
    int PieceCostMineral = 0;

    // 棋子瓦斯花费
    int PieceCostGas = 0;

    // 定义棋子种类枚举类型
    // King = 国王
    // Queen = 王后
    // Rook = 车
    // Bishop = 主教
    // Knight = 骑士
    // Pawn = 禁卫军
    public enum PieceType
    {
        King,
        Queen,
        Bishop,
        Knight,
        Rook,
        Pawn,
        Default
    }

    // 当前棋子所属的种类
    public PieceType Type;

    // 该棋子是否具有朝向
    public bool HasOrientation = false;

    // 棋子在可移动判定的时候能否穿越己方或者敌方单位
    public bool CanJump = false;

    // 常规移动模式能力数组
    // BaseMoveAbility[0] = 2 棋子能够向x轴正向+z轴正向(+1,+1)方向移动最大2步
    // BaseMoveAbility[1] = 2 棋子能够向x轴正向(+1,0)方向移动最大2步
    public int[] BaseMoveAbility = new int[8];

    // 常规攻击模式能力数组
    // BaseMoveAbility[0] = 2 棋子能够攻击沿x轴正向+z轴正向(+1,+1)方向最大2步内的敌方棋子
    // BaseMoveAbility[1] = 2 棋子能够攻击沿x轴正向(+1,0)方向最大2步内的敌方棋子
    public int[] BaseAttackAbility = new int[8];

    // 协助攻击标旗
    // 当棋子所在的当前棋盘格中有单位进攻时协同该单位一同进攻
    public bool AssistAttack = false;

    // 协助防御标旗
    // 当棋子所在的当前棋盘格中有单位防御时协同该单位一同防御
    public bool AssistDefend = false;

    // 获取棋子联合进攻标旗
    public bool GetAssistAttackFlag() { return AssistAttack; }

    // 获取棋子协助防守标旗
    public bool GetAssistDefendFlag() { return AssistDefend; }

    public void SetPieceID(string NewPieceID) { PieceID = NewPieceID; }

    public void SetPieceName(string NewPieceName) { PieceName = NewPieceName; }

    public string GetPieceID() { return PieceID; }

    public string GetPieceName() { return PieceName; }

    public int GetPieceCostMineral() { return PieceCostMineral; }

    public int GetPieceCostGas() { return PieceCostGas; }

    public void SetPieceCostMineral(int NewPieceCostMineral) { PieceCostMineral = NewPieceCostMineral; }

    public void SetPieceCostGas(int NewPieceCostGas) { PieceCostGas = NewPieceCostGas; }

    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
