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
	// 棋子雕像ID
	string PieceStatusID = "";
	// 棋子雕像名称
	string PieceStatusName = "";
	// 棋子底座局部缩放
	Vector3 LocalScale = Vector3(1, 1, 1);
	// 棋子底座局部旋转
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
			// 从XML中读入棋子雕像ID
			PieceStatusID = XMLPieceStatusInfoItem->FirstChild()->ToText()->Value();
			cout << PieceStatusID << endl;
		}
		else if (strcmp(XMLPieceStatusInfoItem->Value(), "PieceStatusName") == 0)
		{
			// 从XML中读入棋子雕像名称
			PieceStatusName = XMLPieceStatusInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPieceStatusInfoItem->Value(), "Scale") == 0)
		{
			// 从XML中读入棋子雕像局部缩放
			LocalScale = Vector3(XMLPieceStatusInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLPieceStatusInfoItem->Value(), "Rotation") == 0)
		{
			// 从XML中读入棋子雕像局部旋转
			LocalRotation = Vector3(XMLPieceStatusInfoItem->FirstChild()->ToText()->Value());
		}
	}
}

#endif