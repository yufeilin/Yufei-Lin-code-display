//Token.cpp

#include "Token.h"

//类型跟字符串构造
Token::Token( TypeKind tk , string str )
{
	this->tk = tk ;
	this->str = str ;
	this->num = 0 ;
}

//类型跟数字构造
Token::Token( TypeKind tk , double num )
{
	this->tk = tk ;
	this->num = num ;
	this->str = "" ;
}

TypeKind Token::getType()
{
	return tk ;
}

string Token::getString()
{
	return str ;
}

double Token::getNum()
{
	return num ;
}
	
void Token::setNum( int num )
{
	this->num = num ;
}

#ifndef OPER_
#define OPER_

ostream & operator << ( ostream & output , Token & A )
{
	switch ( A.getType() )
	{
	case TK_TRUE:
		output << '(' << "TK_TRUE" << ',' << A.getString() << ')' ;
		break ;
	case TK_FALSE:
		output << '(' << "TK_FALSE" << ',' << A.getString() << ')' ;
		break ;
	case TK_OR:
		output << '(' << "TK_OR" << ',' << A.getString() << ')' ;
		break ;
	case TK_AND:
		output << '(' << "TK_AND" << ',' << A.getString() << ')' ;
		break ;
	case TK_NOT:
		output << '(' << "TK_NOT" << ',' << A.getString() << ')' ;
		break ;
	case TK_INT:
		output << '(' << "TK_INT" << ',' << A.getString() << ')' ;
		break ;
	case TK_CHAR:
		output << '(' << "TK_CHAR" << ',' << A.getString() << ')' ;
		break ;
	case TK_FLOAT:
		output << '(' << "TK_FLOAT" << ',' << A.getString() << ')' ;
		break ;
	case TK_BOOL:
		output << '(' << "TK_BOOL" << ',' << A.getString() << ')' ;
		break ;
	case TK_STRING:
		output << '(' << "TK_STRING" << ',' << A.getString() << ')' ;
		break ;
	case TK_WHILE:
		output << '(' << "TK_WHILE" << ',' << A.getString() << ')' ;
		break ;
	case TK_DO:
		output << '(' << "TK_DO" << ',' << A.getString() << ')' ;
		break ;
	case TK_IF:
		output << '(' << "TK_IF" << ',' << A.getString() << ')' ;
		break ;
	case TK_THEN:
		output << '(' << "TK_THEN" << ',' << A.getString() << ')' ;
		break ;
	case TK_ELSE:
		output << '(' << "TK_ELSE" << ',' << A.getString() << ')' ;
		break ;
	case TK_END:
		output << '(' << "TK_END" << ',' << A.getString() << ')' ;
		break ;
	case TK_REPEAT:
		output << '(' << "TK_REPEAT" << ',' << A.getString() << ')' ;
		break ;
	case TK_UNTIL:
		output << '(' << "TK_UNTIL" << ',' << A.getString() << ')' ;
		break ;
	case TK_READ:
		output << '(' << "TK_READ" << ',' << A.getString() << ')' ;
		break ;
	case TK_WRITE:
		output << '(' << "TK_WRITE" << ',' << A.getString() << ')' ;
		break ;
	case TK_GTR:		//>
		output << '(' << "TK_GTR" << ',' << A.getString() << ')' ;
		break ;
	case TK_LEQ:		//<=
		output << '(' << "TK_LEQ" << ',' << A.getString() << ')' ;
		break ;
	case TK_GEQ:		//>=
		output << '(' << "TK_GEQ" << ',' << A.getString() << ')' ;
		break ;
	case TK_COMMA:		//,
		output << '(' << "TK_COMMA" << ',' << A.getString() << ')' ;
		break ;
	case TK_SEMICOLON:	//;
		output << '(' << "TK_SEMICOLON" << ',' << A.getString() << ')' ;
		break ;
	case TK_ASSIGN:		//:=
		output << '(' << "TK_ASSIGN" << ',' << A.getString() << ')' ;
		break ;
	case TK_ADD:		//+
		output << '(' << "TK_ADD" << ',' << A.getString() << ')' ;
		break ;
	case TK_SUB:		//-
		output << '(' << "TK_SUB" << ',' << A.getString() << ')' ;
		break ;
	case TK_MUL:		//*
		output << '(' << "TK_MUL" << ',' << A.getString() << ')' ;
		break ;
	case TK_DIV:		///
		output << '(' << "TK_DIV" << ',' << A.getString() << ')' ;
		break ;
	case TK_LP:			//(
		output << '(' << "TK_LP" << ',' << A.getString() << ')' ;
		break ;
	case TK_RP:			//)
		output << '(' << "TK_RP" << ',' << A.getString() << ')' ;
		break ;
	case TK_LSS:		//<
		output << '(' << "TK_LSS" << ',' << A.getString() << ')' ;
		break ;
	case TK_EQU:		//=
		output << '(' << "TK_EQU" << ',' << A.getString() << ')' ;
		break ;
	case STR:
		output << '(' << "STR" << ',' << A.getString() << ')' ;
		break ;
	case ID:
		output << '(' << "ID" << ',' << A.getString() << ')' ;
		break ;
	case NUM:
		output << '(' << "NUM" << ',' << A.getString() << ')' ;
		break ;
	default:
		output << '(' << "ERROR" << ')' ;
	}
	return output ;
}

#endif