using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DisplayCardController : MonoBehaviour
{

    public GameObject MainDisplayCard = null;

    // 卡牌显示控制变量
    List<GameObject> UnitCardUIReferenceList = new List<GameObject>();
    // ==================================================
    // 单位卡牌显示UI对应引用
    // --------------------------------------------------    
    public GameObject CardNameText = null;
    public GameObject MainImage = null;
    public GameObject PieceUnitNumText = null;
    public GameObject MineralBar = null;
    public GameObject MineralText = null;
    public GameObject GasBar = null;
    public GameObject GasText = null;
    public GameObject CardDescription = null;
    public GameObject EnergyBarBackground = null;
    public GameObject EnergyBar = null;
    public GameObject ShieldBarBackground = null;
    public GameObject ShieldBar = null;
    public GameObject HealthBarBackground = null;
    public GameObject HealthBar = null;
    // --------------------------------------------------

    // 法术卡牌显示UI对应的引用

    // 装备卡牌显示UI对应的引用

    // ==================================================

    void InitializeGameObjectReference()
    {
        if (MainDisplayCard == null) { MainDisplayCard = this.gameObject; }
        if (CardNameText == null) { CardNameText = MainDisplayCard.transform.Find("CardTopPart/CardNameText").gameObject; UnitCardUIReferenceList.Add(CardNameText); }
        if (MainImage == null) { MainImage = MainDisplayCard.transform.Find("MainImage").gameObject; UnitCardUIReferenceList.Add(MainImage); }
        if (PieceUnitNumText == null) { PieceUnitNumText = MainDisplayCard.transform.Find("CardTopPart/UnitNumText").gameObject; UnitCardUIReferenceList.Add(PieceUnitNumText); }
        if (MineralBar == null) { MineralBar = MainDisplayCard.transform.Find("CardTopPart/MineralBar").gameObject; UnitCardUIReferenceList.Add(MineralBar); }
        if (MineralText == null) { MineralText = MineralBar.transform.Find("MineralText").gameObject; UnitCardUIReferenceList.Add(MineralText); }
        if (GasBar == null) { GasBar = MainDisplayCard.transform.Find("CardTopPart/GasBar").gameObject; UnitCardUIReferenceList.Add(GasBar); }
        if (GasText == null) { GasText = GasBar.transform.Find("GasText").gameObject; UnitCardUIReferenceList.Add(GasText); }

        if(CardDescription == null){CardDescription = MainDisplayCard.transform.Find("CardDescription").gameObject;UnitCardUIReferenceList.Add(CardDescription);}

        if (EnergyBarBackground == null) { EnergyBarBackground = MainDisplayCard.transform.Find("CardBottomPart/EnergyBarBackground").gameObject; UnitCardUIReferenceList.Add(EnergyBarBackground); }
        if (EnergyBar == null) { EnergyBar = EnergyBarBackground.transform.Find("EnergyBar").gameObject; UnitCardUIReferenceList.Add(EnergyBar); }
        if (ShieldBarBackground == null) { ShieldBarBackground = MainDisplayCard.transform.Find("CardBottomPart/ShieldBarBackground").gameObject; UnitCardUIReferenceList.Add(ShieldBarBackground); }
        if (ShieldBar == null) { ShieldBar = ShieldBarBackground.transform.Find("ShieldBar").gameObject; UnitCardUIReferenceList.Add(ShieldBar); }
        if (HealthBarBackground == null) { HealthBarBackground = MainDisplayCard.transform.Find("CardBottomPart/HealthBarBackground").gameObject; UnitCardUIReferenceList.Add(HealthBarBackground); }
        if (HealthBar == null) { HealthBar = HealthBarBackground.transform.Find("HealthBar").gameObject; UnitCardUIReferenceList.Add(HealthBar); }
    }

    // 显示展示卡牌
    public void MainDisplayCardDisplayOn()
    {
        MainDisplayCard.SetActive(true);
        //MainDisplayCardDisplay = true;
    }

    // 隐藏展示卡牌
    public void MainDisplayCardDisplayOff()
    {
        MainDisplayCard.SetActive(false);
        //MainDisplayCardDisplay = false;
    }

    // 将一个棋子当前的状态信息装入展示卡牌当中
    public void LoadInfoFromPiece(GameObject ParaPiece)
    {
        PieceInfoController ParaPieceInfoController = ParaPiece.GetComponentInChildren<PieceInfoController>();
        PieceStateController ParaPieceStateController = ParaPiece.GetComponentInChildren<PieceStateController>();

        CardNameText.GetComponent<Text>().text = ParaPieceInfoController.GetPieceName();

        if(ParaPieceStateController.GetCardMainImage() != null)
        {
            MainImage.GetComponent<RawImage>().texture = ParaPieceStateController.GetCardMainImage();
        }
        else
        {

        }

        //MainImage.GetComponent<RawImage>().texture = 
        // 也许在单位数目为1的时候英雄棋子和普通杂兵棋子需要区别对待(英雄类棋子直接隐藏单位数目图标和单位数目文本)
        PieceUnitNumText.GetComponent<Text>().text = (ParaPieceStateController.GetAliveUnitNum()).ToString();
        MineralText.GetComponent<Text>().text = (ParaPieceInfoController.GetPieceCostMineral()).ToString();
        GasText.GetComponent<Text>().text = (ParaPieceInfoController.GetPieceCostGas()).ToString();

        List<string> CardDescriptionItemList = ParaPieceStateController.GetCardDescriptionItemList();

        int CharacterNum = 0;

        if(CardDescriptionItemList != null)
        {
            //Debug.Log("get it");

            for (int i = 0; i < CardDescriptionItemList.Count; i++)
            {
                CharacterNum += CardDescriptionItemList[i].Length;
            }

            // 如果描述项字符总量过多则需要调整文本中的字体大小
            if (CharacterNum <= 56)
            {
                string Res = "";

                // 组装卡牌描述区文本，每个描述项之间用一个空格隔开
                for (int i = 0; i < CardDescriptionItemList.Count; i++)
                {
                    if (i == CardDescriptionItemList.Count - 1)
                    {
                        Res += CardDescriptionItemList[i];
                    }
                    else
                    {
                        Res += (CardDescriptionItemList[i] + " ");
                    }
                }

                // 将卡牌描述区文本设置到text属性
                CardDescription.transform.Find("CardDescriptionText").gameObject.GetComponent<Text>().text = Res;
            }
        }

        // 更新生命值状态条
        float HealthPercentage = ParaPieceStateController.GetRationOfHealth();
      
        float HealthBarWidth = 145 * HealthPercentage;
        float HealthBarPosition = 145 * (1 - HealthPercentage);
        HealthBar.GetComponent<RectTransform>().sizeDelta = new Vector2(HealthBarWidth, 6);
        HealthBar.GetComponent<RectTransform>().localPosition = new Vector3((float)(HealthBarPosition / 2.0), 0, 0);

    }

    // 将一个法术实例当前的状态信息装入展示卡牌当中
    public void LoadInfoFromSpellInstance(SpellInstance ParaSpellInstance)
    {
        //todo
    }

    // Use this for initialization
    void Start ()
    {
        InitializeGameObjectReference();
    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
