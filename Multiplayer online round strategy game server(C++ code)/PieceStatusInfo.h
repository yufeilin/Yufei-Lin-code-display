#ifndef _PIECESTATUSINFO
#define _PIECESTATUSINFO

#include<iostream>
#include "tinyxml2\tinyxml2.h"
#include "YufeiBasic.h"
#include"Project28Basic.h"

using namespace tinyxml2;
using namespace std;

class PieceStatusInfo
{
public:
	PieceStatusInfo();
	~PieceStatusInfo();
	PieceStatusInfo(XMLElement* XMLPieceStatusInfo);
	inline string GetPieceStatusID() { return PieceStatusID; }
	inline string GetPieceStatusName() { return PieceStatusName; }
	inline Vector3 GetPiecePedestalLocalScale() { return LocalScale; }
	inline Vector3 GetPiecePedestalLocalRotation() { return LocalRotation; }

private:
	// ���ӵ���ID
	string PieceStatusID = "";
	// ���ӵ�������
	string PieceStatusName = "";
	// ���ӵ����ֲ�����
	Vector3 LocalScale = Vector3(1, 1, 1);
	// ���ӵ����ֲ���ת
	Vector3 LocalRotation = Vector3(0, 0, 0);
};

PieceStatusInfo::PieceStatusInfo()
{
}

PieceStatusInfo::~PieceStatusInfo()
{
}

PieceStatusInfo::PieceStatusInfo(XMLElement * XMLPieceStatusInfo)
{
	for (XMLElement* XMLPieceStatusInfoItem = XMLPieceStatusInfo->FirstChildElement(); XMLPieceStatusInfoItem != NULL; XMLPieceStatusInfoItem = XMLPieceStatusInfoItem->NextSiblingElement())
	{
		if (strcmp(XMLPieceStatusInfoItem->Value(), "PieceStatusID") == 0)
		{
			// ��XML�ж������ӵ���ID
			PieceStatusID = XMLPieceStatusInfoItem->FirstChild()->ToText()->Value();
			cout << PieceStatusID << endl;
		}
		else if (strcmp(XMLPieceStatusInfoItem->Value(), "PieceStatusName") == 0)
		{
			// ��XML�ж������ӵ�������
			PieceStatusName = XMLPieceStatusInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceStatusInfoItem->Value(), "Scale") == 0)
		{
			// ��XML�ж������ӵ���ֲ�����
			LocalScale = Vector3(XMLPieceStatusInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLPieceStatusInfoItem->Value(), "Rotation") == 0)
		{
			// ��XML�ж������ӵ���ֲ���ת
			LocalRotation = Vector3(XMLPieceStatusInfoItem->FirstChild()->ToText()->Value());
		}
	}
}

#endif