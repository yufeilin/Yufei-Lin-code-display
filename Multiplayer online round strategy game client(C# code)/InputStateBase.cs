using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class InputStateBase
{
    public static GameController MainGameController = null;

    // // 当前游戏客户端对应的玩家ID
    static int CurrentPlayerID = -1;

    // 当前鼠标悬停在的游戏有关物体
    static GameObject HoverGameObject = null;

    // 当前鼠标悬停在的GUI物体
    static GameObject HoverGUI = null;

    // 当前鼠标悬停在的手牌区域
    static GameObject HoverHandCardArea = null;

    static GameObject CurrentOperationPiece = null;

    static GameObject CurrentOperationHandCard = null;

    //public static GameObject GameManager = null;

    

    public static CardManager MainCardManager = null;

    public static DiplomacyManager MainDiplomacyManager = null;

    public static RoundManager MainRoundManager = null;

    public static CardDisplayController MainCardDisplayController = null;

    public static UIController MainUIController = null;

    // public static GameObject CardDisplayManager = null;

    public static GameObject EventSystem = null;

    public static GameObject Canvas = null;

    public static GameObject HandCardArea = null;

    //public static GameObject UIManager = null;
    

    //static int GUIRaycastObjectNum = 0;

    // 初始输入状态默认为Idle
    public static string InputStateID = "Idle";

    public static Ray DetectRay;
    public static RaycastHit Hit;

    // 输入状态基类构造函数
    public InputStateBase()
    {
        
    }

    public void SetGameController(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
    }

    public void Initialize(int PlayerID)
    {
        CurrentPlayerID = PlayerID;

        EventSystem = GameObject.Find("EventSystem");
        Canvas = GameObject.Find("Canvas");
        HandCardArea = GameObject.Find("Canvas/HandCardArea");
    }

    public void EstablishReference()
    {
        MainDiplomacyManager = MainGameController.GetDiplomacyManager();
        MainCardManager = MainGameController.GetCardManager();
        MainCardDisplayController = MainGameController.GetCardDisplayController();
        MainUIController = MainGameController.GetUIController();
        MainRoundManager = MainGameController.GetRoundManager();
    }

    public int GetCurrentPlayerID()
    {
        return CurrentPlayerID;
    }

    public int GetCurrentRoundPlayerID()
    {
        return MainRoundManager.GetCurrentRoundPlayerID();
    }

    // 判断当前回合是否是当前客户端对应玩家的回合
    public bool IsCurrentPlayerRound()
    {
        if(GetCurrentPlayerID() == GetCurrentRoundPlayerID())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public string GetCurrentInputState()
    {
        return InputStateID;
    }

    public void SetCurrentInputState(string NewInputStateID)
    {
        InputStateID = NewInputStateID;
    }

    public GameObject GetCurrentOperationPiece()
    {
        return CurrentOperationPiece;
    }

    public GameObject GetCurrentOperationHandCard()
    {
        return CurrentOperationHandCard;
    }

    // 做一次游戏场景射线检测并将鼠标射线碰撞的第一个游戏相关物体保存在HoverGameObject变量中
    // 这个函数基本上在所有的状态的更新函数中都要首先调用
    public void GetHoverGameObject()
    {
        DetectRay = Camera.main.ScreenPointToRay(Input.mousePosition);

        if (Physics.Raycast(DetectRay, out Hit))
        {
            if (Hit.collider.gameObject.GetComponent<GameObjectPropertyController>() != null)
            {
                //return Hit.collider.gameObject;
                HoverGameObject = Hit.collider.gameObject;
                return;
            }
        }

        HoverGameObject = null;

        //return null;
    }

    // 获取鼠标悬停的棋子物体
    public GameObject GetHoverPiece()
    {
        if(HoverGameObject == null)
        {
            return null;
        }

        if(HoverGameObject.GetComponent<GameObjectPropertyController>() != null && HoverGameObject.GetComponent<GameObjectPropertyController>().IsPiece == true)
        {
            return HoverGameObject;
        }
        else
        {
            return null;
        }
    }

    // 做一次GUI射线检测并将鼠标射线碰撞的第一个GUI物体保存在HoverGUI变量中
    // 这个函数基本上在所有的状态的更新函数中都要首先调用
    public void GetHoverGUI()
    {
        EventSystem MainEventSystem = EventSystem.GetComponent<EventSystem>();
        GraphicRaycaster MainGraphicRaycaster = Canvas.GetComponent<GraphicRaycaster>();

        PointerEventData eventData = new PointerEventData(MainEventSystem);
        eventData.pressPosition = Input.mousePosition;
        eventData.position = Input.mousePosition;

        List<RaycastResult> list = new List<RaycastResult>();
        MainGraphicRaycaster.Raycast(eventData, list);

        HoverGUI = null;
        HoverHandCardArea = null;

        for(int i=0;i< list.Count;i++)
        {
            if(list[i].gameObject.GetComponent<HandCardController>() != null)
            {
                HoverGUI = list[i].gameObject;
            }
            else if(list[i].gameObject.GetComponent<HandCardAreaController>() != null)
            {
                HoverHandCardArea = list[i].gameObject;
            }
        }

        //if (list.Count >= 1)
        //{
        //    HoverGUI = list[0].gameObject;
        //    return;
        //    //return list[0].gameObject;
        //}
        //
        //HoverGUI = null;
        //return null;
    }

    // 获取鼠标悬停的手牌物体
    public GameObject GetHoverHandCard()
    {
        if(HoverGUI == null)
        {
            return null;
        }

        if(HoverGUI.GetComponent<HandCardController>() != null)
        {
            return HoverGUI;
        }
        else
        {
            return null;
        }
    }

    public GameObject GetHoverHandCardArea()
    {
        return HoverHandCardArea;
    }

    public GameObject GetHoverOperatableTargetMove()
    {
        if (HoverGameObject != null && HoverGameObject.GetComponent<GameObjectPropertyController>() != null && HoverGameObject.GetComponent<GameObjectPropertyController>().IsMoveableCheckerBoard)
        {
            return HoverGameObject;
        }
        else
        {
            return null;
        }
    }

    public GameObject GetHoverCheckerBoard()
    {
        if (HoverGameObject!=null && HoverGameObject.GetComponent<GameObjectPropertyController>() != null && HoverGameObject.GetComponent<GameObjectPropertyController>().IsCheckerBoard)
        {
            return HoverGameObject;
        }
        else
        {
            return null;
        }
    }

    public GameObject GetHoverContainUnitIcon()
    {
        if(HoverGUI != null && HoverGUI.GetComponent<GameObjectPropertyController>() != null && HoverGUI.GetComponent<GameObjectPropertyController>().IsContainUnitIcon)
        {
            return HoverGUI;
        }
        else
        {
            return null;
        }
    }

    // 选择一个手牌物体作为当前操作手牌
    public void SelectCurrentOperationHandCard(GameObject NewCurrentOperationHandCard)
    {        
        CurrentOperationHandCard = NewCurrentOperationHandCard;
        HandCardArea.GetComponent<HandCardAreaController>().TakeOutAHandCard(CurrentOperationHandCard);
        // 进入“手牌操作”状态
        SetCurrentInputState("HandCardOperation");

        Debug.Log(GetCurrentInputState());
    }

    // 取消选择当前操作手牌
    public void DisselectCurrentOperationHandCard()
    {
        CurrentOperationHandCard = null;
        // 进入“闲置”状态
        SetCurrentInputState("Idle");
    }

    // 设置当前操作棋子
    public void SetCurrentOperationPiece(GameObject NewCurrentOperationPiece)
    {
        DisselectCurrentOperationPiece();

        SelectCurrentOperationPiece(NewCurrentOperationPiece);
    }

    // 专门为棋盘格棋子堆叠信息条使用的棋子切换方法
    public void SwitchCurrentOperationPiece(GameObject NewCurrentOperationPiece)
    {
        if (CurrentOperationPiece != null)
        {
            CurrentOperationPiece.GetComponentInChildren<PieceOperationController>().OperationPreviewOff();
            CurrentOperationPiece.GetComponentInChildren<PieceOperationController>().HintPreviewOff();
            CurrentOperationPiece.GetComponentInChildren<PieceOperationController>().AttackTargetOff();
            CurrentOperationPiece = null;
        }

        CurrentOperationPiece = NewCurrentOperationPiece;
        CurrentOperationPiece.GetComponentInChildren<PieceOperationController>().OperationPreview();
    }

    // 取消选择当前操作棋子
    public void DisselectCurrentOperationPiece()
    {
        if (CurrentOperationPiece != null)
        {

            Debug.Log("Piece Disselect " + CurrentOperationPiece.name);
            //HasSelectedPiece = false;
            CurrentOperationPiece.GetComponentInChildren<PieceOperationController>().OperationPreviewOff();
            CurrentOperationPiece.GetComponentInChildren<PieceOperationController>().HintPreviewOff();
            CurrentOperationPiece.GetComponentInChildren<PieceOperationController>().AttackTargetOff();
            CurrentOperationPiece = null;

            // 隐藏棋盘格包含单位条
            MainUIController.HideContainUnitBar();
            
        }
        // 进入“闲置”状态
        SetCurrentInputState("Idle");
    }

    // 选择一个棋子作为当前操作棋子
    public void SelectCurrentOperationPiece(GameObject NewCurrentOperationPiece)
    {
        DisselectCurrentOperationPiece();

        CurrentOperationPiece = NewCurrentOperationPiece;
        CurrentOperationPiece.GetComponentInChildren<PieceOperationController>().OperationPreview();

        // 刷新并显示棋盘格包含单位条
        MainUIController.RefreshContainUnitBar(CurrentOperationPiece);

        // 进入“棋子操作状态”
        SetCurrentInputState("PieceOperation");
    }

    public virtual void InputStateUpdate()
    {

    }
}

public class SelectInitialHandCard: InputStateBase
{
    public override void InputStateUpdate()
    {

    }


}

public class Idle:InputStateBase
{
    public override void InputStateUpdate()
    {
        //Debug.Log("Idle");
        // 游戏场景射线检测
        GetHoverGameObject();

        // GUI射线检测
        GetHoverGUI();

        // 因为在Idle状态下玩家只能点击棋子和手牌，悬停棋子和手牌，所以只检测棋子和手牌即可

        GameObject CurrentHoverHandCard = GetHoverHandCard();

        GameObject CurrentHoverPiece = GetHoverPiece();

        if(CurrentHoverHandCard != null)
        {
            //Debug.Log("233333");

            // 如果此时鼠标悬停在一个手牌物体上并且当前回合是当前客户端对应玩家的回合
            if (Input.GetMouseButtonDown(0) && IsCurrentPlayerRound())
            {
                Debug.Log("click handcard");
                MainCardDisplayController.MainDisplayCardDisplayOff();
                // 将点击的手牌作为当前操作手牌并进入“手牌操作”状态
                SelectCurrentOperationHandCard(CurrentHoverHandCard);

                if (GetCurrentInputState() == "HandCardOperation")
                {
                    Debug.Log("GetCurrentInputState() == HandCardOperation");
                }
            }
            else
            {
                //Debug.Log("233333");
                // 如果鼠标没有按下(悬停处理)
                // 展示该手牌对应的卡牌大图
                CurrentHoverHandCard.GetComponent<HandCardController>().GetCard().FetchInfoFromHandCard(MainCardDisplayController);
                MainCardDisplayController.MainDisplayCardDisplayOn();
            }
        }
        else if(CurrentHoverPiece != null)
        {
            Debug.Log(CurrentHoverPiece.GetComponent<PieceStateController>().GetOwner() == GetCurrentPlayerID());

            // 如果此时鼠标悬停在一个棋子物体上并且当前回合是当前客户端对应玩家的回合
            if (Input.GetMouseButtonDown(0) && IsCurrentPlayerRound() && CurrentHoverPiece.GetComponent<PieceStateController>().GetOwner() == GetCurrentPlayerID())
            {
                MainCardDisplayController.MainDisplayCardDisplayOff();
                // 如果此时鼠标左键按下(按下处理)
                // 将点击的棋子作为当前操作棋子并进入“棋子操作”状态
                SelectCurrentOperationPiece(CurrentHoverPiece);
            }
            else
            {
                // 如果鼠标没有按下(悬停处理)
                // 展示该棋子对应的卡牌大图
                MainCardDisplayController.LoadInfoFromPiece(CurrentHoverPiece);
                MainCardDisplayController.MainDisplayCardDisplayOn();
            }
        }
        else
        {
            MainCardDisplayController.MainDisplayCardDisplayOff();
        }

        // 游戏场景射线检测

        // UI层射线检测

        // 悬停处理

        // 鼠标左键按下处理

        //if (GetCurrentInputState() == "HandCardOperation")
        //{
        //    Debug.Log("GetCurrentInputState() == HandCardOperation");
        //}
    }
}


public class PieceOperation:InputStateBase
{
    public override void InputStateUpdate()
    {
        //Debug.Log("PieceOperation");
        // 游戏场景射线检测
        GetHoverGameObject();

        // GUI射线检测
        GetHoverGUI();

        GameObject CurrentHoverHandCard = GetHoverHandCard();

        GameObject CurrentHoverPiece = GetHoverPiece();

        GameObject CurrentHoverOperatableTargetMove = GetHoverOperatableTargetMove();

        GameObject CurrentHoverContainUnitIcon = GetHoverContainUnitIcon();

        if (CurrentHoverPiece != null)
        {
            //Debug.Log(CurrentHoverPiece.transform.parent.name);
        }



        if (CurrentHoverHandCard != null)
        {
            // 如果此时鼠标悬停在一个GUI物体上
            if (Input.GetMouseButtonDown(0))
            {
                // 如果此时鼠标悬停在一个手牌物体上
                if (Input.GetMouseButtonDown(0))
                {
                    // 如果在“棋子操作”状态下点击手牌则取消选择当前操作棋子并进入“手牌操作”状态
                    DisselectCurrentOperationPiece();

                    MainCardDisplayController.MainDisplayCardDisplayOff();
                    // 将点击的手牌作为当前操作手牌并进入“手牌操作”状态
                    SelectCurrentOperationHandCard(CurrentHoverHandCard);
                }
                else
                {
                    // 如果鼠标没有按下(悬停处理)
                    // 展示该手牌对应的卡牌大图
                    CurrentHoverHandCard.GetComponent<HandCardController>().GetCard().FetchInfoFromHandCard(MainCardDisplayController);
                    MainCardDisplayController.MainDisplayCardDisplayOn();
                }
            }
        }
        else if (CurrentHoverPiece != null)
        {
            // 如果此时鼠标悬停在一个棋子物体上
            if (Input.GetMouseButtonDown(0))
            {
                // 如果此时鼠标左键点击一个棋子(按下处理)
                //SelectCurrentOperationPiece(CurrentHoverPiece);

                // 如果点击棋子是敌对并且可以攻击则进行攻击结算并返回状态idle
                if(MainDiplomacyManager.GetDiplomacy(GetCurrentPlayerID(), CurrentHoverPiece.GetComponent<PieceStateController>().GetOwner()) == 0)
                {
                    Debug.Log("Attack a piece");
                    // 并且这个棋子在当前操作棋子的攻击范围内
                    if (CurrentHoverPiece.GetComponent<PieceStateController>().IsAttackTarget == true)
                    {
                        GetCurrentOperationPiece().GetComponent<PieceOperationController>().AttackTarget(CurrentHoverPiece.transform.parent.gameObject);
                    }
                       
                    DisselectCurrentOperationPiece();
                }

                // 如果点击棋子是自己的则切换当前操作棋子，状态仍然留在“棋子操作状态”
                if ( GetCurrentPlayerID() == CurrentHoverPiece.GetComponent<PieceStateController>().GetOwner() )
                {
                    SelectCurrentOperationPiece(CurrentHoverPiece.transform.parent.gameObject);
                }
                
            }
            else
            {
                // 如果鼠标没有按下(悬停处理)
                // 展示该棋子对应的卡牌大图
                MainCardDisplayController.LoadInfoFromPiece(CurrentHoverPiece);
                MainCardDisplayController.MainDisplayCardDisplayOn();
            }
        }
        else if (CurrentHoverOperatableTargetMove != null)
        {
            // 如果此时鼠标悬停在一个可移动位置上
            if (Input.GetMouseButtonDown(0))
            {
                // 其实这里要发送一个移动请求给服务器

                // 如果此时鼠标左键点击该可移动位置则调用棋子的位移函数移动棋子
                GetCurrentOperationPiece().GetComponentInChildren<PieceOperationController>().MoveToTareget(CurrentHoverOperatableTargetMove);

                DisselectCurrentOperationPiece();
            }               
        }
        else if(CurrentHoverContainUnitIcon != null)
        {

        }
        else
        {

            
            // 如果鼠标悬停在其他什么物体或者没有悬停在什么物体上
            if (Input.GetMouseButtonDown(0))
            {
                Debug.Log("Piece selesct canceled.");

                // 单击左键取消当前单位选择
                DisselectCurrentOperationPiece();
            }

            MainCardDisplayController.MainDisplayCardDisplayOff();
        }
    }
}


public class HandCardOperation:InputStateBase
{
    public override void InputStateUpdate()
    {
        //Debug.Log("HandCardOperation");
        // 游戏场景射线检测
        GetHoverGameObject();

        // GUI射线检测
        GetHoverGUI();

        GameObject CurrentHoverCheckerBoard = GetHoverCheckerBoard();

        GameObject CurrentHoverPiece = GetHoverPiece();

        GameObject CurrentHoverHandCardArea = GetHoverHandCardArea();

        if (Input.GetMouseButtonUp(0))
        {
            //Debug.Log("mouseUp");
            // 如果鼠标左键被抬起
            if (GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard().HandCardSpell() == true)
            {
                // 如果当前操作手牌施放成功则更新手牌区

                // 当本地卡牌施放判断成功时向游戏控制器告知卡牌使用
                MainCardManager.PlyerCastACard(GetCurrentPlayerID(), GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard());                

                //HandCardArea.GetComponent<HandCardAreaController>().SpellAHandCard(GetCurrentOperationHandCard());
            }
            else
            {
                // 如果当前操作手牌施放失败则显示手牌UI同时将手牌移回手牌区
                HandCardArea.GetComponent<HandCardAreaController>().RestoreAHandCard(GetCurrentOperationHandCard());
                GetCurrentOperationHandCard().GetComponent<HandCardController>().HandCardDisplayOn();

                // 隐藏手牌UI对应卡牌的瞄准模式
                GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard().TargetPreviewOff();
            }

            DisselectCurrentOperationHandCard();
        }
        else
        {
            // 如果鼠标左键没有被抬起(即用户一直按着鼠标左键)
            GetCurrentOperationHandCard().transform.position = Input.mousePosition;
            
            if(CurrentHoverHandCardArea != null)
            {
                // 当玩家按住鼠标左键拖动卡牌并且卡牌仍处在手牌区中时显示手牌UI并隐藏卡牌瞄准模式

                // 显示手牌UI
                GetCurrentOperationHandCard().GetComponent<HandCardController>().HandCardDisplayOn();

                // 隐藏手牌UI对应卡牌的瞄准模式
                GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard().TargetPreviewOff();

                //Debug.Log("mouse in hand card area.");
            }
            else
            {
                // 当玩家按住鼠标左键拖动卡牌并且卡牌不处在手牌区中时隐藏手牌UI并显示卡牌瞄准模式

                if (GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard().HandCardcCostCheck())
                {
                    // 如果玩家当前资源足以施放该卡牌则允许将手牌拖动到棋盘上进行释放目标选择

                    // 这里应该是3个分支
                    // 1.如果拖动到棋盘格上
                    // 2.如果拖动到棋子上
                    // 3.如果拖动到其他物体上
                    if (CurrentHoverCheckerBoard != null)
                    {
                        // 如果鼠标移动到某个棋盘格上
                        // 隐藏手牌UI
                        GetCurrentOperationHandCard().GetComponent<HandCardController>().HandCardDisplayOff();
                        // 显示手牌UI对应卡牌的瞄准模式
                        GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard().TargetPreviewOn(CurrentHoverCheckerBoard);
                    }
                    else if (CurrentHoverPiece != null)
                    {
                        // 如果鼠标移动到某个棋子上
                        // 隐藏手牌UI
                        GetCurrentOperationHandCard().GetComponent<HandCardController>().HandCardDisplayOff();

                        // 获取棋子所在的棋盘格
                        PieceStateController TmpPieceStateController = CurrentHoverPiece.GetComponentInChildren<PieceStateController>();
                        CurrentHoverCheckerBoard = TmpPieceStateController.LocateChessboard.GetComponent<ChessBoardController>().GetCheckerBoard(TmpPieceStateController.ChessBoardInternalCoordinate);

                        //Debug.Log(CurrentHoverCheckerBoard.name);

                        // 显示手牌UI对应卡牌的瞄准模式
                        GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard().TargetPreviewOn(CurrentHoverCheckerBoard);
                    }
                    else
                    {
                        // 如果鼠标并没有位于某个棋盘格上
                        // 显示手牌UI
                        GetCurrentOperationHandCard().GetComponent<HandCardController>().HandCardDisplayOn();
                        // 隐藏手牌UI对应卡牌的瞄准模式
                        GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard().TargetPreviewOff();
                    }
                }
                else
                {
                    // 如果玩家当前资源不足以施放该卡牌则将卡牌拉回手牌区
                    HandCardArea.GetComponent<HandCardAreaController>().RestoreAHandCard(GetCurrentOperationHandCard());
                    GetCurrentOperationHandCard().GetComponent<HandCardController>().HandCardDisplayOn();

                    // 隐藏手牌UI对应卡牌的瞄准模式
                    GetCurrentOperationHandCard().GetComponent<HandCardController>().GetCard().TargetPreviewOff();

                    DisselectCurrentOperationHandCard();
                }
            }

            

            
        }
    }
}