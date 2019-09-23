#pragma once
#ifndef _RANGE
#define _RANGE



#include <iostream>
#include <sstream>
#include<vector>
#include<set>
#include"YufeiBasic.h"

//class Vector2Int;

using namespace std;

class Range
{
public:
	Range();
	~Range(){}
	void SetSingleCoordinateOn(Vector2Int NewCoordinate);
	void SetRadius(int NewRadius);
	void SetHalfSquareSideLength(int NewHalfSquareSideLength);
	void SetDirectionLength(string DirectionLengthString);
	int GetRangeCoordinateNum();
	vector<Vector2Int>& GetRangeCoordinateVector();
	//Vector2Int& GetRangeCoordinateByIndex(int Index);
	Range& operator=(Range &R);
	bool operator==(Range &R);
	bool operator!=(Range &R);
private:
	// 圆形范围的半径
	int Radius = 0;
	// 正方形范围的1/2边长
	int HalfSquareSideLength = 0;
	// 八个方向的长度
	int DirectionLength[8] = { 0,0,0,0,0,0,0,0 };
	// 范围的坐标集合
	set<Vector2Int> RangeCoordinateSet;

};

void split(const string& s, vector<int>& sv, const char flag = ' ') 
{
	sv.clear();
	istringstream iss(s);
	string temp;

	while (getline(iss, temp, flag)) 
	{
		sv.push_back(stoi(temp));
	}
	return;
}

inline Range::Range()
{
}

inline void Range::SetSingleCoordinateOn(Vector2Int NewCoordinate)
{
	set<Vector2Int>::iterator it;
	it = RangeCoordinateSet.find(NewCoordinate);
	if (it == RangeCoordinateSet.end())
	{
		RangeCoordinateSet.insert(NewCoordinate);
	}
}

void Range::SetRadius(int NewRadius)
{
	Vector2Int Origin(0, 0);
	for (int i = -NewRadius; i <= NewRadius; i++)
	{
		for (int j = -NewRadius; j <= NewRadius; j++)
		{
			Vector2Int Target(i, j);
			if (Vector2Int::Distance(Origin, Target) <= NewRadius)
			{
				SetSingleCoordinateOn(Target);
			}
		}
	}
}

void Range::SetHalfSquareSideLength(int NewHalfSquareSideLength)
{
	Vector2Int Origin(0, 0);
	for (int i = -NewHalfSquareSideLength; i <= NewHalfSquareSideLength; i++)
	{
		for (int j = -NewHalfSquareSideLength; j <= NewHalfSquareSideLength; j++)
		{
			Vector2Int Target(i, j);
			SetSingleCoordinateOn(Target);
		}
	}
}

void Range::SetDirectionLength(string DirectionLengthString)
{
	Vector2Int DirectionUnitCoordinate[8];

	DirectionUnitCoordinate[0] = Vector2Int(0, 1);
	DirectionUnitCoordinate[1] = Vector2Int(1, 1);
	DirectionUnitCoordinate[2] = Vector2Int(1, 0);
	DirectionUnitCoordinate[3] = Vector2Int(1, -1);
	DirectionUnitCoordinate[4] = Vector2Int(0, -1);
	DirectionUnitCoordinate[5] = Vector2Int(-1, -1);
	DirectionUnitCoordinate[6] = Vector2Int(-1, 0);
	DirectionUnitCoordinate[7] = Vector2Int(-1, 1);

	vector<int> DirectionLengthVector;

	split(DirectionLengthString, DirectionLengthVector, ' ');

	for (int i = 0; i < 8; i++)
	{
		for (int j = 1; j <= DirectionLengthVector[i]; j++)
		{
			Vector2Int Target(DirectionUnitCoordinate[i].x * j, DirectionUnitCoordinate[i].y * j);
			SetSingleCoordinateOn(Target);
		}
	}
}

inline int Range::GetRangeCoordinateNum()
{
	return RangeCoordinateSet.size();
}

inline vector<Vector2Int>& Range::GetRangeCoordinateVector()
{
	vector<Vector2Int> Tmp;
	set<Vector2Int>::iterator it;
	for (it = RangeCoordinateSet.begin(); it != RangeCoordinateSet.end(); it++)
	{
		// 这里可能会存在浅复制的问题
		Tmp.push_back(*it);
	}
	return Tmp;
}

inline Range & Range::operator=(Range & R)
{
	RangeCoordinateSet.clear();

	vector<Vector2Int> Tmp = R.GetRangeCoordinateVector();

	for (int i = 0; i < (int)(Tmp.size()); i++)
	{
		// 这里可能会存在浅复制的问题
		RangeCoordinateSet.insert(Tmp[i]);
	}
	
	return *this;
}

#endif
