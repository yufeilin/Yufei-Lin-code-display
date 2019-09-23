#ifndef _UNIT
#define _UNIT

#include"YufeiBasic.h"
#include"Project28Basic.h"

class PieceInstance;
class Weapon;

class Unit
{
public:
	Unit();
	~Unit();
	void LoadInfoFromXML(XMLElement* XMLEffectInfo);
	ParameterDictionary& GetParameterDictionary() { return UnitParameterDictionary; }

	inline void SetUnitID(string NewUnitID) { UnitParameterDictionary.SetStringParameter("UnitID", NewUnitID); }
	inline string GetUnitID(){ return UnitParameterDictionary.GetStringParameter("UnitID"); }
	inline void SetUnitName(string NewUnitName){ UnitParameterDictionary.SetStringParameter("UnitName", NewUnitName); }
	inline string GetUnitName(){ return UnitParameterDictionary.GetStringParameter("UnitName"); }

	inline int GetUnitOwner(){ return UnitOwner; }
	inline void SetUnitOwner(int NewUnitOwner){ UnitOwner = NewUnitOwner; }

	inline int GetMaxHealth(){ return UnitParameterDictionary.GetIntParameter("MaxHealth"); }
	void SetMaxHealth(int NewMaxHealth);
	inline int GetCurrentHealth() { return UnitParameterDictionary.GetIntParameter("CurrentHealth"); }
	void SetCurrentHealth(int NewCurrentHealth);

	inline float GetUnitSize(){ return UnitSize; }
	void SetUnitSize(float NewSize);

	inline float GetMobilityFactor(){ return MobilityFactor;}
	void SetMobilityFactor(float NewMobilityFactor);

	inline bool GetAliveState(){ return IsAlive;}
	inline void SetAliveState(bool NewAliveState);
	inline bool GetAttakableState(){ return IsAttackableInBattle; }
	inline void SetAttackableState(bool NewState){ IsAttackableInBattle = NewState; }
	inline bool GetHasAttackState(){ return HasAttackInBattle; }
	inline void SetHasAttackState(bool NewState){ HasAttackInBattle = NewState; }
	inline int GetAttackCount(){ return AttackCountInBattle; }
	inline void SetAttackCount(int NewCount){ AttackCountInBattle = NewCount; }

	inline int GetWeaponNum(){ return UnitWeapon.Count; }	
	inline Weapon& GetWeapon(int index){ return *(UnitWeapon[index]); }
	inline void AddWeapon(Weapon* NewWeapon){ UnitWeapon.Add(NewWeapon); }

	inline void SetLocatePiece(PieceInstance* NewLocatePiece){ LocatePiece = NewLocatePiece; }
	inline PieceInstance* GetLocatePiece(){ return LocatePiece; }

	void UnitStateUpdateBeforeBattle()
	{
		InBattle = true;

		// 此更新函数中不检查棋子的存活状态
		// 遍历行为list检查是否存在战斗结束后就应该移除的行为并且移除这些行为
	}

	void UnitStateUpdateAfterBattle()
	{
		InBattle = false;

		// 此更新函数中不检查棋子的存活状态
		// 遍历行为list检查是否存在战斗结束后就应该移除的行为并且移除这些行为
	}

	void UnitStateUpdateBeforeTurn()
	{
		// 遍历行为list检查是否存在玩家回合开始前就应该移除的行为并且移除这些行为
	}

	void UnitStateUpdateAfterTurn()
	{
		// 遍历行为list检查是否存在玩家回合结束后就应该移除的行为并且移除这些行为
	}

private:
	ParameterDictionary UnitParameterDictionary;

	// 单位所有者
	int UnitOwner = -1;

	// 单位尺寸
	float UnitSize = 1.0f;

	// 单位机动因子
	float MobilityFactor = 2.0f;

	// 是否能被对地武器攻击标旗
	bool IsGroundTarget = true;

	// 是否能被对空武器攻击标旗
	bool IsAirTarget = false;

	// 单位是否存活
	// IsAlive = true 当前单位HP > 0
	// IsAlive = false 当前单位已经死亡
	bool IsAlive = true;

	// 单位所在的棋子
	PieceInstance* LocatePiece = NULL;

	//-------------------------------------------
	// 在回合战斗结算中是否能够被攻击
	bool IsAttackableInBattle = false;

	// 在回合战斗中是否已经攻击过
	bool HasAttackInBattle = false;

	// 在回合战斗中攻击的次数
	int AttackCountInBattle = 0;

