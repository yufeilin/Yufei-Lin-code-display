using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CompareOperation
{
    int ControlVar = 0;

    public CompareOperation(int Tmp)
    {
        ControlVar = Tmp;
    }

    public bool DoCompare(float a1, float a2)
    {
        if(ControlVar == 2)
        {
            return a1 >= a2;
        }
        else if(ControlVar == 1)
        {
            return a1 > a2;
        }
        else if (ControlVar == 0)
        {
            return a1 == a2;
        }
        else if (ControlVar == -1)
        {
            return a1 < a2;
        }
        else
        {
            return a1 <= a2;
        }
    }
}

public class CardFilter
{
    public static Dictionary<int, CompareOperation> CompareOperationOfType = new Dictionary<int, CompareOperation>();



    // 稀有度判断符号
    // RarityOperation = 2 筛选稀有度 >= Rarity 的卡牌
    // RarityOperation = 1 筛选稀有度 > Rarity 的卡牌
    // RarityOperation = 0 筛选稀有度 == Rarity 的卡牌
    // RarityOperation = -1 筛选稀有度 < Rarity 的卡牌
    // RarityOperation = -2 筛选稀有度 <= Rarity 的卡牌
    int RarityOperation = 0;



    // 稀有度判断值
    // Rarity = -1 不进行稀有度筛选
    // Rarity = 0 白色稀有度
    // Rarity = 1 蓝色稀有度
    // Rarity = 2 紫色稀有度
    // Rarity = 3 橙色稀有度
    // Rarity = 4 彩色稀有度
    int Rarity = -1;

    // 卡牌标旗筛选数组
    List<string> CardFlage = new List<string>();

    // 卡牌属性筛选数组
    List<string> CardAttribute = new List<string>();

    public void Initialize()
    {
        CompareOperationOfType.Add(2, new CompareOperation(2));
        CompareOperationOfType.Add(1, new CompareOperation(1));
        CompareOperationOfType.Add(0, new CompareOperation(0));
        CompareOperationOfType.Add(-1, new CompareOperation(-1));
        CompareOperationOfType.Add(-2, new CompareOperation(-2));
    }

    public bool ExecuteCardFilter(Card ParaCard)
    {
        if(Rarity >= 0)
        {
            // int CurrentCardRarity = ParaCard.GetCardRarity()
            int CurrentCardRarity = 0;
            // 如果不符合稀有度筛选则直接返回false
            if (!CompareOperationOfType[RarityOperation].DoCompare(CurrentCardRarity, Rarity))
            {
                return false;
            }
        }

        return true;
    }
}
