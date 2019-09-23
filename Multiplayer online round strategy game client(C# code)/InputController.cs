using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class InputController : MonoBehaviour
{
    GameController MainGameController = null;

    InputStateManager MainInputStateManager = null;

    // 当前游戏客户端对应的玩家ID
    int CurrentPlayerID = -1;

    public void SetCurrentPlayerID(int NewCurrentPlayerID) { CurrentPlayerID = NewCurrentPlayerID; }

    public int GetCurrentPlayerID() { return CurrentPlayerID; }

    public void Initialize(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
        // 这里不知道为何一定要在这里new，在前面new就new不出来
        MainInputStateManager = new InputStateManager(MainGameController);       
    }

    public void EstablishReference()
    {
        MainInputStateManager.Initialize(CurrentPlayerID);
        MainInputStateManager.EstablishReference();
    }

    public InputStateManager GetInputStateManager()
    {
        return MainInputStateManager;
    }
     
    // Use this for initialization
    void Start ()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(MainInputStateManager != null)
        {
            if(MainInputStateManager.GetCurrentInputState() != null)
            {                
                MainInputStateManager.GetCurrentInputState().InputStateUpdate();
            }
        }        
    }
}