	// 是否处于战斗当中
	bool InBattle = false;
	//-------------------------------------------

	List<Weapon*> UnitWeapon;

	// 单位武器ID列表
	List<string> UnitWeaponID;

};

Unit::Unit()
{
	// 字符串类参数
	UnitParameterDictionary.Add("UnitID", string());
	UnitParameterDictionary.Add("UnitName", string());

	// 数值类参数
	UnitParameterDictionary.Add("MaxHealth", 1);
	UnitParameterDictionary.Add("CurrentHealth", 1);
	UnitParameterDictionary.Add("MaxEnergy", 0);
	UnitParameterDictionary.Add("CurrentEnergy", 0);
	UnitParameterDictionary.Add("MaxShield", 0);
	UnitParameterDictionary.Add("CurrentShield", 0);

	// 标旗类参数
	UnitParameterDictionary.Add("IsGroundTarget", true);
	UnitParameterDictionary.Add("IsAirTarget", false);
}

Unit::~Unit()
{
}

void Unit::LoadInfoFromXML(XMLElement * XMLUnitInfo)
{
	for (XMLElement* XMLUnitInfoItem = XMLUnitInfo->FirstChildElement(); XMLUnitInfoItem != NULL; XMLUnitInfoItem = XMLUnitInfoItem->NextSiblingElement())
	{
		if (strcmp(XMLUnitInfoItem->Value(), "UnitID") == 0)
		{
			// 从XML中读入单位ID
			UnitParameterDictionary.SetStringParameter("UnitID", XMLUnitInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLUnitInfoItem->Value(), "UnitName") == 0)
		{
			// 从XML中读入单位名称
			UnitParameterDictionary.SetStringParameter("UnitName", XMLUnitInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLUnitInfoItem->Value(), "UnitMaxHealth") == 0)
		{
			// 从XML中读入单位最大生命值
			UnitParameterDictionary.Set("MaxHealth", stoi(XMLUnitInfoItem->FirstChild()->ToText()->Value()));
		}
		else if (strcmp(XMLUnitInfoItem->Value(), "UnitSize") == 0)
		{
			// 从XML中读入单位尺寸
			UnitSize = stoi(XMLUnitInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLUnitInfoItem->Value(), "UnitWeaponList") == 0)
		{
			// 从XML中读入单位武器列表
			for (XMLElement* XMLUnitWeaponItem = XMLUnitInfoItem->FirstChildElement(); XMLUnitWeaponItem != NULL; XMLUnitWeaponItem = XMLUnitWeaponItem->NextSiblingElement())
			{
				if (strcmp(XMLUnitWeaponItem->Value(), "WeaponID") == 0)
				{
					// 从XML中读入单位武器项
					UnitWeaponID.Add(XMLUnitWeaponItem->FirstChild()->ToText()->Value());
				}
			}
		}

	}
}

inline void Unit::SetMaxHealth(int NewMaxHealth)
{
	if (NewMaxHealth > 0) UnitParameterDictionary.SetIntParameter("MaxHealth", NewMaxHealth);
	else abort();
}

inline void Unit::SetCurrentHealth(int NewCurrentHealth)
{
	int MaxHealth = GetMaxHealth();
	if (NewCurrentHealth > MaxHealth) UnitParameterDictionary.SetIntParameter("CurrentHealth", UnitParameterDictionary.GetIntParameter("MaxHealth"));
	else UnitParameterDictionary.SetIntParameter("CurrentHealth", NewCurrentHealth);
}

inline void Unit::SetUnitSize(float NewSize)
{	
	if (NewSize >= 0.0f) UnitSize = NewSize;
	else abort();
}

inline void Unit::SetMobilityFactor(float NewMobilityFactor)
{
	if(NewMobilityFactor >= 0.0f) MobilityFactor = NewMobilityFactor;
	else abort();
}

inline void Unit::SetAliveState(bool NewAliveState)
{
	if (IsAlive != NewAliveState)
	{
		if (NewAliveState == false)
		{
			// 设置新存活状态为false
			// 这个分支对应效果直接将单位杀死，不会触发伤害响应但是会触发亡语
			UnitParameterDictionary.SetIntParameter("CurrentHealth", 0);
			IsAlive = false;
		}
		else
		{
			// 设置新存活状态为true
			// 这个分支对应复活效果，被复活的单位默认只有1点生命值
			UnitParameterDictionary.SetIntParameter("CurrentHealth", 1);
			IsAlive = true;
		}
	}
}

#endif