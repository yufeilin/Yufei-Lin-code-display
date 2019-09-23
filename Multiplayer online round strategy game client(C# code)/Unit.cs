using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Unit
{
    Dictionary<string, Parameter> ParameterOfName = new Dictionary<string, Parameter>();

    //// 单位ID(每个单位各不相同)
    //string UnitID = "";
    //
    //// 单位名称(可以重复)
    //string UnitName = "";
    //
    //// 单位最大生命值
    //int MaxHealth = 1;
    //
    //// 单位当前生命值
    //int CurrentHealth = 1;

    // 单位所有者
    int UnitOwner = -1;

    // 单位尺寸
    float UnitSize = 1.0f;

    // 单位机动因子
    float MobilityFactor = 2.0f;

    // 是否能被对地武器攻击标旗
    //bool IsGroundTarget = true;

    // 是否能被对空武器攻击标旗
    //bool IsAirTarget = false;

    public float HP = 1.0f;

    // 单位是否存活
    // IsAlive = true 当前单位HP > 0
    // IsAlive = false 当前单位已经死亡
    bool IsAlive = true;

    // 单位所在的棋子
    GameObject LocatePiece = null;

    //-------------------------------------------
    // 在回合战斗结算中是否能够被攻击
    bool IsAttackableInBattle = false;

    // 在回合战斗中是否已经攻击过
    bool HasAttackInBattle = false;

    // 在回合战斗中攻击的次数
    int AttackCountInBattle = 0;

    bool InBattle = false;
    //-------------------------------------------

    List<Weapon> UnitWeapon = new List<Weapon>();

    int WeaponNum = 0;

    public System.Type DeriveClassType = typeof(Unit);

    // 属性维护
    // ======================================================================
    public Unit()
    {
        ParameterOfName.Add("UnitID", new StringParameter());
        ParameterOfName.Add("UnitName", new StringParameter());
        ParameterOfName.Add("MaxHealth", new IntParameter());
        ParameterOfName.Add("CurrentHealth", new IntParameter());
        ParameterOfName.Add("MaxEnergy", new IntParameter());
        ParameterOfName.Add("CurrentEnergy", new IntParameter());
        ParameterOfName.Add("MaxShield", new IntParameter());
        ParameterOfName.Add("CurrentShield", new IntParameter());

        // 标旗类参数声明
        ParameterOfName.Add("IsGroundTarget", new BoolParameter());
        ParameterOfName.Add("IsAirTarget", new BoolParameter());

        ParameterOfName["UnitID"].SetParameter("");
        ParameterOfName["UnitName"].SetParameter("");
        ParameterOfName["MaxHealth"].SetParameter("1");
        ParameterOfName["CurrentHealth"].SetParameter("1");
        ParameterOfName["MaxEnergy"].SetParameter("0");
        ParameterOfName["CurrentEnergy"].SetParameter("0");
        ParameterOfName["MaxShield"].SetParameter("0");
        ParameterOfName["CurrentShield"].SetParameter("0");

        // 标旗类参数初始化
        ParameterOfName["IsGroundTarget"].SetParameter("true");
        ParameterOfName["IsAirTarget"].SetParameter("false");
    }

    // 获取参数对象的引用(专门为行为behavior系统使用)
    public Parameter GetParameterObject(string ParaName)
    {
        return ParameterOfName[ParaName];
    }

    // 获取参数对象内在的参数值
    public object GetParameter(string ParaName)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {
            return ParameterOfName[ParaName].GetParameter();
        }
        else
        {
            Debug.Log("Cannot Find Parameters");
            return null;
        }
    }

    public void SetParameter(string ParaName, string ParaValue)
    {
        if (ParameterOfName.ContainsKey(ParaName) == true)
        {



            ParameterOfName[ParaName].SetParameter(ParaValue);
        }
        else
        {
            Debug.Log("Cannot Find Parameters");
        }
    }
    // ======================================================================

    // 从一个单位模板ParaUnit实例化当前单位
    public void InitialUnit(Unit ParaUnit)
    {
        SetParameter("UnitID", (string)ParaUnit.GetParameter("UnitID"));
        //UnitID = ParaUnit.GetUnitID();
        SetParameter("UnitName", (string)ParaUnit.GetParameter("UnitName"));
        //UnitName = ParaUnit.GetUnitName();
        SetParameter("MaxHealth", ((int)ParaUnit.GetParameter("MaxHealth")).ToString());
        //MaxHealth = ParaUnit.GetMaxHealth();
        SetParameter("CurrentHealth", ((int)GetParameter("MaxHealth")).ToString());
        //CurrentHealth = MaxHealth;
        UnitSize = ParaUnit.GetUnitSize();
        IsAlive = true;
        WeaponNum = ParaUnit.GetWeaponNum();
        for (int i=0;i< WeaponNum;i++)
        {
            UnitWeapon.Add(ParaUnit.GetWeapon(i));
        }
    }

    public List<Action> GetActions()
    {
        List<Action> Res = new List<Action>();

        // 遍历该单位的每一个武器并将每种武器转化为一个Action实例
        for(int i=0;i< UnitWeapon.Count; i++)
        {
            //Debug.Log("&&&&&" + UnitWeapon[i].GetWeaponID());
            Action CurrentWeaponAction = UnitWeapon[i].GetAction();
            //Debug.Log("&&&&&" + CurrentWeaponAction.GetActionRange());
            CurrentWeaponAction.SetExcuteUnit(this);

            //Debug.Log("<<" + UnitOwner + ">>");

            CurrentWeaponAction.SetUnitID((string)GetParameter("UnitID"));
            //CurrentWeaponAction.SetUnitName(UnitName);
            CurrentWeaponAction.SetRelativeBattleOperation(UnitWeapon[i]);
            Res.Add(CurrentWeaponAction);
        }

        // 遍历该单位的每一个攻击技能并将每个攻击技能转化为一个Action实例

        // 遍历该单位的每一个辅助技能并将每个辅助技能转化为一个Action实例

        return Res;
    }

    // 计算该单位的战斗射程
    // 战斗射程的计算方法为武器，攻击技能，辅助技能射程根据各自冷却时间的加权平均值
    public float CalculateUnitRange()
    {
        // TODO: 攻击技能 和 辅助技能 的计算部分需要实现
        float Res = 0.0f;

        // 武器，攻击技能，辅助技能在单位时间内使用次数总和
        float UseTimeSum = 0.0f;

        // 计算武器，攻击技能，辅助技能在单位时间内使用次数总和
        for (int i=0;i< UnitWeapon.Count; i++)
        {
            UseTimeSum += 1.0f / UnitWeapon[i].GetAttackInterval();
        }

        // 某个技能或者武器的使用次数与使用次数总和的比值与该技能或者武器的冷却时间相乘
        // 将所有武器和技能的该乘积加和即为该单位的战斗平均射程
        for (int i = 0; i < UnitWeapon.Count; i++)
        {
            Res += ((1.0f / UnitWeapon[i].GetAttackInterval()) / UseTimeSum) * UnitWeapon[i].GetAttackInterval();               
        }

        return Res;
    }

    public void SetUnitID(string NewUnitID)
    {
        //UnitID = NewUnitID;
        SetParameter("UnitID", NewUnitID);
    }
    
    public string GetUnitID()
    {
        return (string)GetParameter("UnitID");
        //return UnitID;
    }
    
    public void SetUnitName(string NewUnitName)
    {
        //UnitName = NewUnitName;
        SetParameter("UnitName", NewUnitName);
    }
    
    public string GetUnitName()
    {
        return (string)GetParameter("UnitName");
        //return UnitName;
    }

    public int GetUnitOwner()
    {
        return UnitOwner;
    }

    public void SetUnitOwner(int NewUnitOwner)
    {
        UnitOwner = NewUnitOwner;
    }
    
    public int GetMaxHealth()
    {
        return (int)GetParameter("MaxHealth");
        //return MaxHealth;
    }

    public float GetUnitSize()
    {
        return UnitSize;
    }

    public void SetUnitSize(float NewSize)
    {
        if(NewSize >= 0.0f)
        {
            UnitSize = NewSize;
        }
    }

    public float GetMobilityFactor()
    {
        return MobilityFactor;
    }

    public void SetMobilityFactor(float NewMobilityFactor)
    {
        MobilityFactor = NewMobilityFactor;
    }

    public bool GetAliveState()
    {
        return IsAlive;
    }

    public bool GetAttakableState()
    {
        return IsAttackableInBattle;
    }

    public void SetAttackableState(bool NewState)
    {
        IsAttackableInBattle = NewState;
    }

    public bool GetHasAttackState()
    {
        return HasAttackInBattle;
    }

    public void SetHasAttackState(bool NewState)
    {
        HasAttackInBattle = NewState;
    }

    public int GetAttackCount()
    {
        return AttackCountInBattle;
    }

    public void SetAttackCount(int NewCount)
    {
        AttackCountInBattle = NewCount;
    }

    public int GetWeaponNum()
    {
        return UnitWeapon.Count;
    }

    public Weapon GetWeapon(int index)
    {
        return UnitWeapon[index];
    }

    public void SetMaxHealth(int NewMaxHealth)
    {
        //MaxHealth = NewMaxHealth;
        SetParameter("MaxHealth", NewMaxHealth.ToString());
    }
    
    public int GetCurrentHealth()
    {
        return (int)GetParameter("CurrentHealth");
        //return CurrentHealth;

    }

    public void SetCurrentHealth(int NewCurrentHealth)
    {
        int MaxHealth = GetMaxHealth();
        if (NewCurrentHealth > MaxHealth)
        {
            SetParameter("CurrentHealth", MaxHealth.ToString());
            //CurrentHealth = MaxHealth;
        }
        else
        {
            //CurrentHealth = NewCurrentHealth;
            SetParameter("CurrentHealth", NewCurrentHealth.ToString());
        }
    }

    // 当前单位接收伤害
    public void ReceiveDamage(Unit AttackUnit, int DamageAmount)
    {
        // 伤害响应部分

        // 实际伤害结算
        int CurrentHealth = GetCurrentHealth();
        if (CurrentHealth - DamageAmount <= 0)
        {
            CurrentHealth = 0;
            IsAlive = false;

            // 每当一个棋子包含的单位不在存活就需要检查棋子是否还存活
            PieceStateController TmpPieceStateController = LocatePiece.GetComponentInChildren<PieceStateController>();

            // 如果棋子包含的存活单位为0则判定棋子不再存活
            if(TmpPieceStateController.GetAliveUnitNum() == 0)
            {
                Debug.Log("Piece is not alive.");

                // 调用棋子亡语函数
                TmpPieceStateController.PieceDead(AttackUnit);
            }

        }
        else
        {
            CurrentHealth -= DamageAmount;
        }

        SetCurrentHealth(CurrentHealth);
    }

    // 在回合战斗中执行一次攻击指令
    public void ExcuteAttack(Unit TargetUnit)
    {
        Debug.Log(UnitWeapon.Count + "+++++");

        if(UnitWeapon[0] == null)
        {
            Debug.Log("weapon null");
        }

        UnitWeapon[0].WeaponActivate(this, TargetUnit);

        // 如果在当前战斗中该单位为第一次攻击则设置战斗攻击状态变量为true
        if(HasAttackInBattle == false)
        {
            HasAttackInBattle = true;
        }

        // 战斗攻击次数计数器加一
        AttackCountInBattle++;
    }

    public void AddWeapon(Weapon NewWeapon)
    {
        //Debug.Log("WeaponLoad");
        UnitWeapon.Add(NewWeapon);
    }

    public void SetLocatePiece(GameObject ParaPiece)
    {
        LocatePiece = ParaPiece;
    }

    public void UnitStateUpdateBeforeBattle()
    {
        InBattle = true;

        // 此更新函数中不检查棋子的存活状态
        // 遍历行为list检查是否存在战斗结束后就应该移除的行为并且移除这些行为
    }

    public void UnitStateUpdateAfterBattle()
    {
        InBattle = false;

        // 此更新函数中不检查棋子的存活状态
        // 遍历行为list检查是否存在战斗结束后就应该移除的行为并且移除这些行为
    }

    public void UnitStateUpdateBeforeTurn()
    {
        // 遍历行为list检查是否存在玩家回合开始前就应该移除的行为并且移除这些行为
    }

    public void UnitStateUpdateAfterTurn()
    {
        // 遍历行为list检查是否存在玩家回合结束后就应该移除的行为并且移除这些行为
    }

    public GameObject GetLocatePiece()
    {
        return LocatePiece;
    }

}

