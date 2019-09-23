#ifndef _LIBRARYMANAGER
#define _LIBRARYMANAGER

#include<iostream>

#include "tinyxml2\tinyxml2.h"

#include "YufeiBasic.h"
#include "Project28Basic.h"

//#include"PiecePedestalInfo.h"
//#include"PieceStatusInfo.h"

using namespace tinyxml2;

using namespace std;

class Card;
class ChessBoard;
class Effect;

class Unit;
class Range;

class PiecePedestalInfo;
class PieceStatusInfo;

class EffectLoader;

class LibraryManager
{
public:
	LibraryManager();
	~LibraryManager();
	void LoadPiecePedestalInfoFromXML();
	void LoadPieceStatusInfoFromXML();
	void LoadEffectFromXML();
	void LoadWeaponFromXML();
	void LoadUnitFromXML();
	void LoadPieceFromXML();
	void LoadCardFromXML();
	inline PiecePedestalInfo& GetPiecePedestalInfo(string PiecePedestalInfoID) { return *(PiecePedestalInfoLibrary[PiecePedestalInfoID]); }
	inline PieceStatusInfo& GetPieceStatusInfo(string PieceStatusInfoID) { return *(PieceStatusInfoLibrary[PieceStatusInfoID]); }
	inline Effect& GetEffect(string EffectID) { return *(EffectLibrary[EffectID]); }
	inline Weapon& GetWeapon(string WeaponID) { return *(WeaponLibrary[WeaponID]); }
	inline Unit& GetUnit(string UnitID) { return *(UnitLibrary[UnitID]); }
	inline PieceInstance& GetPiece(string PieceID) { return *(PieceLibrary[PieceID]); }
	inline Card& GetCard(string CardID) { return *(CardLibrary[CardID]); }
	void UnloadPiecePedestalInfo();
	void UnloadPieceStatusInfo();
	void UnloadEffect();
	void UnloadWeapon();
	void UnloadUnit();
	void UnloadPiece();
	void UnloadCard();

private:
	// ���ӵ�����Ϣ��
	Dictionary<string, PiecePedestalInfo*> PiecePedestalInfoLibrary;

	// ���ӵ�����Ϣ��
	Dictionary<string, PieceStatusInfo*> PieceStatusInfoLibrary;

	// ��ϷЧ����
	Dictionary<string, Effect*> EffectLibrary;

	Dictionary<string, Weapon*> WeaponLibrary;

	Dictionary<string, Unit*> UnitLibrary;

	Dictionary<string, PieceInstance*> PieceLibrary;

	Dictionary<string, Card*> CardLibrary;

	//Unit ttt;
};

LibraryManager::LibraryManager()
{
	LoadPiecePedestalInfoFromXML();
	LoadPieceStatusInfoFromXML();
	LoadEffectFromXML();
	LoadWeaponFromXML();
	LoadUnitFromXML();
	LoadPieceFromXML();
}

LibraryManager::~LibraryManager()
{
	UnloadPiecePedestalInfo();
	UnloadPieceStatusInfo();
	UnloadEffect();
	UnloadWeapon();
	UnloadUnit();
	UnloadPiece();
}

void LibraryManager::LoadPiecePedestalInfoFromXML()
{
	tinyxml2::XMLDocument doc;

	doc.LoadFile("XML/Pedestal.xml");

	XMLElement* root = doc.FirstChildElement();

	if (root == NULL) { cout << "Root Node Is Null." << endl; return; }

	PiecePedestalInfo* NewPiecePedestalInfo = NULL;

	for (tinyxml2::XMLElement* XMLPiecePedestalInfo = root->FirstChildElement(); XMLPiecePedestalInfo != NULL; XMLPiecePedestalInfo = XMLPiecePedestalInfo->NextSiblingElement())
	{
		NewPiecePedestalInfo = new PiecePedestalInfo(XMLPiecePedestalInfo);
		PiecePedestalInfoLibrary.Add(NewPiecePedestalInfo->GetPiecePedestalID(), NewPiecePedestalInfo);
		NewPiecePedestalInfo = NULL;
	}
}

