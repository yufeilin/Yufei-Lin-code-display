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
	// 棋子底座信息库
	Dictionary<string, PiecePedestalInfo*> PiecePedestalInfoLibrary;

	// 棋子雕像信息库
	Dictionary<string, PieceStatusInfo*> PieceStatusInfoLibrary;

	// 游戏效果库
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
	// 尝试加载XML文件(TODO: 如果文件加载异常则请求重新获取该XML)
	doc.LoadFile("XML/Status.xml");
	// 获取XML文件根节点
	XMLElement* root = doc.FirstChildElement();
	// 检查XML根节点(TODO: 如果文件根节点异常则请求重新获取该XML)
	if (root == NULL) { cout << "Root Node Is Null." << endl; return; }

	PieceStatusInfo* NewPieceStatusInfo = NULL;

	for (tinyxml2::XMLElement* XMLPieceStatusInfo = root->FirstChildElement(); XMLPieceStatusInfo != NULL; XMLPieceStatusInfo = XMLPieceStatusInfo->NextSiblingElement())
	{
		// 通过XML节点初始化一个棋子雕像信息实例
		NewPieceStatusInfo = new PieceStatusInfo(XMLPieceStatusInfo);
		// 将新生成的棋子雕像信息实例放入棋子雕像信息字典中
		PieceStatusInfoLibrary.Add(NewPieceStatusInfo->GetPieceStatusID(), NewPieceStatusInfo);
		// 将临时指针置空
		NewPieceStatusInfo = NULL;
	}
}

void LibraryManager::LoadEffectFromXML()
{
	tinyxml2::XMLDocument doc;
	// 尝试加载XML文件(TODO: 如果文件加载异常则请求重新获取该XML)
	doc.LoadFile("XML/Effect.xml");
	// 获取XML文件根节点
	XMLElement* root = doc.FirstChildElement();
	// 检查XML根节点(TODO: 如果文件根节点异常则请求重新获取该XML)
	if (root == NULL) { cout << "Root Node Is Null." << endl; return; }

	// 新建效果装载器
	EffectLoader* TmpEffectLoader = new EffectLoader();

	// 新建效果基类指针
	Effect* NewEffect = NULL;

	for (tinyxml2::XMLElement* XMLEffectInfo = root->FirstChildElement(); XMLEffectInfo != NULL; XMLEffectInfo = XMLEffectInfo->NextSiblingElement())
	{
		for (XMLElement* XMLEffectInfoItem = XMLEffectInfo->FirstChildElement(); XMLEffectInfoItem != NULL; XMLEffectInfoItem = XMLEffectInfoItem->NextSiblingElement())
		{
			if (strcmp(XMLEffectInfoItem->Value(), "EffectType") == 0)
			{
				// 先读取效果类型并根据类型生成对应的效果实例
				NewEffect = TmpEffectLoader->GetNewEffectByType(XMLEffectInfoItem->FirstChild()->ToText()->Value());
			}
			else if (strcmp(XMLEffectInfoItem->Value(), "EffectSubInfo") == 0)
			{
				// 将XML节点传递给衍生效果类进行初始化赋值
				NewEffect->LoadInfoFromXML(XMLEffectInfoItem);
			}			
		}
		// 将初始化完毕的衍生效果添加到效果字典中
		EffectLibrary.Add(NewEffect->GetParameterDictionary().GetStringParameter("EffectID"), NewEffect);
	}

	// 释放效果装载器并置空指针
	delete TmpEffectLoader;
	TmpEffectLoader = NULL;
}

void LibraryManager::LoadWeaponFromXML()
{
	tinyxml2::XMLDocument doc;
	// 尝试加载XML文件(TODO: 如果文件加载异常则请求重新获取该XML)
	doc.LoadFile("XML/Weapon.xml");
	// 获取XML文件根节点
	XMLElement* root = doc.FirstChildElement();
	// 检查XML根节点(TODO: 如果文件根节点异常则请求重新获取该XML)
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
	// 尝试加载XML文件(TODO: 如果文件加载异常则请求重新获取该XML)
	doc.LoadFile("XML/Unit.xml");
	// 获取XML文件根节点
	XMLElement* root = doc.FirstChildElement();
	// 检查XML根节点(TODO: 如果文件根节点异常则请求重新获取该XML)
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
	// 尝试加载XML文件(TODO: 如果文件加载异常则请求重新获取该XML)
	doc.LoadFile("XML/Piece.xml");
	// 获取XML文件根节点
	XMLElement* root = doc.FirstChildElement();
	// 检查XML根节点(TODO: 如果文件根节点异常则请求重新获取该XML)
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
	// 尝试加载XML文件(TODO: 如果文件加载异常则请求重新获取该XML)
	doc.LoadFile("XML/Card.xml");
	// 获取XML文件根节点
	XMLElement* root = doc.FirstChildElement();
	// 检查XML根节点(TODO: 如果文件根节点异常则请求重新获取该XML)
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

	// 遍历棋子底座信息库的每条信息记录
	for (int i = 0; i < n; i++)
	{
		// 获取指向棋子底座信息记录的指针
		PiecePedestalInfo* Tmp = PiecePedestalInfoLibrary.GetByIndex(i);
		// 释放该指针指向的棋子底座信息对象
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

	// 遍历棋子底座信息库的每条信息记录
	for (int i = 0; i < n; i++)
	{
		// 获取指向棋子底座信息记录的指针
		PieceStatusInfo* Tmp = PieceStatusInfoLibrary.GetByIndex(i);
		// 释放该指针指向的棋子底座信息对象
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

	// 遍历棋子底座信息库的每条信息记录
	for (int i = 0; i < n; i++)
	{
		// 获取指向棋子底座信息记录的指针
		Effect* Tmp = EffectLibrary.GetByIndex(i);
		// 释放该指针指向的棋子底座信息对象
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

	// 遍历武器库的每条信息记录
	for (int i = 0; i < n; i++)
	{
		// 获取指向武器记录的指针
		Weapon* Tmp = WeaponLibrary.GetByIndex(i);
		// 释放该指针指向的武器对象
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

	// 遍历单位库的每条信息记录
	for (int i = 0; i < n; i++)
	{
		// 获取指向单位记录的指针
		Unit* Tmp = UnitLibrary.GetByIndex(i);
		// 释放该指针指向的单位对象
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

	// 遍历单位库的每条信息记录
	for (int i = 0; i < n; i++)
	{
		// 获取指向单位记录的指针
		PieceInstance* Tmp = PieceLibrary.GetByIndex(i);
		// 释放该指针指向的单位对象
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