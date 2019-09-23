using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DiplomacyManager
{
    GameController MainGameController = null;

    // 玩家外交关系矩阵
    // DiplomacyMatrix[i, j] = 0 玩家i视玩家j为敌人 玩家i可攻击玩家j
    // DiplomacyMatrix[i, j] = 1 玩家i视玩家j为盟友 玩家i不可攻击玩家j
    int[,] DiplomacyMatrix = null;

    // 本场游戏的玩家总数
    int PlayerNum = 5;

    public DiplomacyManager(GameController ParaGameController)
    {
        MainGameController = ParaGameController;

        DiplomacyMatrix = new int[PlayerNum, PlayerNum];

        // 初始化将所有玩家设为彼此为敌人
        for (int i = 0; i < PlayerNum; i++)
        {
            for (int j = 0; j < PlayerNum; j++)
            {
                if (i == j)
                {
                    DiplomacyMatrix[i, j] = 1;
                }
                else
                {
                    DiplomacyMatrix[i, j] = 0;
                }

            }
        }
    }

    public void EstablishReference()
    {

    }

    public void SetDiplomacy(int PlayerI, int PlayerJ, int NerDiplomacy)
    {
        DiplomacyMatrix[PlayerI, PlayerJ] = NerDiplomacy;
    }

    public int GetDiplomacy(int PlayerI, int PlayerJ)
    {
        Debug.Log(PlayerI + " " + PlayerJ);

        return DiplomacyMatrix[PlayerI, PlayerJ];
    }



}
