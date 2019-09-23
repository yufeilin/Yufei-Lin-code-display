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

	// 卡牌在游戏开始时触发的效果
	string GameStartEffectID = "";
	Effect* GameStartEffect = NULL;

	// 卡牌被抽取时触发的效果
	string CardDrowToHandEffectID = "";
	Effect* CardDrowToHandEffect = NULL;

	// 卡牌进入墓地时（随从死亡，法术释放完毕，装备耐久为0）触发的效果
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
	// 随从卡关联的棋子实例
	string MinionPieceInstanceID = "";
	PieceInstance* MinionPieceInstance = NULL;

	// 棋子召唤效果（通常使用默认的随从卡召唤效果）
	string SummonPieceInstanceEffectID = "";
	Effect* SummonPieceInstanceEffect = NULL;

	// 棋子召唤后效果（可以用来链接战吼效果）
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
	//1.执行卡牌进入坟场效果

	//2.通知卡牌管理器将卡牌移入坟场
}



#endif