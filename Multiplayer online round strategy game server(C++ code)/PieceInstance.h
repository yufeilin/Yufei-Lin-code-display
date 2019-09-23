#ifndef _PIECEINSTANCE
#define _PIECEINSTANCE

#include<string>
#include<stdio.h>

#include"YufeiBasic.h"
#include"Project28Basic.h"

class Card;
class ChessBoard;
class Effect;
class PiecePedestalInfo;
class PieceStatusInfo;
class Unit;
class Range;

using namespace std;


class PieceInstance
{
public:
	PieceInstance();
	~PieceInstance();
	void LoadInfoFromXML(XMLElement* XMLEffectInfo);

	inline string GetPieceID() const { return PieceID; }
	inline void SetPieceID(string NewPieceID) { PieceID = NewPieceID; }
	inline string GetPieceName() const { return PieceName; }
	inline void SetPieceName(string NewPieceName) { PieceName = NewPieceName; }

	inline int GetPieceMineralCost() const { return PieceMineralCost; }
	inline void SetPieceMineralCost(int NewPieceMineralCost) { PieceMineralCost = NewPieceMineralCost; }
	inline int GetPieceGasCost() const { return PieceGasCost; }
	inline void SetPieceGasCost(int NewPieceGasCost) { PieceGasCost = NewPieceGasCost; }

	inline int GetOwner() const { return Owner; }
	void SetOwner(int NewOwnerID); 
	inline int GetOrientation() const { return Orientation; }
	void SetOrientation(int NewOrientation); 


	void SetPiecePedestalInfo(PiecePedestalInfo& NewPiecePedestalInfo);
	void SetPieceStatusInfo(PieceStatusInfo& NewPieceStatusInfo);

	PieceInstance& operator= (const PieceInstance &V);

private:
	// 棋子标识符
	string PieceID = "";

	// 棋子名称
	string PieceName = "";

	// 棋子晶体矿花费
	int PieceMineralCost = 0;

	// 棋子瓦斯花费
	int PieceGasCost = 0;

	// 该棋子是否具有朝向
	bool HasOrientation = false;

	// 棋子移动范围
	Range PieceMoveRange;

	// 常规攻击模式范围数组
	// BaseMoveAbility[0] = 2 棋子能够攻击沿x轴正向+z轴正向(+1,+1)方向最大2步内的敌方棋子
	// BaseMoveAbility[1] = 2 棋子能够攻击沿x轴正向(+1,0)方向最大2步内的敌方棋子
	//int BaseAttackRange[8];

	// 移动允许标旗
	bool MoveEnable = true;

	// 是否为攻击目标标旗
	bool IsAttackTarget = false;

	// 棋子朝向(绝大多数棋子是全向的，但是有部分特殊棋子是有朝向设定的)
	// 朝向 = -1 该棋子为全向棋子，不具有朝向设定
	// 朝向 = 1 为世界坐标 Z轴 正方向
	// 朝向 = 2 为世界坐标 X轴 正方向
	// 朝向 = 3 为世界坐标 Z轴 负方向
	// 朝向 = 4 为世界坐标 X轴 负方向
	int Orientation = -1;

	// 棋子所有者
	// Owner = 0 棋子所有者为玩家0
	// Owner = 1 棋子所有者为玩家1
	// 以此类推
	int Owner = 0;

	// 棋子在空间中的位置
	Vector3 InterPosition = Vector3();

	// 棋子在所在棋盘上的坐标
	Vector2Int InterCoordinate = Vector2Int();

	// 棋子所在棋盘的棋盘世界坐标
	Vector2Int OuterCoordinate = Vector2Int();

	// 指向所在棋盘实例的指针
	ChessBoard* LocateChessboard = NULL;

	// 每个单位种类的数量
	Dictionary<string, int> ContainUnitNum;
	// 每个单位种类对应一个单位指针list	
	Dictionary<string, List<Unit*>> ContainUnitsOfUnitName;
	// 每个单位种类的索引下标
	Dictionary<int, string> UnitNameOfIndex;

	// 棋子包含的单位种类数目
	int UnitTypeNum = 0;

	// 棋子在它所在棋盘格的层数
	int Layer = 0;

	// 棋子当前是否处于战斗中
	bool InBattle = false;

	// 棋子的撤退阈值
	double RetreatThreshold = 0.25f;

	// 指向关联卡牌实例的指针
	Card *RelativeCard = NULL;

	//Card f;

	// 棋子底座配置信息
	string PiecePedestalInfoID;
	PiecePedestalInfo* CurrentPiecePedestalInfo = NULL;

