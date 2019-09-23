using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.Events;

public class UIController : MonoBehaviour
{
    GameController MainGameController = null;

    GameObject MainCanvas = null;
    //public GameObject InputManager = null;
    InputController ThisInputController = null;

    GameObject CheckerBoardContainUnitBarBasePrefab = null;
    GameObject CheckerBoardContainUnitBarBase = null;

    GameObject ContainUnitIconBasePrefab = null;
    GameObject ContainUnitIconBase = null;

    GameObject MidLayer = null;

    GameObject BlackLayerPrefab = null;
    GameObject BlackLayer = null;

    List<GameObject> ContainUnitIcanArray = new List<GameObject>();

    int[] LayerOfUnitIcon = new int[10];

    //int i = 0;

    public bool click = false;

    float FirstPosition = 0;

    int CurrentPlayerID = 0;

    public void Initialize(GameController ParaGameController)
    {
        MainGameController = ParaGameController;
    }

    public void EstablishReference()
    {
        for (int i = 0; i < 10; i++)
        {
            LayerOfUnitIcon[i] = i;
        }

        // 建立画布引用
        MainCanvas = GameObject.Find("Canvas");

        // 建立主输入控制器引用
        ThisInputController = MainGameController.GetInputController();

        // 初始化棋盘格包含单位信息条
        //==============================================================================================================================================
        CheckerBoardContainUnitBarBasePrefab = (GameObject)Resources.Load("Prefabs/CheckerBoardContainUnitBarBase");
        CheckerBoardContainUnitBarBase = GameObject.Instantiate(CheckerBoardContainUnitBarBasePrefab);

        CheckerBoardContainUnitBarBase.transform.SetParent(GameObject.Find("Canvas").gameObject.transform);

        float CanvasPositionX = MainCanvas.GetComponent<RectTransform>().position.x;
        float ContainBarBaseWidth = CheckerBoardContainUnitBarBase.GetComponent<RectTransform>().sizeDelta.x;

        // 这里的x需要根据canvas的尺寸来调整
        CheckerBoardContainUnitBarBase.GetComponent<RectTransform>().localPosition = new Vector3(CanvasPositionX - (ContainBarBaseWidth / 2), 40, 0);
        //==============================================================================================================================================


        ContainUnitIconBasePrefab = (GameObject)Resources.Load("Prefabs/ContainUnitIconBase");
        //ContainUnitIconBase = GameObject.Instantiate(ContainUnitIconBasePrefab);
        //
        //ContainUnitIconBase.transform.SetParent(CheckerBoardContainUnitBarBase.transform);
        //
        //ContainUnitIconBase.GetComponent<RectTransform>().localPosition = new Vector3(0, 0, 0);

        //Debug.Log();

        BlackLayerPrefab = (GameObject)Resources.Load("Prefabs/BlackLayer");
    }

    public void LoadUI()
    {
        MidLayer = MainCanvas.transform.Find("MidLayer").gameObject;
        MidLayer.GetComponent<RectTransform>().sizeDelta = new Vector2(Screen.width+6, Screen.height+6);

        BlackLayer = GameObject.Instantiate(BlackLayerPrefab);
        BlackLayer.name = "BlackLayer";
        BlackLayer.transform.SetParent(MainCanvas.transform.Find("MidLayer"));
        BlackLayer.GetComponent<RectTransform>().offsetMax = new Vector2(0, 0);
        BlackLayer.GetComponent<RectTransform>().offsetMin = new Vector2(0, 0);



    }

    // 将Vector2转换为Vector2Int
    public Vector2Int Vector2ToVector2Int(Vector2 ParaVector2)
    {
        return new Vector2Int((int)ParaVector2.x, (int)ParaVector2.y);
    }

    int CreateInt(int Index)
    {
        int res = Index;
        return res;
    }

