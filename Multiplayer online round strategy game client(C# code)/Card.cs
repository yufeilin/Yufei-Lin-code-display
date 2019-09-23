using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Xml;     //xml操作

public class Card
{
    static WorldManager MainWorldManager = null;

    static GameController MainGameController = null;

    static ResourceManager MainResourceManager = null;

    static CardManager MainCardManager = null;

    int Owner = -1;



    public void SetWorldManager(WorldManager NewWorldManager)
    {
        MainWorldManager = NewWorldManager;
    }

    public WorldManager GetWorldManager()
    {
        return MainWorldManager;
    }

    public void SetMainGameController(GameController NewGameController)
    {
        MainGameController = NewGameController;

        MainResourceManager = MainGameController.GetResourceManager();

        MainCardManager = MainGameController.GetCardManager();

        MainWorldManager = MainGameController.GetWorldManager();
    }

    public GameController GetGameController()
    {
        return MainGameController;
    }

    public ResourceManager GetResourceManager()
    {
        return MainResourceManager;
    }

    public CardManager GetCardManager()
    {
        return MainCardManager;
    }

    public Card()
    {
        //MainWorldManager = GetGameController().GetWorldManager();
    }

    // 将卡牌信息传递给牌面物体
    public virtual void FetchInfoFromCard(GameObject MainDisplayCard)
    {

    }

    // 卡牌重装填
    public virtual Card ReloadNewCard()
    {
        return null;
    }

    // 获取参数
    public virtual object GetParameter(string ParaName)
    {
        return null;
    }

    // 设置参数
    public virtual void SetParameter(string ParaName, string ParaValue)
    {

    }

    public virtual void LoadInfoFromXML(XmlElement XMLInfo)
    {

    }

    public virtual GameObject GetMainHandCardBase()
    {
        return null;
    }

    public virtual void FetchInfoFromHandCard(CardDisplayController ParaCardDisplayController)
    {

    }

    public virtual void TargetPreviewOn(GameObject ParaCheckerBoard)
    {

    }

    public virtual void TargetPreviewOff()
    {

    }

    public virtual bool HandCardcCostCheck()
    {
        return false;
    }

    public virtual bool HandCardSpell()
    {
        return false;
    }

    public virtual void CardEnterGrave()
    {

    }

    public virtual Card GetCopy()
    {
        return null;
    }

    public virtual void SetOwner(int NewOwnerID)
    {
        Owner = NewOwnerID;
    }

    public int GetOwner()
    {
        return Owner;
    }

}

public class MinionCard : Card
{
    Dictionary<string, Parameter> ParameterOfName = new Dictionary<string, Parameter>();

    // ====================================

    

    // ====================================

    public MinionCard()
    {
        // 随从卡的卡牌ID和相关联的棋子的棋子ID是完全相同的所以不需要额外的字段来标记关联棋子的ID
        ParameterOfName.Add("CardID", new StringParameter());
        ParameterOfName.Add("CardCTN", new StringParameter());
        ParameterOfName.Add("CardName", new StringParameter());

        ParameterOfName["CardID"].SetParameter("");
        ParameterOfName["CardCTN"].SetParameter("");
        ParameterOfName["CardName"].SetParameter("");

        CardType = "MinionCard";
    }

    // 和随从卡关联的棋子物体引用(模板中为null而卡牌实例中为存在于游戏场景中的棋子物体)
    GameObject MinionPiece = null;

    // 和随从卡关联的卡面物体引用(模板中为null而卡牌实例中为存在于游戏场景中的卡面物体)
    GameObject MainHandCardBase = null;

    // 卡牌释放后产生的效果链接(模板链接与卡牌实例链接相同，直接复制即可)
    Effect SpellEffect = null;

    // 卡牌类型字符串(一般不改变)
    string CardType = "MinionCard";

    public override object GetParameter(string ParaName)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            return ParameterOfName[ParaName].GetParameter();
        }
        else
        {
            return null;
        }
    }

    public override void SetParameter(string ParaName, string ParaValue)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            ParameterOfName[ParaName].SetParameter(ParaValue);
        }
    }

    public override Card ReloadNewCard()
    {
        return new MinionCard();
    }

    public override void LoadInfoFromXML(XmlElement XMLInfo)
    {
        foreach (XmlElement EffectSubInfoItem in XMLInfo.ChildNodes)
        {
            SetParameter(EffectSubInfoItem.Name, EffectSubInfoItem.InnerText);
        }

        //Debug.Log((string)GetParameter("HandCardID") + " " + (string)GetParameter("HandCardName"));
    }

    public GameObject GetMinionPiece()
    {
        return MinionPiece;
    }

    public void SetMinionPiece(GameObject NewMinionPiece)
    {
        MinionPiece = NewMinionPiece;
    }

    public override GameObject GetMainHandCardBase()
    {
        return MainHandCardBase;
    }

    public override void FetchInfoFromHandCard(CardDisplayController ParaCardDisplayController)
    {
        ParaCardDisplayController.LoadInfoFromPiece(MinionPiece);       
    }

    // 这个是当手牌被鼠标拖出手牌区但是鼠标尚未松开时调用的目标选择函数
    // 对于随从卡，是把对应的棋子放置在瞄准的棋盘格上
    // 参数ParaCheckerBoard即为当前鼠标射线所在的棋盘格
    public override void TargetPreviewOn(GameObject ParaCheckerBoard)
    {
        //MinionPiece.transform.Find("PieceMainframe").GetComponent<CapsuleCollider>().enabled = false;
        // 显示当前卡牌对应的棋子物体
        MinionPiece.SetActive(true);
        // 将当前卡牌对应的棋子物体移动到棋盘格的中心位置
        MinionPiece.transform.position = ParaCheckerBoard.GetComponent<CheckerBoardInfoController>().CheckerBoardInternalPosition;
    }

    // 隐藏当前卡牌的瞄准模式
    public override void TargetPreviewOff()
    {
        //MinionPiece.transform.Find("PieceMainframe").GetComponent<CapsuleCollider>().enabled = false;
        //MinionPiece.transform.position = ParaCheckerBoard.GetComponent<CheckerBoardInfoController>().CheckerBoardInternalPosition;
        // 隐藏当前卡牌对应的棋子
        MinionPiece.SetActive(false);
    }

    public override bool HandCardcCostCheck()
    {
        int PlayerMineral = GetResourceManager().GetClientPlayerMineral();
        int PlayerGas = GetResourceManager().GetClientPlayerGas();
        int PieceCostMineral = MinionPiece.GetComponentInChildren<PieceInfoController>().GetPieceCostMineral();
        int PieceCostGas = MinionPiece.GetComponentInChildren<PieceInfoController>().GetPieceCostGas();

        if (PieceCostMineral > PlayerMineral || PieceCostGas > PlayerGas)
        {
            // 显示一条提示资源不足的消息
            GetGameController().MessageManager.GetComponent<MessageController>().PrivateMessage("资源不足");
            // 卡牌施放失败返回false
            return false;
        }

        return true;
    }

    public override bool HandCardSpell()
    {
        Ray DetectRay;
        RaycastHit Hit;

        GameObject ColliderGameObject;

        DetectRay = Camera.main.ScreenPointToRay(Input.mousePosition);

        if (Physics.Raycast(DetectRay, out Hit))
        {
            ColliderGameObject = Hit.collider.gameObject;
        }
        else
        {
            ColliderGameObject = null;
            return false;
        }

        // 费用检查
        //Debug.Log("This Card belong to player " + GetOwner());
        int PlayerMineral = GetResourceManager().GetPlayerMineral(GetOwner());
        int PlayerGas = GetResourceManager().GetPlayerGas(GetOwner());
        int PieceCostMineral = MinionPiece.GetComponentInChildren<PieceInfoController>().GetPieceCostMineral();
        int PieceCostGas = MinionPiece.GetComponentInChildren<PieceInfoController>().GetPieceCostGas();
        if(PieceCostMineral > PlayerMineral || PieceCostGas > PlayerGas)
        {
            // 显示一条提示资源不足的消息
            GetGameController().MessageManager.GetComponent<MessageController>().PrivateMessage("资源不足");
            // 卡牌施放失败返回false
            return false;
        }

        if (ColliderGameObject != null && ColliderGameObject.GetComponent<GameObjectPropertyController>().IsCheckerBoard == true)
        {
            // 如果鼠标悬停在一个棋盘格上
            CheckerBoardInfoController TmpCheckerBoardInfoController = ColliderGameObject.GetComponent<CheckerBoardInfoController>();
            // 卡牌释放
            GetWorldManager().DeployPieceOnChessBoard(MinionPiece, TmpCheckerBoardInfoController.CheckerBoardInternalCoordinate, TmpCheckerBoardInfoController.CheckerBoardOuterCoordinate);

            MinionPiece.GetComponentInChildren<PieceAnimationController>().DeployAnimationStart();

            // 资源消耗更新
            PlayerMineral -= PieceCostMineral;
            PlayerGas -= PieceCostGas;
            GetResourceManager().SetPlayerMineral(GetOwner(), PlayerMineral);
            GetResourceManager().SetPlayerGas(GetOwner(), PlayerGas);

            return true;
        }
        else if(ColliderGameObject != null && ColliderGameObject.GetComponent<GameObjectPropertyController>() != null && ColliderGameObject.GetComponent<GameObjectPropertyController>().IsPiece == true)
        {
            // 如果鼠标悬停在一个棋子上
            PieceStateController TmpPieceStateController = ColliderGameObject.GetComponentInChildren<PieceStateController>();

            // TODO: 此处应该检查该棋盘格处是否含有可见的敌方棋子
            // 如果含有的话，不具有渗透者技能的棋子会部署失败
            // 如果含有的话，具有渗透者技能的棋子可以成功部署
            // 如果不含有敌方棋子则可以成功部署

            GameObject TmpCheckerBoard = TmpPieceStateController.LocateChessboard.GetComponent<ChessBoardController>().GetCheckerBoard(TmpPieceStateController.ChessBoardInternalCoordinate);

            CheckerBoardInfoController TmpCheckerBoardInfoController = TmpCheckerBoard.GetComponent<CheckerBoardInfoController>();
            //Debug.Log(CurrentHoverCheckerBoard.name);

            GetWorldManager().DeployPieceOnChessBoard(MinionPiece, TmpCheckerBoardInfoController.CheckerBoardInternalCoordinate, TmpCheckerBoardInfoController.CheckerBoardOuterCoordinate);

            MinionPiece.GetComponentInChildren<PieceAnimationController>().DeployAnimationStart();

            // 资源消耗更新
            PlayerMineral -= PieceCostMineral;
            PlayerGas -= PieceCostGas;
            GetResourceManager().SetPlayerMineral(GetOwner(), PlayerMineral);
            GetResourceManager().SetPlayerGas(GetOwner(), PlayerGas);

            return true;
        }
        else
        {
            return false;
        }
    }

    public override void CardEnterGrave()
    {
        //1.触发卡牌进入坟场的效果

        //2.通知卡牌管理器将当前卡牌放入坟场
        GetCardManager().PlayerGetAGraveCard(GetOwner(), this);
    }

    public void SetMainHandCardBase(GameObject NewMainHandCardBase)
    {
        MainHandCardBase = NewMainHandCardBase;
    }

    public Effect GetSpellEffect()
    {
        return SpellEffect;
    }

    public void SetSpellEffect(Effect NewSpellEffect)
    {
        SpellEffect = NewSpellEffect;
    }

    // 向卡面传递卡牌相关信息以刷新卡面的显示
    public override void FetchInfoFromCard(GameObject MainDisplayCard)
    {
        MainDisplayCard.GetComponent<DisplayCardController>().LoadInfoFromPiece(MinionPiece);
    }

    // 根据当前卡牌模板复制一张新的卡牌对象
    // this 是卡牌模板
    // NewMinionCard 是生成的新的卡牌实例
    public override Card GetCopy()
    {
        MinionCard NewMinionCard = new MinionCard();

        // 1.将新卡的释放效果链接到模板卡的释放效果
        NewMinionCard.SetSpellEffect(SpellEffect);

        // 2.创建棋子GameObject并将引用进行设置       
        NewMinionCard.SetMinionPiece(GetWorldManager().CreatePieceForCard((string)GetParameter("CardID")));
        NewMinionCard.GetMinionPiece().GetComponentInChildren<PieceStateController>().SetCard(NewMinionCard);
        //NewMinionCard.GetMinionPiece().GetComponentInChildren<PieceStateController>().DisplayPiece();


        // 3.创建卡面显示GameObject并设置引用
        // ---------------------------------------------
        // 获取卡面物体预制体
        GameObject MainHandCardBasePrefab = (GameObject)Resources.Load("Prefabs/HandCardBase");
        // 实例化卡面物体
        NewMinionCard.SetMainHandCardBase(GameObject.Instantiate(MainHandCardBasePrefab));
        NewMinionCard.GetMainHandCardBase().GetComponent<HandCardController>().SetCard(NewMinionCard);

        // 将卡面物体放置于Canvas下
        NewMinionCard.GetMainHandCardBase().transform.SetParent(GameObject.Find("Canvas").gameObject.transform);
        // 设置默认位置
        NewMinionCard.GetMainHandCardBase().GetComponent<RectTransform>().localPosition = new Vector3(0,0,0);
        // 将卡面物体的卡牌引用设置为当前卡牌
        NewMinionCard.GetMainHandCardBase().GetComponent<HandCardController>().SetCard(NewMinionCard);
        // 将卡面物体隐藏
        //MainHandCardBase.SetActive(false);
        // ---------------------------------------------
        return NewMinionCard;
    }

    // 为随从卡设置所有者
    // 设置卡牌所有者
    // 设置卡牌对应棋子中所有单位的所有者
    public override void SetOwner(int NewOwnerID)
    {
        base.SetOwner(NewOwnerID);

        MinionPiece.GetComponentInChildren<PieceStateController>().SetOwner(NewOwnerID);
    }


}

