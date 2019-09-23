using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MessageController : MonoBehaviour
{
    GameController MainGameController = null;

    Dictionary<int, GameObject> PlayerMessageAreaOfPlayer = new Dictionary<int, GameObject>();

    int PlayerNum = 0;

    public void Initialize(GameController ParaGameController)
    {
        MainGameController = ParaGameController;        
    }

    public void EstablishReference()
    {
        PlayerMessageAreaOfPlayer.Add(PlayerNum, GameObject.Find("Canvas/PlayerMessageArea"));
    }

    public void PlayerBeginRound(int PlayerID)
    {
        if(PlayerID == 0)
        {
            PlayerMessageAreaOfPlayer[PlayerID].GetComponent<PlayerMessageAreaController>().CurrentPlayerBeginRound();
        }
        else
        {
            PlayerMessageAreaOfPlayer[0].GetComponent<PlayerMessageAreaController>().OtherPlayerBeginRound(PlayerID);
        }
    }

    public void PlayerEndRound(int PlayerID)
    {
        if (PlayerID == 0)
        {
            PlayerMessageAreaOfPlayer[PlayerID].GetComponent<PlayerMessageAreaController>().CurrentPlayerEndRound();
        }
        else
        {
            PlayerMessageAreaOfPlayer[0].GetComponent<PlayerMessageAreaController>().OtherPlayerEndRound(PlayerID);
        }
    }

    public void PrivateMessage(string Message)
    {
        PlayerMessageAreaOfPlayer[0].GetComponent<PlayerMessageAreaController>().PrivateMessage(Message);
    }


	// Use this for initialization
	void Start () {

    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