void LibraryManager::LoadPieceStatusInfoFromXML()
{
	tinyxml2::XMLDocument doc;
	// ���Լ���XML�ļ�(TODO: ����ļ������쳣���������»�ȡ��XML)
	doc.LoadFile("XML/Status.xml");
	// ��ȡXML�ļ����ڵ�
	XMLElement* root = doc.FirstChildElement();
	// ���XML���ڵ�(TODO: ����ļ����ڵ��쳣���������»�ȡ��XML)
	if (root == NULL) { cout << "Root Node Is Null." << endl; return; }

	PieceStatusInfo* NewPieceStatusInfo = NULL;

	for (tinyxml2::XMLElement* XMLPieceStatusInfo = root->FirstChildElement(); XMLPieceStatusInfo != NULL; XMLPieceStatusInfo = XMLPieceStatusInfo->NextSiblingElement())
	{
		// ͨ��XML�ڵ��ʼ��һ�����ӵ�����Ϣʵ��
		NewPieceStatusInfo = new PieceStatusInfo(XMLPieceStatusInfo);
		// �������ɵ����ӵ�����Ϣʵ���������ӵ�����Ϣ�ֵ���
		PieceStatusInfoLibrary.Add(NewPieceStatusInfo->GetPieceStatusID(), NewPieceStatusInfo);
		// ����ʱָ���ÿ�
		NewPieceStatusInfo = NULL;
	}
}

void LibraryManager::LoadEffectFromXML()
{
	tinyxml2::XMLDocument doc;
	// ���Լ���XML�ļ�(TODO: ����ļ������쳣���������»�ȡ��XML)
	doc.LoadFile("XML/Effect.xml");
	// ��ȡXML�ļ����ڵ�
	XMLElement* root = doc.FirstChildElement();
	// ���XML���ڵ�(TODO: ����ļ����ڵ��쳣���������»�ȡ��XML)
	if (root == NULL) { cout << "Root Node Is Null." << endl; return; }

	// �½�Ч��װ����
	EffectLoader* TmpEffectLoader = new EffectLoader();

	// �½�Ч������ָ��
	Effect* NewEffect = NULL;

	for (tinyxml2::XMLElement* XMLEffectInfo = root->FirstChildElement(); XMLEffectInfo != NULL; XMLEffectInfo = XMLEffectInfo->NextSiblingElement())
	{
		for (XMLElement* XMLEffectInfoItem = XMLEffectInfo->FirstChildElement(); XMLEffectInfoItem != NULL; XMLEffectInfoItem = XMLEffectInfoItem->NextSiblingElement())
		{
			if (strcmp(XMLEffectInfoItem->Value(), "EffectType") == 0)
			{
				// �ȶ�ȡЧ�����Ͳ������������ɶ�Ӧ��Ч��ʵ��
				NewEffect = TmpEffectLoader->GetNewEffectByType(XMLEffectInfoItem->FirstChild()->ToText()->Value());
			}
			else if (strcmp(XMLEffectInfoItem->Value(), "EffectSubInfo") == 0)
			{
				// ��XML�ڵ㴫�ݸ�����Ч������г�ʼ����ֵ
				NewEffect->LoadInfoFromXML(XMLEffectInfoItem);
			}			
		}
		// ����ʼ����ϵ�����Ч����ӵ�Ч���ֵ���
		EffectLibrary.Add(NewEffect->GetParameterDictionary().GetStringParameter("EffectID"), NewEffect);
	}

	// �ͷ�Ч��װ�������ÿ�ָ��
	delete TmpEffectLoader;
	TmpEffectLoader = NULL;
}

void LibraryManager::LoadWeaponFromXML()
{
	tinyxml2::XMLDocument doc;
	// ���Լ���XML�ļ�(TODO: ����ļ������쳣���������»�ȡ��XML)
	doc.LoadFile("XML/Weapon.xml");
	// ��ȡXML�ļ����ڵ�
	XMLElement* root = doc.FirstChildElement();
	// ���XML���ڵ�(TODO: ����ļ����ڵ��쳣���������»�ȡ��XML)
	if (root == NULL) { cout << "Root Node Is Null." << endl; return; }

	Weapon* NewWeapon = NULL;

	for (tinyxml2::XMLElement* XMLWeaponInfo = root->FirstChildElement(); XMLWeaponInfo != NULL; XMLWeaponInfo = XMLWeaponInfo->NextSiblingElement())
	{
		NewWeapon = new Weapon(XMLWeaponInfo);
		WeaponLibrary.Add(NewWeapon->GetWeaponID(), NewWeapon);
		NewWeapon = NULL;
	}
}

