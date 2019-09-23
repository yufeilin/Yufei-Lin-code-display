#ifndef _WEAPON
#define _WEAPON

#include"YufeiBasic.h"
#include"Project28Basic.h"

#include "tinyxml2\tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Range;

class Weapon :public OperationBase
{
public:
	Weapon(){}
	Weapon(XMLElement* XMLWeaponInfo);
	~Weapon(){}
	inline string GetWeaponID() { return WeaponID; }
	inline void SetWeaponID(string NewWeaponID) { WeaponID = NewWeaponID; }
	inline string GetWeaponName(){ return WeaponName; }
	inline void SetWeaponName(string NewWeaponName){ WeaponName = NewWeaponName; }
	inline float GetWeaponInterRange(){ return WeaponInterRange; }
	inline void SetWeaponInterRange(float NewWeaponInterRange){ WeaponInterRange = NewWeaponInterRange; }
	inline Range& GetWeaponOuterRange() { return WeaponOuterRange; }
	inline void SetWeaponOuterRange(Range* NewWeaponOuterRange) { WeaponOuterRange = *NewWeaponOuterRange; }
	inline float GetAttackInterval(){ return WeaponAttackInterval; }
	void SetAttackInterval(float NewInterval){ WeaponAttackInterval = NewInterval; }
	inline Effect& GetWeaponAttackEffect(){ return *WeaponEffect; }
	inline void SetWeaponAttackEffect(Effect* NewWeaponAttackEffect){ WeaponEffect = NewWeaponAttackEffect; }
	bool ValidTargetUnit(Unit TargetUnit)
	{
		// TODO: 根据武器单位筛选标旗来判断潜在攻击目标是否为合法攻击目标
		return true;
	}

private:
	// 武器ID(每个武器都不相同)
	string WeaponID = "";

	// 武器名称(可以重复)
	string WeaponName = "";

	// 武器内射程(战斗射程)
	float WeaponInterRange = 1.0f;

	// 武器外射程(棋盘格射程)
	Range WeaponOuterRange;

	// 武器开火间隔
	float WeaponAttackInterval = 1.0f;

	// 武器开火效果
	Effect* WeaponEffect = NULL;

	// 武器开火效果ID
	string WeaponEffectID;

	// 武器单次开火单位锁定数目
	int TargetNum = 1;

	// 武器开火次数
	int MultipleAttacks = 1;

	// 武器的单位类型选择器
	//UnitTypeSelector WeaponTargetUnitTypeSelector = new UnitTypeSelector();

	// 武器的单位选择器
	//UnitSelector WeaponTargetUnitSelector = new UnitSelector();


};

Weapon::Weapon(XMLElement* XMLWeaponInfo)
{
	for (XMLElement* XMLWeaponInfoItem = XMLWeaponInfo->FirstChildElement(); XMLWeaponInfoItem != NULL; XMLWeaponInfoItem = XMLWeaponInfoItem->NextSiblingElement())
	{
		if (strcmp(XMLWeaponInfoItem->Value(), "WeaponID") == 0)
		{
			// 从XML中读入武器ID
			WeaponID = XMLWeaponInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLWeaponInfoItem->Value(), "WeaponName") == 0)
		{
			// 从XML中读入武器名称
			WeaponName = XMLWeaponInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLWeaponInfoItem->Value(), "WeaponAttackInterval") == 0)
		{
			// 从XML中读入武器开火间隔
			WeaponAttackInterval = atof(XMLWeaponInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLWeaponInfoItem->Value(), "WeaponAttackEffect") == 0)
		{
			// 从XML中读入武器开火效果ID
			WeaponEffectID = XMLWeaponInfoItem->FirstChild()->ToText()->Value();
		}
	}
}

#endif
