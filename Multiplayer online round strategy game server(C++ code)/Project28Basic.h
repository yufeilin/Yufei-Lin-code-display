#ifndef _PROJECT28BASIC
#define _PROJECT28BASIC

//class Card;
//class ChessBoard;
//class Effect;
//class PieceInstance;
//class PiecePedestalInfo;
//class PieceStatusInfo;
//class Unit;
//class Range;

// 由于在系统编译的时候是由上至下编译头文件的
// 添加头文件的时候基类文件在上，基于基类的子类头文件在其下，先编译基类再编译子类

#include"Range.h"
#include"OperationBase.h"

#include"ChessBoard.h"
#include"Effect.h"
#include"EffectLoader.h"
#include"PiecePedestalInfo.h"
#include"PieceStatusInfo.h"
#include"Unit.h"

#include"Weapon.h"

// 游戏物体类
#include"Card.h"
#include"PieceInstance.h"

// 服务器管理器类
#include "LibraryManager.h"

class PiecePedestalInfo;
class PieceStatusInfo;

#endif