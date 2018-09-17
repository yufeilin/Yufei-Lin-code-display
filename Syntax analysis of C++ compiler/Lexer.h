//Lexer.h

#pragma once

#include "Token.h"
#include "ErrorsInfo.h"
#include <iostream>
#include <vector>
#include <map>

using namespace std ;

/**
 * 词法分析器
 * 接口为构造函数
 */

class Lexer
{
public:
	Lexer( char [] , int ) ;
	void lexer( ) ;
	vector<Token*> getTokens() ;
	void Output() ;
private:
	char *buffer ;
	int n ;
	vector<Token*> tokens ;
	vector<string> errors ;
	static map<string,TypeKind> KindMap ;
	static void initKindMap() ;
	inline bool isLetter( char ) ;
	inline bool isDigit( char ) ;
	inline bool isSpace( char ) ;
	inline bool isSym( char ) ;
	inline bool islegal( char ) ;
};


