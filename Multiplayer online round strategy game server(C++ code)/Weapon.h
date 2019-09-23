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
		// TODO: ����������λɸѡ�������ж�Ǳ�ڹ���Ŀ���Ƿ�Ϊ�Ϸ�����Ŀ��
		return true;
	}

private:
	// ����ID(ÿ������������ͬ)
	string WeaponID = "";

	// ��������(�����ظ�)
	string WeaponName = "";

	// ���������(ս�����)
	float WeaponInterRange = 1.0f;

	// ���������(���̸����)
	Range WeaponOuterRange;

	// ����������
	float WeaponAttackInterval = 1.0f;

	// ��������Ч��
	Effect* WeaponEffect = NULL;

	// ��������Ч��ID
	string WeaponEffectID;

	// �������ο���λ������Ŀ
	int TargetNum = 1;

	// �����������
	int MultipleAttacks = 1;

	// �����ĵ�λ����ѡ����
	//UnitTypeSelector WeaponTargetUnitTypeSelector = new UnitTypeSelector();

	// �����ĵ�λѡ����
	//UnitSelector WeaponTargetUnitSelector = new UnitSelector();


};

Weapon::Weapon(XMLElement* XMLWeaponInfo)
{
	for (XMLElement* XMLWeaponInfoItem = XMLWeaponInfo->FirstChildElement(); XMLWeaponInfoItem != NULL; XMLWeaponInfoItem = XMLWeaponInfoItem->NextSiblingElement())
	{
		if (strcmp(XMLWeaponInfoItem->Value(), "WeaponID") == 0)
		{
			// ��XML�ж�������ID
			WeaponID = XMLWeaponInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLWeaponInfoItem->Value(), "WeaponName") == 0)
		{
			// ��XML�ж�����������
			WeaponName = XMLWeaponInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLWeaponInfoItem->Value(), "WeaponAttackInterval") == 0)
		{
			// ��XML�ж�������������
			WeaponAttackInterval = atof(XMLWeaponInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLWeaponInfoItem->Value(), "WeaponAttackEffect") == 0)
		{
			// ��XML�ж�����������Ч��ID
			WeaponEffectID = XMLWeaponInfoItem->FirstChild()->ToText()->Value();
		}
	}
}

#endif
