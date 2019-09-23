using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardLoader
{
    Dictionary<string, Card> CardOfCardName = new Dictionary<string, Card>();

    public CardLoader()
    {
        CardOfCardName.Add("MinionCard", new MinionCard());

    }

    public Card GetNewCardByType(string CardType)
    {
        Card ReturnCard = CardOfCardName[CardType];

        CardOfCardName[CardType] = CardOfCardName[CardType].ReloadNewCard();

        return ReturnCard;
    }
}
