using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardManager
{
    GameController MainGameController = null;

    LibraryManager MainLibraryManager = null;

    // 每个玩家的手牌区游戏物体数组
    Dictionary<int, GameObject> PlyerHandCardArea = new Dictionary<int, GameObject>();

    // 每个玩家的手牌区卡牌数组
    Dictionary<int, List<Card>> PlayerHandCard = new Dictionary<int, List<Card>>();

    // 每个玩家的牌堆区卡牌数组
    Dictionary<int, List<Card>> PlayerDeck = new Dictionary<int, List<Card>>();

    // 每个玩家的坟场区卡牌数组
    Dictionary<int, List<Card>> PlayerGrave = new Dictionary<int, List<Card>>();

    // 每个玩家的放逐区卡牌数组
    Dictionary<int, List<Card>> PlayerExile = new Dictionary<int, List<Card>>();

    Dictionary<int, int> PlayerIndex = new Dictionary<int, int>();

    int PlayerNum = 0;

    List<Card> ClientPlayerDeck = new List<Card>();

    List<Card> ClientPlayerInGameDeck = new List<Card>();

    public CardManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;        
    }

    public void EstablishReference()
    {
        MainLibraryManager = MainGameController.GetLibraryManager();
    }

    // 如果所有卡牌区数组中都不存在玩家PlayerID则返回true
    bool PlayerNotExist(int PlayerID)
    {
        if(!PlayerHandCard.ContainsKey(PlayerID) && !PlayerDeck.ContainsKey(PlayerID) && !PlayerGrave.ContainsKey(PlayerID) && !PlayerExile.ContainsKey(PlayerID))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // 如果所有卡牌区数组中都存在玩家PlayerID则返回true
    bool PlayerExist(int PlayerID)
    {
        if(PlayerHandCard.ContainsKey(PlayerID) && PlayerDeck.ContainsKey(PlayerID) && PlayerGrave.ContainsKey(PlayerID) && PlayerExile.ContainsKey(PlayerID))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public void AddPlayer(int PlayerID)
    {
        if(PlayerNotExist(PlayerID))
        {
            PlayerHandCard.Add(PlayerID, new List<Card>());
            PlayerDeck.Add(PlayerID, new List<Card>());
            PlayerGrave.Add(PlayerID, new List<Card>());
            PlayerExile.Add(PlayerID, new List<Card>());
            PlyerHandCardArea.Add(PlayerID, GameObject.Find("Canvas/HandCardArea"));
            PlayerIndex.Add(PlayerNum, PlayerID);
            PlayerNum++;
        }
        else
        {
            Debug.Log("Add player fail. player exist.");
        }
    }

    // 实例化一张随机的属于玩家PlayerID的卡牌对象并返回
    // TODO: 完成这个函数，目前它只能返回一张特定的卡牌实例对象
    public Card CreateARandomCard(int PlayerID, CardFilter ParaCardFilter = null)
    {
        if(ParaCardFilter == null)
        {
            // 如果没有任何随机卡牌筛选要求则直接随机生成一张卡牌

            // 从卡牌模板实例化一张卡牌对象(相当于深复制，在这个过程中卡面物体和相关棋子物体等会被生成并隐藏)
            Card NewCard = MainLibraryManager.GetCard("BanelingCommando").GetCopy();

            NewCard.SetOwner(PlayerID);

            return NewCard;
        }
        else
        {
            // 如果有具体的随机卡牌生成要求则遍历所有的卡牌并生成一个临时引用数组然后再该引用数组中随机选择卡牌

            // 从卡牌模板实例化一张卡牌对象(相当于深复制，在这个过程中卡面物体和相关棋子物体等会被生成并隐藏)
            Card NewCard = MainLibraryManager.GetCard("BanelingCommando").GetCopy();

            NewCard.SetOwner(PlayerID);

            return NewCard;
        }
        
    }

    // 根据卡牌ID实例化一张属于玩家PlayerID的卡牌对象并返回
    public Card CreateACard(string CardID, int PlayerID)
    {
        // 从卡牌模板实例化一张卡牌对象(相当于深复制，在这个过程中卡面物体和相关棋子物体等会被生成并隐藏)
        Card NewCard = MainLibraryManager.GetCard(CardID).GetCopy();

        NewCard.SetOwner(PlayerID);

        return NewCard;
    }

    public void InitialPlayerDeck(int PlayerID, List<string> PlayerDeckNameList)
    {
        List<Card> Tmp = new List<Card>();

        for (int i = 0; i < PlayerDeckNameList.Count; i++)
        {
            // 从卡牌模板实例化一张卡牌对象(相当于深复制，在这个过程中卡面物体和相关棋子物体等会被生成并隐藏)
            Card NewCard = MainLibraryManager.GetCard(PlayerDeckNameList[i]).GetCopy();

            // 设置卡牌所有者
            NewCard.SetOwner(PlayerID);

            // 当卡牌所有的准备事宜就绪后就可以放入牌堆list了
            Tmp.Add(NewCard);
        }

        if (PlayerExist(PlayerID) == true)
        {
            PlayerDeck[PlayerID] = Tmp;
        }
        else
        {
            Debug.Log("Player Deck Initialize Error.");
        }
    }


    public void PlayerDrawACard(int PlayerID)
    {
        if(PlayerDeck[PlayerID].Count >= 1)
        {
            List<Card> Deck = PlayerDeck[PlayerID];
            int RandomIndex = Random.Range(0, Deck.Count - 1);

            Card DrawHandCard = Deck[RandomIndex];

            // 2.更新抽牌玩家的手牌区卡牌数组
            PlayerHandCard[PlayerID].Add(DrawHandCard);

            // 3.移除
            Deck[RandomIndex] = null;
            Deck.Remove(Deck[RandomIndex]);

            // 4.更新该玩家手牌区UI
            PlyerHandCardArea[PlayerID].GetComponent<HandCardAreaController>().PlayerGetAHandCard(DrawHandCard);

            //return DrawHandCard;
        }
        else
        {
            //return null;
        }        
    }

    public void PlyerCastACard(int PlayerID, Card CastHandCard)
    {
        // 1.将该手牌从对应玩家的手牌区卡牌数组中移除

        List<Card> PlayerHandCards = PlayerHandCard[PlayerID];

        int TmpIndex = -1;

        for (int i=0;i< PlayerHandCards.Count;i++)
        {
            if(PlayerHandCards[i] == CastHandCard)
            {
                TmpIndex = i;
                break;
            }
        }

        //Debug.Log("Card Casted~~~~~~~~~~~~~~~~~~~~~");

        PlayerHandCards[TmpIndex] = null;
        PlayerHandCards.Remove(PlayerHandCards[TmpIndex]);

        // 2.将该卡牌放入战场(随从或者装备卡)或者坟场中(法术卡)

        // 3.通知该玩家的手牌区游戏物体进行更新
        PlyerHandCardArea[PlayerID].GetComponent<HandCardAreaController>().SpellAHandCard(CastHandCard.GetMainHandCardBase());

    }

    // 向玩家的手牌中添加一张卡牌（不是从自己的卡组中抽取）
    public void PlayerGetAHandCard(int PlayerID, Card GetCard)
    {
        List<Card> PlayerHandCards = PlayerHandCard[PlayerID];

        //1.将获得的卡牌添加到该玩家的手牌区卡牌数组中
        PlayerHandCards.Add(GetCard);

        //2.更新该玩家的手牌区UI
        PlyerHandCardArea[PlayerID].GetComponent<HandCardAreaController>().PlayerGetAHandCard(GetCard);
    }

    // 向玩家的牌库中添加一张卡牌
    public void PlayerGetADeckCard(int PlayerID, Card GetCard)
    {
        List<Card> PlayerDeckCards = PlayerDeck[PlayerID];

        //1.将获得的卡牌添加到该玩家的牌库区卡牌数组中
        PlayerDeckCards.Add(GetCard);

        //2.更新该玩家的牌库区UI
    }

    public void PlayerGetAGraveCard(int PlayerID, Card GetCard)
    {
        List<Card> PlayerGraveList = PlayerGrave[PlayerID];

        Debug.Log("Card enter grave");

        //1.将获得的卡牌添加到该玩家的坟场区卡牌数组中
        PlayerGraveList.Add(GetCard);

        //2.更新该玩家的坟场区UI
    }

    public void Player0DrawACard()
    {
        PlayerDrawACard(0);
    }
}
