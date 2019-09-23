#ifndef _PIECEPEDESTALINFO
#define _PIECEPEDESTALINFO

#include<iostream>
#include "tinyxml2\tinyxml2.h"
#include "YufeiBasic.h"
#include"Project28Basic.h"

using namespace tinyxml2;
using namespace std;

void split(const string& s, vector<float>& sv, const char flag = ' ')
{
	sv.clear();
	istringstream iss(s);
	string temp;

	while (getline(iss, temp, flag))
	{
		cout << temp<<endl;
		sv.push_back(stof(temp));
	}
	return;
}

class PiecePedestalInfo
{
public:
	PiecePedestalInfo();
	~PiecePedestalInfo();
	PiecePedestalInfo(XMLElement* XMLPiecePedestalInfo);
	inline string GetPiecePedestalID() { return PiecePedestalID; }
	inline string GetPiecePedestalName() { return PiecePedestalName; }
	inline Vector3 GetPiecePedestalLocalPosition() { return LocalPosition; }
	inline Vector3 GetPiecePedestalLocalScale() { return LocalScale; }
	inline Vector3 GetPiecePedestalLocalRotation() { return LocalRotation; }

private:
	// 棋子底座ID
	string PiecePedestalID = "";
	// 棋子底座名称
	string PiecePedestalName = "";
	// 棋子底座局部位置
	Vector3 LocalPosition = Vector3();
	// 棋子底座局部缩放
	Vector3 LocalScale = Vector3(1, 1, 1);
	// 棋子底座局部旋转
	Vector3 LocalRotation = Vector3(0, 0, 0);


};

PiecePedestalInfo::PiecePedestalInfo()
{
}

PiecePedestalInfo::~PiecePedestalInfo()
{
}

PiecePedestalInfo::PiecePedestalInfo(XMLElement * XMLPiecePedestalInfo)
{
	for (XMLElement* XMLPiecePedestalInfoItem = XMLPiecePedestalInfo->FirstChildElement(); XMLPiecePedestalInfoItem != NULL; XMLPiecePedestalInfoItem = XMLPiecePedestalInfoItem->NextSiblingElement())
	{
		if (strcmp(XMLPiecePedestalInfoItem->Value(), "PiecePedestalID") == 0)
		{
			// 从XML中读入棋子底座ID
			PiecePedestalID = XMLPiecePedestalInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPiecePedestalInfoItem->Value(), "PiecePedestalName") == 0)
		{
			// 从XML中读入棋子底座名称
			PiecePedestalName = XMLPiecePedestalInfoItem->FirstChild()->ToText()->Value();
		}
		else if (strcmp(XMLPiecePedestalInfoItem->Value(), "Position") == 0)
		{
			// 从XML中读入棋子底座局部位置
			LocalPosition = Vector3(XMLPiecePedestalInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLPiecePedestalInfoItem->Value(), "Scale") == 0)
		{
			// 从XML中读入棋子底座局部缩放
			LocalScale = Vector3(XMLPiecePedestalInfoItem->FirstChild()->ToText()->Value());
		}
		else if (strcmp(XMLPiecePedestalInfoItem->Value(), "Rotation") == 0)
		{
			// 从XML中读入棋子底座局部旋转
			LocalRotation = Vector3(XMLPiecePedestalInfoItem->FirstChild()->ToText()->Value());
		}
	}
}

#endif