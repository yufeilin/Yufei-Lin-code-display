#include "Card.h"

extern LibraryManager* MainLibraryManager;

Card * MinionCard::GetCopy()
{
	MinionCard* NewMinionCard = new MinionCard();

	NewMinionCard->SetOwner(this->GetOwner());

	NewMinionCard->GetParameterDictionary().SetStringParameter("CardID", this->GetParameterDictionary().GetStringParameter("CardID"));
	NewMinionCard->GetParameterDictionary().SetStringParameter("CardName", this->GetParameterDictionary().GetStringParameter("CardName"));

	NewMinionCard->GameStartEffectID = this->GameStartEffectID;
	MainLibraryManager->GetEffect(this->GameStartEffectID);

	NewMinionCard->GameStartEffect = this->GameStartEffect;

	NewMinionCard->CardDrowToHandEffectID = this->CardDrowToHandEffectID;
	NewMinionCard->CardDrowToHandEffect = this->CardDrowToHandEffect;

	NewMinionCard->CardEnterGraveEffectID = this->CardEnterGraveEffectID;
	NewMinionCard->CardEnterGraveEffect = this->CardEnterGraveEffect;

	// ������һ������ʵ��Ȼ��������

	return NewMinionCard;
}