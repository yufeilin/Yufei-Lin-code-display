using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpellInstance
{
    static public GameObject GameManager = null;

    static public GameObject MessageManager = null;
    // 法术晶体矿消耗
    int SpellMineralCost = 0;

    // 法术瓦斯消耗
    int SpellGasCost = 0;

    // 法术能量消耗
    int SpellEnergyCost = 0;

    Card RelativeCard = null;

    // 法术尺寸（决定受到影响的棋子内单位数量）
    float SpellDimension = 0.0f;

    // 法术重数（描述区中的法术效果会执行的次数）
    int SpellNum = 1;

    // 目标选择
    // TargetSelect == 0 目标必须为棋子
    // TargetSelect == 1 目标必须为棋盘格
    // TargetSelect == 2 目标可以是棋盘格也可以是棋子
    int TargetSelect = 0;

    int Owner = -1;

    Effect RelativeEffect = null;

    public SpellInstance()
    {
        if(GameManager == null)
        {
            GameManager = GameObject.Find("GameManager");
        }

        if (MessageManager == null)
        {
            MessageManager = GameObject.Find("MessageManager");
        }
    }

    public bool CardSpell()
    {
        ResourceManager MainResourceManager = GameManager.GetComponent<GameController>().GetResourceManager();

        //1.获取瞄准的目标

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
        int PlayerMineral = MainResourceManager.GetPlayerMineral(Owner);
        int PlayerGas = MainResourceManager.GetPlayerGas(Owner);
        int PieceCostMineral = SpellMineralCost;
        int PieceCostGas = SpellGasCost;
        if (PieceCostMineral > PlayerMineral || PieceCostGas > PlayerGas)
        {
            // 显示一条提示资源不足的消息
            MessageManager.GetComponent<MessageController>().PrivateMessage("资源不足");
            // 卡牌施放失败返回false
            return false;
        }

        if(TargetSelect == 0)
        {
            if (ColliderGameObject != null && ColliderGameObject.GetComponent<GameObjectPropertyController>() != null && ColliderGameObject.GetComponent<GameObjectPropertyController>().IsPiece == true)
            {
                // 资源消耗更新
                PlayerMineral -= SpellMineralCost;
                PlayerGas -= SpellGasCost;
                MainResourceManager.SetPlayerMineral(Owner, PlayerMineral);
                MainResourceManager.SetPlayerGas(Owner, PlayerGas);

                for (int i = 0; i < SpellNum; i++) { RelativeEffect.ExcuteEffect(null, null, null, null, ColliderGameObject, null, null, null); }   
                
                return true;
            }
            else
            {
                MessageManager.GetComponent<MessageController>().PrivateMessage("必须要以棋子为目标");
                return false;
            }
        }
        else if(TargetSelect == 1)
        {
            if (ColliderGameObject != null && ColliderGameObject.GetComponent<GameObjectPropertyController>().IsCheckerBoard == true)
            {
                // 资源消耗更新
                PlayerMineral -= SpellMineralCost;
                PlayerGas -= SpellGasCost;
                MainResourceManager.SetPlayerMineral(Owner, PlayerMineral);
                MainResourceManager.SetPlayerGas(Owner, PlayerGas);

                for (int i = 0; i < SpellNum; i++) { RelativeEffect.ExcuteEffect(null, null, null, null, null, ColliderGameObject, null, null); }                    
                return true;
            }
            else
            {
                MessageManager.GetComponent<MessageController>().PrivateMessage("必须要以棋盘格为目标");
                return false;
            }
        }
        else
        {
            if (ColliderGameObject != null && ColliderGameObject.GetComponent<GameObjectPropertyController>() != null && ColliderGameObject.GetComponent<GameObjectPropertyController>().IsPiece == true)
            {
                // 资源消耗更新
                PlayerMineral -= SpellMineralCost;
                PlayerGas -= SpellGasCost;
                MainResourceManager.SetPlayerMineral(Owner, PlayerMineral);
                MainResourceManager.SetPlayerGas(Owner, PlayerGas);

                for (int i = 0; i < SpellNum; i++) { RelativeEffect.ExcuteEffect(null, null, null, null, ColliderGameObject, null, null, null); }                    
                return true;
            }
            else if (ColliderGameObject != null && ColliderGameObject.GetComponent<GameObjectPropertyController>().IsCheckerBoard == true)
            {
                // 资源消耗更新
                PlayerMineral -= SpellMineralCost;
                PlayerGas -= SpellGasCost;
                MainResourceManager.SetPlayerMineral(Owner, PlayerMineral);
                MainResourceManager.SetPlayerGas(Owner, PlayerGas);

                for (int i = 0; i < SpellNum; i++) { RelativeEffect.ExcuteEffect(null, null, null, null, null, ColliderGameObject, null, null); }                    
                return true;
            }
            else
            {
                MessageManager.GetComponent<MessageController>().PrivateMessage("必须要以棋子或者棋盘格为目标");
                return false;
            }
        }
    }

    public void SetOwner(int NewOwnerID)
    {
        Owner = NewOwnerID;
    }

    public int GetOwner()
    {
        return Owner;
    }

    public int GetSpellMineralCost()
    {
        return SpellMineralCost;
    }

    public int GetSpellGasCost()
    {
        return SpellGasCost;
    }

    public int GetSpellEnergyCost()
    {
        return SpellEnergyCost;
    }

    public void SetSpellMineralCost(int NewSpellMineralCost)
    {
        SpellMineralCost = NewSpellMineralCost;
    }

    public void SetSpellGasCost(int NewSpellGasCost)
    {
        SpellGasCost = NewSpellGasCost;
    }

    public void SetSpellEnergyCost(int NewSpellEnergyCost)
    {
        SpellEnergyCost = NewSpellEnergyCost;
    }
}
