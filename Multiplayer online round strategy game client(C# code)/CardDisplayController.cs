using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CardDisplayController : MonoBehaviour
{
    GameController MainGameController = null;

    GameObject MainDisplayCard = null;

    bool MainDisplayCardDisplay = false;

    void InitializeGameObjectReference()
    {
        if (MainDisplayCard == null){ MainDisplayCard = GameObject.Find("Canvas/MainDisplayCard");}
    }

    public void Initialize(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
    }

    public void EstablishReference()
    {
        MainDisplayCard = GameObject.Find("Canvas/MainDisplayCard");
    }

    // 显示展示卡牌
    public void MainDisplayCardDisplayOn()
    {
        MainDisplayCard.SetActive(true);
        MainDisplayCardDisplay = true;
    }

    // 隐藏展示卡牌
    public void MainDisplayCardDisplayOff()
    {
        MainDisplayCard.SetActive(false);
        MainDisplayCardDisplay = false;
    }

    // 将一个棋子当前的状态信息装入展示卡牌当中
    public void LoadInfoFromPiece(GameObject ParaPiece)
    {
        MainDisplayCard.GetComponent<DisplayCardController>().LoadInfoFromPiece(ParaPiece);
    }

    // 将一个法术实例当前的状态信息装入展示卡牌当中
    public void LoadInfoFromSpellInstance(SpellInstance ParaSpellInstance)
    {
        MainDisplayCard.GetComponent<DisplayCardController>().LoadInfoFromSpellInstance(ParaSpellInstance);
    }

    // Use this for initialization
    void Start ()
    {

        //InitializeGameObjectReference();

        // 初始隐藏展示卡牌
        //MainDisplayCardDisplayOff();
    }
	
	// Update is called once per frame
	void Update ()
    {
		if(MainDisplayCard == null)
        {
            MainDisplayCard = GameObject.Find("Canvas/MainDisplayCard");
        }
	}
}
