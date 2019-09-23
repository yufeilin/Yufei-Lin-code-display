using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using UnityEngine.Events;

public class InitialHandCardManager
{
    GameController MainGameController = null;

    CardManager MainCardManager = null;

    InputController MainInputController = null;

    List<Card> InitialCards = new List<Card>();

    List<bool> CardChangeState = new List<bool>();

    List<GameObject> ChangeCardIcon = new List<GameObject>();

    int PlayerID = -1;

    int ChangeCardNum = 0;

    GameObject InitialHandCardSelectorPrefab = null;
    GameObject ChangeCardIconPrefab = null;
    GameObject ConfirmChangeCardButtonPrefab = null;
    GameObject ReadyToStartGameButtonPrefab = null;

    GameObject CurrentConfirmChangeCardButton = null;
    GameObject CurrentReadyToStartGameButton = null;

    public InitialHandCardManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
        PlayerID = MainGameController.GetPlayerID();
        
    }

    public void EstablishReference()
    {
        MainCardManager = MainGameController.GetCardManager();
        MainInputController = MainGameController.GetInputController();
    }

    public void GenerateInitialHandCard(int InitialHandCard)
    {
        MainInputController.GetInputStateManager().SetCurrentInputState("SelectInitialHandCard");

        InitialHandCardSelectorPrefab = (GameObject)Resources.Load("Prefabs/InitialHandCardSelector");

        ChangeCardIconPrefab = (GameObject)Resources.Load("Prefabs/ChangeCardIcon");

        for (int i = 0; i < 3; i++)
        {
            Card InitialCard = MainCardManager.CreateARandomCard(PlayerID);
            InitialCard.GetMainHandCardBase().GetComponent<RectTransform>().localPosition = new Vector3(-135 + 135 * i, 0, 0);
            InitialCards.Add(InitialCard);

            GameObject CurrentInitialHandCardSelector = GameObject.Instantiate(InitialHandCardSelectorPrefab);
            CurrentInitialHandCardSelector.GetComponent<RectTransform>().SetParent(GameObject.Find("Canvas").transform);
            CurrentInitialHandCardSelector.GetComponent<RectTransform>().localPosition = new Vector3(-135 + 135 * i, 0, 0);


            CardChangeState.Add(false);

            GameObject CurrentChangeCardIcon = GameObject.Instantiate(ChangeCardIconPrefab);
            CurrentChangeCardIcon.GetComponent<RectTransform>().SetParent(GameObject.Find("Canvas").transform);
            CurrentChangeCardIcon.GetComponent<RectTransform>().localPosition = new Vector3(-135 + 135 * i, 0, 0);
            ChangeCardIcon.Add(CurrentChangeCardIcon);
            CurrentChangeCardIcon.SetActive(false);

            EventTrigger trigger = CurrentInitialHandCardSelector.GetComponent<EventTrigger>();
            trigger.triggers = new List<EventTrigger.Entry>();
            EventTrigger.Entry enter = new EventTrigger.Entry();
            //3.指定事件触发的类型
            enter.eventID = EventTriggerType.PointerClick;
            //4.指定事件触发的方法
            enter.callback = new EventTrigger.TriggerEvent();

            int p = i;

            enter.callback.AddListener(delegate {
                SelectChangeCard(p);
            });

            trigger.triggers.Add(enter);
        }

        ConfirmChangeCardButtonPrefab = (GameObject)Resources.Load("Prefabs/ConfirmChangeCardButton");
        ReadyToStartGameButtonPrefab = (GameObject)Resources.Load("Prefabs/ReadyToStartGameButton");

        CurrentConfirmChangeCardButton = GameObject.Instantiate(ConfirmChangeCardButtonPrefab);
        CurrentConfirmChangeCardButton.GetComponent<RectTransform>().SetParent(GameObject.Find("Canvas").transform);
        CurrentConfirmChangeCardButton.GetComponent<RectTransform>().localPosition = new Vector3(0, -140, 0);
        CurrentConfirmChangeCardButton.GetComponent<Button>().onClick.AddListener(ConfirmChangeHandCard);
        CurrentConfirmChangeCardButton.SetActive(false);

        CurrentReadyToStartGameButton = GameObject.Instantiate(ReadyToStartGameButtonPrefab);
        CurrentReadyToStartGameButton.GetComponent<RectTransform>().SetParent(GameObject.Find("Canvas").transform);
        CurrentReadyToStartGameButton.GetComponent<RectTransform>().localPosition = new Vector3(0, -140, 0);
        CurrentReadyToStartGameButton.GetComponent<Button>().onClick.AddListener(ReadyToStartGame);
        //CurrentReadyToStartGameButton.SetActive(false);
    }

    public void SelectChangeCard(int CardIndex)
    {
        Debug.Log(CardIndex);

        if(CardChangeState[CardIndex])
        {
            ChangeCardNum--;
            ChangeCardIcon[CardIndex].SetActive(false);
            CardChangeState[CardIndex] = false;
        }
        else
        {
            ChangeCardNum++;
            ChangeCardIcon[CardIndex].SetActive(true);
            CardChangeState[CardIndex] = true;
        }

        if(ChangeCardNum>0)
        {
            CurrentConfirmChangeCardButton.SetActive(true);
            CurrentReadyToStartGameButton.SetActive(false);
        }
        else
        {
            CurrentConfirmChangeCardButton.SetActive(false);
            CurrentReadyToStartGameButton.SetActive(true);
        }
    }

    public void ConfirmChangeHandCard()
    {
        Debug.Log("Confirm Change");
    }

    public void ReadyToStartGame()
    {
        Debug.Log("Ready to game");

        HandCardAreaController MainHandCardAreaController = GameObject.Find("HandCardArea").GetComponent<HandCardAreaController>();

        for(int i=0;i< InitialCards.Count;i++)
        {
            MainHandCardAreaController.PlayerGetAInitialHandCard(InitialCards[i]);
        }

        CurrentConfirmChangeCardButton.SetActive(false);
        CurrentReadyToStartGameButton.SetActive(false);

        GameObject BlackLayer = GameObject.Find("Canvas/MidLayer/BlackLayer");
        BlackLayer.GetComponent<BlackLayerController>().SetTurnBright(200);

        MainInputController.GetInputStateManager().SetCurrentInputState("Idle");
    }

    //public 
}
