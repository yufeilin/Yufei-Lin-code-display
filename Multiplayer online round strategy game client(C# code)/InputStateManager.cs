using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InputStateManager
{
    GameController MainGameController = null;

    // 当前游戏客户端对应的玩家ID
    int CurrentPlayerID = -1;

    InputStateBase MainInputStateBase = null;

    Dictionary<string, InputStateBase> InputStateOfName = new Dictionary<string, InputStateBase>();

    public InputStateManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
        // 不能再构造函数中call find函数
        //MainInputStateBase.Initialize();
        MainInputStateBase = new InputStateBase();
        MainInputStateBase.SetGameController(MainGameController);

        InputStateOfName.Add("SelectInitialHandCard", new SelectInitialHandCard());
        InputStateOfName.Add("Idle", new Idle());
        InputStateOfName.Add("PieceOperation", new PieceOperation());
        InputStateOfName.Add("HandCardOperation", new HandCardOperation());
    }

    public void Initialize(int PlayerID)
    {
        CurrentPlayerID = PlayerID;        
    }

    public void EstablishReference()
    {
        MainInputStateBase.Initialize(CurrentPlayerID);
        MainInputStateBase.EstablishReference();
    }

    // 获取当前的输入状态实例
    public InputStateBase GetCurrentInputState()
    {
        return InputStateOfName[MainInputStateBase.GetCurrentInputState()];
    }

    public GameObject GetCurrentOperationPiece()
    {
        return MainInputStateBase.GetCurrentOperationPiece();
    }

    public void SetCurrentOperationPiece(GameObject NewCurrentOperationPiece)
    {
        MainInputStateBase.SwitchCurrentOperationPiece(NewCurrentOperationPiece);
    }

    public void SetCurrentInputState(string NewInputStateID)
    {
        MainInputStateBase.SetCurrentInputState(NewInputStateID);
    }
}
