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

		// �˸��º����в�������ӵĴ��״̬
		// ������Ϊlist����Ƿ����ս���������Ӧ���Ƴ�����Ϊ�����Ƴ���Щ��Ϊ
	}

	void UnitStateUpdateAfterBattle()
	{
		InBattle = false;

		// �˸��º����в�������ӵĴ��״̬
		// ������Ϊlist����Ƿ����ս���������Ӧ���Ƴ�����Ϊ�����Ƴ���Щ��Ϊ
	}

	void UnitStateUpdateBeforeTurn()
	{
		// ������Ϊlist����Ƿ������һغϿ�ʼǰ��Ӧ���Ƴ�����Ϊ�����Ƴ���Щ��Ϊ
	}

	void UnitStateUpdateAfterTurn()
	{
		// ������Ϊlist����Ƿ������һغϽ������Ӧ���Ƴ�����Ϊ�����Ƴ���Щ��Ϊ
	}

private:
	ParameterDictionary UnitParameterDictionary;

	// ��λ������
	int UnitOwner = -1;

	// ��λ�ߴ�
	float UnitSize = 1.0f;

	// ��λ��������
	float MobilityFactor = 2.0f;

	// �Ƿ��ܱ��Ե�������������
	bool IsGroundTarget = true;

	// �Ƿ��ܱ��Կ�������������
	bool IsAirTarget = false;

	// ��λ�Ƿ���
	// IsAlive = true ��ǰ��λHP > 0
	// IsAlive = false ��ǰ��λ�Ѿ�����
	bool IsAlive = true;

	// ��λ���ڵ�����
	PieceInstance* LocatePiece = NULL;

	//-------------------------------------------
	// �ڻغ�ս���������Ƿ��ܹ�������
	bool IsAttackableInBattle = false;

	// �ڻغ�ս�����Ƿ��Ѿ�������
	bool HasAttackInBattle = false;

	// �ڻغ�ս���й����Ĵ���
	int AttackCountInBattle = 0;

	// �Ƿ���ս������
	bool InBattle = false;
	//-------------------------------------------

	List<Weapon*> UnitWeapon;

	// ��λ����ID�б�
	List<string> UnitWeaponID;

};

Unit::Unit()
{
	// �ַ��������
	UnitParameterDictionary.Add("UnitID", string());
	UnitParameterDictionary.Add("UnitName", string());

	// ��ֵ�����
	UnitParameterDictionary.Add("MaxHealth", 1);
	UnitParameterDictionary.Add("CurrentHealth", 1);
	UnitParameterDictionary.Add("MaxEnergy", 0);
	UnitParameterDictionary.Add("CurrentEnergy", 0);
	UnitParameterDictionary.Add("MaxShield", 0);
	UnitParameterDictionary.Add("CurrentShield", 0);

	// ���������
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
			// ��XML�ж��뵥λID
			UnitParameterDictionary.SetStringParameter("UnitID", XMLUnitInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLUnitInfoItem->Value(), "UnitName") == 0)
		{
			// ��XML�ж��뵥λ����
			UnitParameterDictionary.SetStringParameter("UnitName", XMLUnitInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLUnitInfoItem->Value(), "UnitMaxHealth") == 0)
		{
			// ��XML�ж��뵥λ�������ֵ
			UnitParameterDictionary.Set("MaxHealth", stoi(XMLUnitInfoItem->FirstChild()->ToText()->Value()));
		}
		else if (strcmp(XMLUnitInfoItem->Value(), "UnitSize") == 0)
		{
			// ��XML�ж��뵥λ�ߴ�
			UnitSize = stoi(XMLUnitInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLUnitInfoItem->Value(), "UnitWeaponList") == 0)
		{
			// ��XML�ж��뵥λ�����б�
			for (XMLElement* XMLUnitWeaponItem = XMLUnitInfoItem->FirstChildElement(); XMLUnitWeaponItem != NULL; XMLUnitWeaponItem = XMLUnitWeaponItem->NextSiblingElement())
			{
				if (strcmp(XMLUnitWeaponItem->Value(), "WeaponID") == 0)
				{
					// ��XML�ж��뵥λ������
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
			// �����´��״̬Ϊfalse
			// �����֧��ӦЧ��ֱ�ӽ���λɱ�������ᴥ���˺���Ӧ���ǻᴥ������
			UnitParameterDictionary.SetIntParameter("CurrentHealth", 0);
			IsAlive = false;
		}
		else
		{
			// �����´��״̬Ϊtrue
			// �����֧��Ӧ����Ч����������ĵ�λĬ��ֻ��1������ֵ
			UnitParameterDictionary.SetIntParameter("CurrentHealth", 1);
			IsAlive = true;
		}
	}
}

#endif