    // 显示并刷新参数棋子所在棋盘格对应的棋盘格包含单位信息条
    public void RefreshContainUnitBar(GameObject SelectPiece)
    {
        PieceStateController SelectPieceStateController = SelectPiece.GetComponent<PieceStateController>();

        if(SelectPieceStateController != null)
        {
            // 判断如果该棋子所在的棋盘格上有一个以上的棋子则显示并刷新棋盘格包含单位信息条
            GameObject SelectLocateChessBoard = SelectPieceStateController.LocateChessboard;
            int SelectCheckerPieceNum = SelectLocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePieceNum(SelectPieceStateController.ChessBoardInternalCoordinate, CurrentPlayerID);
            if (SelectCheckerPieceNum  >= 2)
            {
                for (int i = 0; i < 10; i++)
                {
                    LayerOfUnitIcon[i] = i;
                }

                // 显示棋盘格内包含单位侧边栏
                CheckerBoardContainUnitBarBase.SetActive(true);

                // 根据该棋盘格内包含的单位调整侧边栏高度
                CheckerBoardContainUnitBarBase.GetComponent<RectTransform>().sizeDelta = new Vector2(100, 10 + 90 * SelectCheckerPieceNum);

                FirstPosition = 0;
                
                if(SelectCheckerPieceNum % 2 == 0)
                {
                    FirstPosition = ((SelectCheckerPieceNum / 2) - 1) * 90 + 45;
                }
                else
                {
                    FirstPosition = (SelectCheckerPieceNum / 2) * 90;
                }

                for(int i=0;i< SelectCheckerPieceNum;i++)
                {
                    ContainUnitIconBase = GameObject.Instantiate(ContainUnitIconBasePrefab);

                    ContainUnitIconBase.GetComponent<ContainUnitIconController>().SetPieceEntity(SelectLocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePiece(Vector2ToVector2Int(SelectPieceStateController.ChessBoardInternalCoordinate), CurrentPlayerID, i));



                    ContainUnitIcanArray.Add(ContainUnitIconBase);

                    ContainUnitIconBase.transform.SetParent(CheckerBoardContainUnitBarBase.transform);

                    ContainUnitIconBase.GetComponent<RectTransform>().localPosition = new Vector3(0, FirstPosition - (i) * 90, 0);

                    ContainUnitIconBase.name = "ContainUnitIconBase.name" + i;

                    EventTrigger trigger = ContainUnitIconBase.GetComponent<EventTrigger>();

                    trigger.triggers = new List<EventTrigger.Entry>();

                    EventTrigger.Entry enter = new EventTrigger.Entry();

                    //3.指定事件触发的类型
                    enter.eventID = EventTriggerType.PointerClick;
                    //4.指定事件触发的方法
                    enter.callback = new EventTrigger.TriggerEvent();

                    //Debug.Log("#######################" + i);

                    int p = i;

                    enter.callback.AddListener(delegate {
                        ClickUnitIconBase(LayerOfUnitIcon[p]);
                    });

                    trigger.triggers.Add(enter);
                }
            }
        }
        else
        {
            Debug.Log("PieceStateController reference fail");
        }
    }

    // 隐藏当前显示的棋盘格包含单位信息条
    public void HideContainUnitBar()
    {
        while(ContainUnitIcanArray.Count > 0)
        {
            Destroy(ContainUnitIcanArray[0]);
            ContainUnitIcanArray[0] = null;
            ContainUnitIcanArray.Remove(ContainUnitIcanArray[0]);
        }

        CheckerBoardContainUnitBarBase.SetActive(false);
    }

    private void PEnter(BaseEventData pd)
    {
        Debug.Log("Penter");
    }

    public void ClickUnitIconBase(int ID)
    {
        click = true;
        Debug.Log(ID);
        //i++;

        // 棋盘格包含单位信息条单位图标动画播放
        // 棋盘格包含单位信息条单位图标上下顺序调整
        for (int i=0;i< ContainUnitIcanArray.Count;i++)
        {
            if(i==ID)
            {
                ContainUnitIcanArray[i].GetComponent<ContainUnitIconController>().SetMoveUpward(new Vector3(0, FirstPosition, 0));
            }
            else if(i<ID)
            {
                ContainUnitIcanArray[i].GetComponent<ContainUnitIconController>().SetMoveDownward(new Vector3(0, FirstPosition - (i+1) * 90, 0));
            }
        }

        for(int i=0;i<10;i++)
        {
            if(LayerOfUnitIcon[i] == ID)
            {
                LayerOfUnitIcon[i] = 0;
            }
            else if(LayerOfUnitIcon[i] < ID)
            {
                LayerOfUnitIcon[i]++;
            }
        }

        GameObject CurrentUnitIcon = ContainUnitIcanArray[ID];

        for(int i=ID;i>=1;i--)
        {
            ContainUnitIcanArray[i] = ContainUnitIcanArray[i - 1];
        }

        ContainUnitIcanArray[0] = CurrentUnitIcon;


        // 这里获得的是mainframe
        //GameObject CurrentOperationPiece = ThisDebugInputController.GetCurrentOperationPiece();

        GameObject CurrentOperationPiece = ThisInputController.GetInputStateManager().GetCurrentOperationPiece();

        if (CurrentOperationPiece!= null)
        {
            //Debug.Log(CurrentOperationPiece.name);
            Vector2Int InterCoord = CurrentOperationPiece.GetComponent<PieceStateController>().ChessBoardInternalCoordinate;
            Vector2Int OuterCoord = CurrentOperationPiece.GetComponent<PieceStateController>().ChessBoardOuterCoordinate;

            GameObject LocateChessBoard = CurrentOperationPiece.GetComponent<PieceStateController>().LocateChessboard;

            GameObject SelectPiece = LocateChessBoard.GetComponent<ChessBoardController>().GetCheckerBoardPlayerAlivePiece(Vector2ToVector2Int(InterCoord), CurrentPlayerID, ID);

            LocateChessBoard.GetComponent<ChessBoardController>().PieceMove(SelectPiece, InterCoord, Vector2ToVector2Int(OuterCoord),0);

            //ThisDebugInputController.SetCurrentOperationPiece(SelectPiece.transform.Find("PieceMainframe").gameObject);

            ThisInputController.GetInputStateManager().SetCurrentOperationPiece(SelectPiece.transform.Find("PieceMainframe").gameObject);

            // 棋子层级变更动画播放设置
            SelectPiece.GetComponentInChildren<PieceAnimationController>().SetLiftToTop();
        }
        else
        {
            Debug.Log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        }
    }

	// Use this for initialization
	void Start ()
    {
        
    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}
}
