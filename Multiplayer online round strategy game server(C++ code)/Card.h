#ifndef _CARD
#define _CARD


#include "tinyxml2\tinyxml2.h"
#include "Global.h"

using namespace tinyxml2;

class LibraryManager;



class Card
{
public:
	Card();
	~Card();
	virtual Card* ReloadNewCard() { return new Card(); }
	virtual void LoadInfoFromXML(XMLElement* XMLCardInfo) {}
	virtual bool CardcCostCheck() { return false; }
	virtual bool CardSpell() { return false; }
	virtual void CardEnterGrave() {}
	virtual Card* GetCopy() { return new Card(); }
	virtual void SetOwner(int NewOwnerID) { Owner = NewOwnerID; }
	int GetOwner() { return Owner; }

	ParameterDictionary& GetParameterDictionary() { return EffectParameterDictionary; }

	// ��������Ϸ��ʼʱ������Ч��
	string GameStartEffectID = "";
	Effect* GameStartEffect = NULL;

	// ���Ʊ���ȡʱ������Ч��
	string CardDrowToHandEffectID = "";
	Effect* CardDrowToHandEffect = NULL;

	// ���ƽ���Ĺ��ʱ����������������ͷ���ϣ�װ���;�Ϊ0��������Ч��
	string CardEnterGraveEffectID = "";
	Effect* CardEnterGraveEffect = NULL;

private:
	int Owner = -1;
	ParameterDictionary EffectParameterDictionary;



};

Card::Card()
{
	GetParameterDictionary().Add("CardID", string());
	GetParameterDictionary().Add("CardName", string());
}

Card::~Card()
{
}

class MinionCard :public Card
{
public:
	MinionCard();
	~MinionCard();
	Card* ReloadNewCard()override { return new MinionCard(); }
	void LoadInfoFromXML(XMLElement* XMLCardInfo)override;
	inline PieceInstance& GetRelativePieceInstance() { return *MinionPieceInstance; }
	inline void SetMinionPieceInstance(PieceInstance* NewPieceInstance) { MinionPieceInstance = NewPieceInstance; }
	bool CardSpell()override;
	void CardEnterGrave()override;
	Card* GetCopy()override;

private:
	// ��ӿ�����������ʵ��
	string MinionPieceInstanceID = "";
	PieceInstance* MinionPieceInstance = NULL;

	// �����ٻ�Ч����ͨ��ʹ��Ĭ�ϵ���ӿ��ٻ�Ч����
	string SummonPieceInstanceEffectID = "";
	Effect* SummonPieceInstanceEffect = NULL;

	// �����ٻ���Ч����������������ս��Ч����
	string AfterSummonPieceInstanceEffectID = "";
	Effect* AfterSummonPieceInstanceEffect = NULL;


};

MinionCard::MinionCard()
{
}

MinionCard::~MinionCard()
{
}

void MinionCard::LoadInfoFromXML(XMLElement * XMLCardInfo)
{

}

inline bool MinionCard::CardSpell()
{
	return false;
}

inline void MinionCard::CardEnterGrave()
{
	//1.ִ�п��ƽ���س�Ч��

	//2.֪ͨ���ƹ���������������س�
}



#endif