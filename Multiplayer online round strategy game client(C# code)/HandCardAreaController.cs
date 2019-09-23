using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HandCardAreaController : MonoBehaviour
{
    public GameObject HandCardPrefab = null;

    public GameObject GameManager = null;

    CardManager MainCardManager = null;

    Vector3[,] CardPosition = new Vector3[11, 10];
    Quaternion[,] CardRotation = new Quaternion[11, 10];

    int InHandCardNum = 0;

    List<GameObject> InHandCard = new List<GameObject>();


    public void PlayerGetAInitialHandCard(Card GetHandCard)
    {
        for (int i = 0; i < InHandCard.Count; i++)
        {
            // 触发每张卡的位置调整动画
            InHandCard[i].GetComponent<HandCardController>().SetReposition(CardPosition[InHandCardNum + 1, i], CardRotation[InHandCardNum + 1, i]);

        }

        // 初始化一张新卡
        // GameObject NewHandCard = GameObject.Instantiate(HandCardPrefab);

        GameObject NewHandCard = GetHandCard.GetMainHandCardBase();
        NewHandCard.transform.SetParent(GameObject.Find("Canvas/HandCardArea").gameObject.transform);
        //NewHandCard.transform.localPosition = new Vector3(0, 260, 0);



        // 将新卡添加到索引数组
        InHandCard.Add(NewHandCard);

        // 为新卡添加触发事件

        // 启动新卡的位移动画
        NewHandCard.GetComponent<HandCardController>().SetReposition(CardPosition[InHandCardNum + 1, InHandCardNum], CardRotation[InHandCardNum + 1, InHandCardNum]);

        InHandCardNum++;
    }

    // 玩家获取一张新的卡牌
    public void PlayerGetAHandCard(Card GetHandCard)
    {
        for (int i = 0; i < InHandCard.Count; i++)
        {
            // 触发每张卡的位置调整动画
            InHandCard[i].GetComponent<HandCardController>().SetReposition(CardPosition[InHandCardNum + 1, i], CardRotation[InHandCardNum + 1, i]);

        }

        // 初始化一张新卡
        // GameObject NewHandCard = GameObject.Instantiate(HandCardPrefab);

        GameObject NewHandCard = GetHandCard.GetMainHandCardBase();
        NewHandCard.transform.SetParent(GameObject.Find("Canvas/HandCardArea").gameObject.transform);
        NewHandCard.transform.localPosition = new Vector3(0, 260, 0);



        // 将新卡添加到索引数组
        InHandCard.Add(NewHandCard);

        // 为新卡添加触发事件

        // 启动新卡的位移动画
        NewHandCard.GetComponent<HandCardController>().SetReposition(CardPosition[InHandCardNum + 1, InHandCardNum], CardRotation[InHandCardNum + 1, InHandCardNum]);

        InHandCardNum++;
    }

    // 当一张手牌被从手牌区中拖出
    public void TakeOutAHandCard(GameObject ParaHandCard)
    {
        int HandCardIndex = -1;

        // 在手牌引用数组中找到是哪一张手牌被拖出
        for (int i = 0; i < InHandCard.Count; i++)
        {
            if(InHandCard[i] == ParaHandCard)
            {
                //Debug.Log("lalalalala");
                HandCardIndex = i;
                break;
            }
        }

        // 将鼠标拖出手牌区的手牌扶正
        ParaHandCard.transform.rotation = Quaternion.identity;

        int TmpIndex = 0;

        // 将其余手牌调整到当前手牌数目-1的摆放模式
        for (int i = 0; i < InHandCard.Count; i++)
        {
            if(i != HandCardIndex)
            {
                InHandCard[i].GetComponent<HandCardController>().SetReposition(CardPosition[InHandCardNum - 1, TmpIndex], CardRotation[InHandCardNum - 1, TmpIndex]);

                TmpIndex++;
            }
        }
    }

    // 当一张手牌施放失败被拖回手牌区
    public void RestoreAHandCard(GameObject ParaHandCard)
    {
        for (int i = 0; i < InHandCard.Count; i++)
        {
            InHandCard[i].GetComponent<HandCardController>().SetReposition(CardPosition[InHandCardNum, i], CardRotation[InHandCardNum, i]);
        }
    }

    // 当一张手牌施放成功时更新手牌数组
    public void SpellAHandCard(GameObject ParaHandCard)
    {
        //int HandCardIndex = -1;

        // 在手牌引用数组中找到是哪一张手牌被拖出
        for (int i = 0; i < InHandCard.Count; i++)
        {
            if (InHandCard[i] == ParaHandCard)
            {
                //Debug.Log("lalalalala");
                //HandCardIndex = i;

                InHandCard[i] = null;
                InHandCard.Remove(InHandCard[i]);

                break;
            }
        }

        InHandCardNum--;
    }

	// Use this for initialization
	void Start ()
    {
		for(int i=0;i<11;i++)
        {
            for(int j=0;j<10;j++)
            {
                if(j<i)
                {                   
                    GameObject TmpGameObject = this.transform.Find("CardPosition[" + i + "," + j + "]").gameObject;

                    CardPosition[i, j] = TmpGameObject.transform.localPosition;

                    CardRotation[i, j] = TmpGameObject.transform.rotation;
                }

            }
        }

        //MainCardManager = GameManager.GetComponent<GameController>().GetCardManager();

    }
	
	// Update is called once per frame
	void Update ()
    {


    }
}
