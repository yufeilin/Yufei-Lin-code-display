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
	// ���ӱ�ʶ��
	string PieceID = "";

	// ��������
	string PieceName = "";

	// ���Ӿ���󻨷�
	int PieceMineralCost = 0;

	// ������˹����
	int PieceGasCost = 0;

	// �������Ƿ���г���
	bool HasOrientation = false;

	// �����ƶ���Χ
	Range PieceMoveRange;

	// ���湥��ģʽ��Χ����
	// BaseMoveAbility[0] = 2 �����ܹ�������x������+z������(+1,+1)�������2���ڵĵз�����
	// BaseMoveAbility[1] = 2 �����ܹ�������x������(+1,0)�������2���ڵĵз�����
	//int BaseAttackRange[8];

	// �ƶ��������
	bool MoveEnable = true;

	// �Ƿ�Ϊ����Ŀ�����
	bool IsAttackTarget = false;

	// ���ӳ���(�������������ȫ��ģ������в��������������г����趨��)
	// ���� = -1 ������Ϊȫ�����ӣ������г����趨
	// ���� = 1 Ϊ�������� Z�� ������
	// ���� = 2 Ϊ�������� X�� ������
	// ���� = 3 Ϊ�������� Z�� ������
	// ���� = 4 Ϊ�������� X�� ������
	int Orientation = -1;

	// ����������
	// Owner = 0 ����������Ϊ���0
	// Owner = 1 ����������Ϊ���1
	// �Դ�����
	int Owner = 0;

	// �����ڿռ��е�λ��
	Vector3 InterPosition = Vector3();

	// ���������������ϵ�����
	Vector2Int InterCoordinate = Vector2Int();

	// �����������̵�������������
	Vector2Int OuterCoordinate = Vector2Int();

	// ָ����������ʵ����ָ��
	ChessBoard* LocateChessboard = NULL;

	// ÿ����λ���������
	Dictionary<string, int> ContainUnitNum;
	// ÿ����λ�����Ӧһ����λָ��list	
	Dictionary<string, List<Unit*>> ContainUnitsOfUnitName;
	// ÿ����λ����������±�
	Dictionary<int, string> UnitNameOfIndex;

	// ���Ӱ����ĵ�λ������Ŀ
	int UnitTypeNum = 0;

	// ���������������̸�Ĳ���
	int Layer = 0;

	// ���ӵ�ǰ�Ƿ���ս����
	bool InBattle = false;

	// ���ӵĳ�����ֵ
	double RetreatThreshold = 0.25f;

	// ָ���������ʵ����ָ��
	Card *RelativeCard = NULL;

	//Card f;

	// ���ӵ���������Ϣ
	string PiecePedestalInfoID;
	PiecePedestalInfo* CurrentPiecePedestalInfo = NULL;

	// ���ӵ���������Ϣ
	string PieceStatusInfoID;
	PieceStatusInfo* CurrentPieceStatusInfo = NULL;

	// ָ����������ʱ����Ч����ָ��
	string PieceDeathEffectID;
	Effect* PieceDeathEffect;

	// ���Ӷ�Ӧ�Ŀ�����ʹ�õ�ͼƬ(230 X 170)
	string MainImageID = "";

	// �����ڲ��������ʹ�õ�ͼƬ(70 X 70)
	string SideUnitIconImageID = "";

	List<string> CardDescriptionItemList;

	// ��ǰ�����Ƿ���
	bool IsAlive = true;

	// ս����ʱ����(��Щ��������ս�����п�ʼ�ͽ�����ʱ����и���)
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
			// ��XML�ж�������ID
			PieceID = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceName") == 0)
		{
			// ��XML�ж�����������
			PieceName = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceMineralCost") == 0)
		{
			// ��XML�ж������Ӿ��������
			PieceMineralCost = atoi(XMLPieceInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceGasCost") == 0)
		{
			// ��XML�ж���������������
			PieceGasCost = atoi(XMLPieceInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PiecePedestalID") == 0)
		{
			// ��XML�ж������ӵ�����ϢID
			PiecePedestalInfoID = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceStatusID") == 0)
		{
			// ��XML�ж������ӵ�����ϢID
			PieceStatusInfoID = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceDeathEffectID") == 0)
		{
			// ��XML�ж�����������ID
			PieceDeathEffectID = XMLPieceInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceInfoItem->Value(), "PieceIncludeUnit") == 0)
		{
			// ��XML�ж������Ӱ����ĵ�λ
			int Index = 0;
			string TmpUnitID;
			int TmpUnitNum;
			for (XMLElement* XMLPieceIncludeUnitItem = XMLPieceInfoItem->FirstChildElement(); XMLPieceIncludeUnitItem != NULL; XMLPieceIncludeUnitItem = XMLPieceIncludeUnitItem->NextSiblingElement())
			{
				if (strcmp(XMLPieceIncludeUnitItem->Value(), "UnitID") == 0)
				{
					// ��XML�ж��������λID
					TmpUnitID = XMLPieceIncludeUnitItem->FirstChild()->ToText()->Value();
				}
				else if (strcmp(XMLPieceIncludeUnitItem->Value(), "UnitNum") == 0)
				{
					// ��XML�ж��������λ����
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