//Token.h

#pragma once

#include <iostream>
#include <string>
//#include "TypeKind.h"

using namespace std ;


enum TypeKind
{
	TK_TRUE ,
	TK_FALSE ,
	TK_OR ,
	TK_AND ,
	TK_NOT ,
	TK_INT ,
	TK_CHAR ,
	TK_FLOAT,
	TK_BOOL ,
	TK_STRING ,
	TK_WHILE ,
	TK_DO ,
	TK_IF ,
	TK_THEN ,
	TK_ELSE ,
	TK_END ,
	TK_REPEAT ,
	TK_UNTIL ,
	TK_READ ,
	TK_WRITE ,
	TK_GTR ,		//>
	TK_LEQ ,		//<=
	TK_GEQ ,		//>=
	TK_COMMA ,		//,
	TK_SEMICOLON ,	//;
	TK_ASSIGN ,		//:=
	TK_ADD ,		//+
	TK_SUB ,		//-
	TK_MUL ,		//*
	TK_DIV ,		///
	TK_LP ,			//(
	TK_RP ,			//)
	TK_LSS ,		//<
	TK_EQU ,		//=
	ID ,			//标识符
	NUM ,			//数值常数
	STR ,			//字符串常数
	FILE_END		//分析结束
};



class Token
{
public:
	Token( TypeKind tk , string str ) ;

	Token( TypeKind tk , double num ) ;

	void setNum( int num ) ;

	TypeKind getType() ;

	string getString() ;

	double getNum() ;

	friend ostream & operator <<( ostream & , Token & ) ;

private:
	TypeKind tk ;    //符号的类型
	string str ;    //若符号为标识符或常量，则保存为string
	int num ;       //若符号是整型常量，则记录常量值
};