void LibraryManager::LoadUnitFromXML()
{
	tinyxml2::XMLDocument doc;
	// ���Լ���XML�ļ�(TODO: ����ļ������쳣���������»�ȡ��XML)
	doc.LoadFile("XML/Unit.xml");
	// ��ȡXML�ļ����ڵ�
	XMLElement* root = doc.FirstChildElement();
	// ���XML���ڵ�(TODO: ����ļ����ڵ��쳣���������»�ȡ��XML)
	if (root == NULL) { cout << "Unit XML File Root Node Is Null." << endl; return; }

	Unit* NewUnit = NULL;

	for (tinyxml2::XMLElement* XMLUnitInfo = root->FirstChildElement(); XMLUnitInfo != NULL; XMLUnitInfo = XMLUnitInfo->NextSiblingElement())
	{
		NewUnit = new Unit();
		NewUnit->LoadInfoFromXML(XMLUnitInfo);
		UnitLibrary.Add(NewUnit->GetUnitID(), NewUnit);
		NewUnit = NULL;
	}
}

void LibraryManager::LoadPieceFromXML()
{
	tinyxml2::XMLDocument doc;
	// ���Լ���XML�ļ�(TODO: ����ļ������쳣���������»�ȡ��XML)
	doc.LoadFile("XML/Piece.xml");
	// ��ȡXML�ļ����ڵ�
	XMLElement* root = doc.FirstChildElement();
	// ���XML���ڵ�(TODO: ����ļ����ڵ��쳣���������»�ȡ��XML)
	if (root == NULL) { cout << "Piece XML File Root Node Is Null." << endl; return; }

	PieceInstance* NewPiece = NULL;

	for (tinyxml2::XMLElement* XMLPieceInfo = root->FirstChildElement(); XMLPieceInfo != NULL; XMLPieceInfo = XMLPieceInfo->NextSiblingElement())
	{
		NewPiece = new PieceInstance();
		NewPiece->LoadInfoFromXML(XMLPieceInfo);
		PieceLibrary.Add(NewPiece->GetPieceID(), NewPiece);
		NewPiece = NULL;
	}
}

void LibraryManager::LoadCardFromXML()
{
	tinyxml2::XMLDocument doc;
	// ���Լ���XML�ļ�(TODO: ����ļ������쳣���������»�ȡ��XML)
	doc.LoadFile("XML/Card.xml");
	// ��ȡXML�ļ����ڵ�
	XMLElement* root = doc.FirstChildElement();
	// ���XML���ڵ�(TODO: ����ļ����ڵ��쳣���������»�ȡ��XML)
	if (root == NULL) { cout << "Card XML File Root Node Is Null." << endl; return; }

	Card* NewCard;

	for (tinyxml2::XMLElement* XMLCardInfo = root->FirstChildElement(); XMLCardInfo != NULL; XMLCardInfo = XMLCardInfo->NextSiblingElement())
	{

	}
}