	// 棋子雕像配置信息
	string PieceStatusInfoID;
	PieceStatusInfo* CurrentPieceStatusInfo = NULL;

	// 指向棋子死亡时触发效果的指针
	string PieceDeathEffectID;
	Effect* PieceDeathEffect;

	// 棋子对应的卡牌上使用的图片(230 X 170)
	string MainImageID = "";

	// 棋子在侧边棋子栏使用的图片(70 X 70)
	string SideUnitIconImageID = "";

	List<string> CardDescriptionItemList;

	// 当前棋子是否存活
	bool IsAlive = true;

	// 战斗临时变量(这些变量会在战斗序列开始和结束的时候进行更新)
	//---------------------------------------------------

	double CurrentPieceHealthBeforeBattle = 0;

	//---------------------------------------------------

};

PieceInstance::PieceInstance()
{
}

PieceInstance::~PieceInstance()
{
}

void PieceInstance::LoadInfoFromXML(XMLElement * XMLPieceInfo)
{
	for (XMLElement* XMLPieceInfoItem = XMLPieceInfo->FirstChildElement(); XMLPieceInfoItem != NULL; XMLPieceInfoItem = XMLPieceInfoItem->NextSiblingElement())
	{
		if (strcmp(XMLPieceInfoItem->Value(), "PieceID") == 0)
		{
			// 从XML中读入棋子ID
			PieceID = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceName") == 0)
		{
			// 从XML中读入棋子名称
			PieceName = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceMineralCost") == 0)
		{
			// 从XML中读入棋子晶体矿消耗
			PieceMineralCost = atoi(XMLPieceInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceGasCost") == 0)
		{
			// 从XML中读入棋子气体消耗
			PieceGasCost = atoi(XMLPieceInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PiecePedestalID") == 0)
		{
			// 从XML中读入棋子底座信息ID
			PiecePedestalInfoID = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceStatusID") == 0)
		{
			// 从XML中读入棋子雕像信息ID
			PieceStatusInfoID = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceDeathEffectID") == 0)
		{
			// 从XML中读入棋子亡语ID
			PieceDeathEffectID = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceIncludeUnit") == 0)
		{
			// 从XML中读入棋子包含的单位
			int Index = 0;
			string TmpUnitID;
			int TmpUnitNum;
			for (XMLElement* XMLPieceIncludeUnitItem = XMLPieceInfoItem->FirstChildElement(); XMLPieceIncludeUnitItem != NULL; XMLPieceIncludeUnitItem = XMLPieceIncludeUnitItem->NextSiblingElement())
			{
				if (strcmp(XMLPieceIncludeUnitItem->Value(), "UnitID") == 0)
				{
					// 从XML中读入包含单位ID
					TmpUnitID = XMLPieceIncludeUnitItem->FirstChild()->ToText()->Value();
				}
				else if (strcmp(XMLPieceIncludeUnitItem->Value(), "UnitNum") == 0)
				{
					// 从XML中读入包含单位数量
					TmpUnitNum = atoi(XMLPieceIncludeUnitItem->FirstChild()->ToText()->Value());
				}

				if (Index%2 == 1)
				{
					//cout << "ContainUnitID:" << TmpUnitID << " ContainUnitNum:" << TmpUnitNum << endl;
					ContainUnitNum.Add(TmpUnitID, TmpUnitNum);
				}

				Index++;
			}
		}
	}
}

inline void PieceInstance::SetOwner(int NewOwnerID)
{
	if (NewOwnerID >= 0) Owner = NewOwnerID;
	else abort();
}

inline void PieceInstance::SetOrientation(int NewOrientation)
{
	if (NewOrientation >= 1 && NewOrientation <= 4) Orientation = NewOrientation;
	else abort();
}

inline void PieceInstance::SetPiecePedestalInfo(PiecePedestalInfo & NewPiecePedestalInfo)
{
	CurrentPiecePedestalInfo =  &NewPiecePedestalInfo;
}

inline void PieceInstance::SetPieceStatusInfo(PieceStatusInfo & NewPieceStatusInfo)
{
	CurrentPieceStatusInfo = &NewPieceStatusInfo;
}

inline PieceInstance & PieceInstance::operator=(const PieceInstance & V)
{

	this->PieceID = V.GetPieceID();
	this->PieceName = V.GetPieceName();
	this->PieceMineralCost = V.GetPieceMineralCost();
	this->PieceGasCost = V.GetPieceGasCost();

	bool HasOrientation = false;

	Range PieceMoveRange;

	bool MoveEnable = true;

	bool IsAttackTarget = false;

	int Orientation = -1;

	Owner = V.GetOwner();

	return *this;
}

#endif