public class SpellCard : Card
{
    Dictionary<string, Parameter> ParameterOfName = new Dictionary<string, Parameter>();

    public SpellCard()
    {
        // 随从卡的卡牌ID和相关联的棋子的棋子ID是完全相同的所以不需要额外的字段来标记关联棋子的ID
        ParameterOfName.Add("CardID", new StringParameter());
        ParameterOfName.Add("CardCTN", new StringParameter());
        ParameterOfName.Add("CardName", new StringParameter());

        ParameterOfName["CardID"].SetParameter("");
        ParameterOfName["CardCTN"].SetParameter("");
        ParameterOfName["CardName"].SetParameter("");

    }

    // 待定: 卡牌瞄准准星物体的引用
    GameObject SelectTarget = null;

    // 和随从卡关联的法术实例引用(模板中为null而卡牌实例中为存在于内存中的法术实例)
    SpellInstance RelativeSpell = null;

    // 和随从卡关联的卡面物体引用(模板中为null而卡牌实例中为存在于游戏场景中的卡面物体)
    GameObject MainHandCardBase = null;

    // 卡牌释放后产生的效果链接(模板链接与卡牌实例链接相同，直接复制即可)
    Effect SpellEffect = null;

    public override object GetParameter(string ParaName)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            return ParameterOfName[ParaName].GetParameter();
        }
        else
        {
            return null;
        }
    }

    public override void SetParameter(string ParaName, string ParaValue)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            ParameterOfName[ParaName].SetParameter(ParaValue);
        }
    }

    public override Card ReloadNewCard()
    {
        return new SpellCard();
    }

    public override void LoadInfoFromXML(XmlElement XMLInfo)
    {
        foreach (XmlElement EffectSubInfoItem in XMLInfo.ChildNodes)
        {
            SetParameter(EffectSubInfoItem.Name, EffectSubInfoItem.InnerText);
        }
    }

    public SpellInstance GetRelativeSpell()
    {
        return RelativeSpell;
    }

    public void SetRelativeSpell(SpellInstance NewRelativeSpell)
    {
        RelativeSpell = NewRelativeSpell;
    }

    public override GameObject GetMainHandCardBase()
    {
        return MainHandCardBase;
    }

    public override void FetchInfoFromHandCard(CardDisplayController ParaCardDisplayController)
    {
       // 将卡牌关联的法术实例移交给卡牌详情显示控制器用来刷新显示卡牌
       ParaCardDisplayController.LoadInfoFromSpellInstance(RelativeSpell);
    }

    // 这个是当手牌被鼠标拖出手牌区但是鼠标尚未松开时调用的目标选择函数
    // 对于法术卡，是把对应的准星放置在瞄准的棋盘格上
    // 参数ParaCheckerBoard即为当前鼠标射线所在的棋盘格
    public override void TargetPreviewOn(GameObject ParaCheckerBoard)
    {
        if(SelectTarget == null)
        {
            GameObject SelectTargetPrefab = (GameObject)Resources.Load("Prefabs/SelectTarget");
            SelectTarget = GameObject.Instantiate(SelectTargetPrefab);
        }

        // 显示卡牌瞄准准星
        SelectTarget.SetActive(true);

        // 将瞄准准星移动到目标棋盘格的中心位置
        SelectTarget.transform.position = ParaCheckerBoard.GetComponent<CheckerBoardInfoController>().CheckerBoardInternalPosition;

        // 显示当前卡牌对应的棋子物体
        //MinionPiece.SetActive(true);
    }

    // 隐藏当前卡牌的瞄准模式
    public override void TargetPreviewOff()
    {
        // 隐藏瞄准准星
        GameObject.Destroy(SelectTarget);

        // 隐藏当前卡牌对应的棋子
        //MinionPiece.SetActive(false);
    }

    public override bool HandCardSpell()
    {
        return RelativeSpell.CardSpell();
    }

    public override void CardEnterGrave()
    {
        //1.触发卡牌进入坟场的效果

        //2.通知卡牌管理器将当前卡牌放入坟场
        GetCardManager().PlayerGetAGraveCard(GetOwner(), this);
    }

    public void SetMainHandCardBase(GameObject NewMainHandCardBase)
    {
        MainHandCardBase = NewMainHandCardBase;
    }

    public Effect GetSpellEffect()
    {
        return SpellEffect;
    }

    public void SetSpellEffect(Effect NewSpellEffect)
    {
        SpellEffect = NewSpellEffect;
    }

    // 向卡面传递卡牌相关信息以刷新卡面的显示
    public override void FetchInfoFromCard(GameObject MainDisplayCard)
    {
        MainDisplayCard.GetComponent<DisplayCardController>().LoadInfoFromSpellInstance(RelativeSpell);
    }

    // 根据当前卡牌模板复制一张新的卡牌对象
    // this 是卡牌模板
    // NewMinionCard 是生成的新的卡牌实例
    public override Card GetCopy()
    {
        SpellCard NewSpellCard = new SpellCard();

        // 1.将新卡的释放效果链接到模板卡的释放效果
        NewSpellCard.SetSpellEffect(SpellEffect);

        // 2.创建法术实例并将引用进行设置       
        //NewMinionCard.SetMinionPiece(GetWorldOperationController().CreatePieceForCard((string)GetParameter("HandCardID")));
        //NewMinionCard.GetMinionPiece().GetComponentInChildren<PieceStateController>().SetCard(NewMinionCard);
        //NewMinionCard.GetMinionPiece().GetComponentInChildren<PieceStateController>().DisplayPiece();


        // 3.创建卡面显示GameObject并设置引用
        // ---------------------------------------------
        // 获取卡面物体预制体
        GameObject MainHandCardBasePrefab = (GameObject)Resources.Load("Prefabs/HandCardBase");
        // 实例化卡面物体
        NewSpellCard.SetMainHandCardBase(GameObject.Instantiate(MainHandCardBasePrefab));
        // 将卡面物体的卡牌引用设置为当前卡牌
        NewSpellCard.GetMainHandCardBase().GetComponent<HandCardController>().SetCard(NewSpellCard);

        // 将卡面物体放置于Canvas下
        NewSpellCard.GetMainHandCardBase().transform.SetParent(GameObject.Find("Canvas").gameObject.transform);
        // 设置默认位置
        NewSpellCard.GetMainHandCardBase().GetComponent<RectTransform>().localPosition = new Vector3(0, 0, 0);        
        // 将卡面物体隐藏
        //MainHandCardBase.SetActive(false);
        // ---------------------------------------------
        return NewSpellCard;
    }

    // 为随从卡设置所有者
    // 设置卡牌所有者
    // 设置卡牌对应棋子中所有单位的所有者
    public override void SetOwner(int NewOwnerID)
    {
        base.SetOwner(NewOwnerID);

        RelativeSpell.SetOwner(NewOwnerID);
    }
}