void LibraryManager::UnloadPiecePedestalInfo()
{
	int n = PiecePedestalInfoLibrary.GetCount();

	int UnloadSuccessNum = 0;

	// �������ӵ�����Ϣ���ÿ����Ϣ��¼
	for (int i = 0; i < n; i++)
	{
		// ��ȡָ�����ӵ�����Ϣ��¼��ָ��
		PiecePedestalInfo* Tmp = PiecePedestalInfoLibrary.GetByIndex(i);
		// �ͷŸ�ָ��ָ������ӵ�����Ϣ����
		if (Tmp != NULL)
		{
			//cout << "PiecePedestalInfo Unload~" << endl;
			delete Tmp;
			Tmp = NULL;
			PiecePedestalInfoLibrary.GetByIndex(i) = NULL;
			UnloadSuccessNum++;
		}
	}

	cout << "PiecePedestalInfoLibrary Unload (" << UnloadSuccessNum << "/" << n << ")" << endl;
}
void LibraryManager::UnloadPieceStatusInfo()
{
	int n = PieceStatusInfoLibrary.GetCount();

	int UnloadSuccessNum = 0;

	// �������ӵ�����Ϣ���ÿ����Ϣ��¼
	for (int i = 0; i < n; i++)
	{
		// ��ȡָ�����ӵ�����Ϣ��¼��ָ��
		PieceStatusInfo* Tmp = PieceStatusInfoLibrary.GetByIndex(i);
		// �ͷŸ�ָ��ָ������ӵ�����Ϣ����
		if (Tmp != NULL)
		{
			//cout << "PieceStatusInfo Unload~" << endl;
			delete Tmp;
			Tmp = NULL;
			PieceStatusInfoLibrary.GetByIndex(i) = NULL;
			UnloadSuccessNum++;
		}
	}

	cout << "PieceStatusInfoLibrary Unload (" << UnloadSuccessNum << "/" << n << ")" << endl;
}

void LibraryManager::UnloadEffect()
{
	int n = EffectLibrary.GetCount();

	int UnloadSuccessNum = 0;

	// �������ӵ�����Ϣ���ÿ����Ϣ��¼
	for (int i = 0; i < n; i++)
	{
		// ��ȡָ�����ӵ�����Ϣ��¼��ָ��
		Effect* Tmp = EffectLibrary.GetByIndex(i);
		// �ͷŸ�ָ��ָ������ӵ�����Ϣ����
		if (Tmp != NULL)
		{
			//cout << "Effect Unload~" << endl;
			delete Tmp;
			Tmp = NULL;
			EffectLibrary.GetByIndex(i) = NULL;
			UnloadSuccessNum++;
		}
	}

	cout << "Effectbrary Unload (" << UnloadSuccessNum << "/" << n << ")" << endl;
}

void LibraryManager::UnloadWeapon()
{
	int n = WeaponLibrary.GetCount();

	int UnloadSuccessNum = 0;

	// �����������ÿ����Ϣ��¼
	for (int i = 0; i < n; i++)
	{
		// ��ȡָ��������¼��ָ��
		Weapon* Tmp = WeaponLibrary.GetByIndex(i);
		// �ͷŸ�ָ��ָ�����������
		if (Tmp != NULL)
		{
			delete Tmp;
			Tmp = NULL;
			WeaponLibrary.GetByIndex(i) = NULL;
			UnloadSuccessNum++;
		}
	}

	cout << "WeaponLibrary Unload (" << UnloadSuccessNum << "/" << n << ")" << endl;
}

void LibraryManager::UnloadUnit()
{
	int n = UnitLibrary.GetCount();

	int UnloadSuccessNum = 0;

	// ������λ���ÿ����Ϣ��¼
	for (int i = 0; i < n; i++)
	{
		// ��ȡָ��λ��¼��ָ��
		Unit* Tmp = UnitLibrary.GetByIndex(i);
		// �ͷŸ�ָ��ָ��ĵ�λ����
		if (Tmp != NULL)
		{
			delete Tmp;
			Tmp = NULL;
			UnitLibrary.GetByIndex(i) = NULL;
			UnloadSuccessNum++;
		}
	}

	cout << "UnitLibrary Unload (" << UnloadSuccessNum << "/" << n << ")" << endl;
}

void LibraryManager::UnloadPiece()
{
	int n = PieceLibrary.GetCount();

	int UnloadSuccessNum = 0;

	// ������λ���ÿ����Ϣ��¼
	for (int i = 0; i < n; i++)
	{
		// ��ȡָ��λ��¼��ָ��
		PieceInstance* Tmp = PieceLibrary.GetByIndex(i);
		// �ͷŸ�ָ��ָ��ĵ�λ����
		if (Tmp != NULL)
		{
			delete Tmp;
			Tmp = NULL;
			PieceLibrary.GetByIndex(i) = NULL;
			UnloadSuccessNum++;
		}
	}

	cout << "PieceLibrary Unload (" << UnloadSuccessNum << "/" << n << ")" << endl;
}

#endif