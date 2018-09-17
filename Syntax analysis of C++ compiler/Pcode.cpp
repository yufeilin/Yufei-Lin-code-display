//Pcode.cpp

#include "Pcode.h"
#include <sstream>

//这里先来个int转string的函数
string int2str(int &i) 
{
	string s;
	stringstream ss(s);
	ss << i;
	return ss.str();
}

void Pcode::stmt_sequence( TreeNode * subroot )
{
	NodeType nodeType = subroot->getNodeType() ;
	if( nodeType == STMT_SEQUENCE )
	{
		statment(subroot->getChild(0)) ;
		statment(subroot->getChild(1)) ;
	}
}


void Pcode::statment( TreeNode * subroot )
{
	if( subroot == NULL )
	{
		cout << "errors!" << endl ;
		return ;
	}
	NodeType nodeType = subroot->getNodeType() ;
	switch( nodeType )
	{
	case STMT_SEQUENCE:
		stmt_sequence(subroot) ;
		break ;
	case IF_STMT:
		if_stmt(subroot) ;
		break;
	case REPEAT_STMT:
		repeat_stmt(subroot) ;
		break;
	case WHILE_STMT:
		while_stmt(subroot) ;
		break; 
	case READ_STMT:
		read_stmt(subroot) ;
		break;
	case WRITE_STMT:
		write_stmt(subroot) ;
		break;
	case ASSIGN_STMT:
		assign_stmt(subroot) ;
		break;
	case EQU_EXP:
	case GTR_EXP:
	case GEQ_EXP:
	case LSS_EXP:
	case LEQ_EXP:
		compexp(subroot) ;
		break;
	case LOG_AND_EXP:
	case LOG_OR_EXP:
	case LOG_NOT_EXP:
		artexp(subroot) ;
		break;
	case ADD_EXP:
	case SUB_EXP:
	case MUL_EXP:
	case DIV_EXP:
		exp(subroot) ;
		break;
	case FACTOR:
		factor(subroot) ;
		break;
	default:
		break;
	}
}


void Pcode::exp( TreeNode * subroot )
{
	NodeType nodeType = subroot->getNodeType() ;
	statment(subroot->getChild(0)) ;
	statment(subroot->getChild(1)) ;
	string t ;
	switch(nodeType)
	{
	case ADD_EXP:
		t = "+" ; break;
	case SUB_EXP:
		t = "-" ; break;
	case MUL_EXP:
		t = "*" ; break;
	case DIV_EXP:
		t = "/" ; break;
	default: break;
	}
	string s1 = syms.top() ;
	syms.pop() ;
	string s2 = syms.top() ;
	syms.pop() ;
	string s3 = "t" + int2str(label) ;
	syms.push(s3) ;
	PcodeType *pcodeType = new PcodeType( label++ , t , s2 , s1 , s3 ) ;
	pcodes.push_back(pcodeType) ;
}

void Pcode::compexp( TreeNode * subroot )
{
	NodeType nodeType = subroot->getNodeType() ;
	statment(subroot->getChild(0)) ;
	statment(subroot->getChild(1)) ;
	string t ;
	switch(nodeType)
	{
	case EQU_EXP:
		t = "=" ; break;
	case GTR_EXP:
		t = ">" ; break; 
	case GEQ_EXP:
		t = ">=" ; break; 
	case LSS_EXP:
		t = "<" ; break;
	case LEQ_EXP:
		t = "<=" ; break; 
	default: break ;
	}
	string s1 = syms.top() ;
	syms.pop() ;
	string s2 = syms.top() ;
	syms.pop() ;
	string s3 = "t" + int2str(label) ;
	syms.push(s3) ;
	PcodeType *pcodeType = new PcodeType( label++ , t , s2 , s1 , s3 ) ;
	pcodes.push_back(pcodeType) ;
}

void Pcode::artexp( TreeNode * subroot )
{
	NodeType nodeType = subroot->getNodeType() ;
	if( nodeType == LOG_AND_EXP )
	{
		statment(subroot->getChild(0)) ;
		string s1 = syms.top() ;
		syms.pop() ;
		statment( subroot->getChild(1)) ;
		string s2 = syms.top() ;
		syms.pop() ;
		string s3 = "t" + int2str(label) ;
		syms.push(s3) ;
		PcodeType *p = new PcodeType(label++ , "and" , s2 , s1 , s3 ) ;
		pcodes.push_back(p) ;
	}
	else if( nodeType == LOG_OR_EXP )
	{
		statment(subroot->getChild(0)) ;
		string s1 = syms.top() ;
		syms.pop() ;
		statment( subroot->getChild(1)) ;
		string s2 = syms.top() ;
		syms.pop() ;
		string s3 = "t" + int2str(label) ;
		syms.push(s3) ;
		PcodeType *p = new PcodeType(label++ , "or" , s2 , s1 , s3 ) ;
		pcodes.push_back(p) ;
	}
	else
	{
		statment(subroot->getChild(0)) ;
		string s1 = syms.top() ;
		syms.pop() ;
		string s3 = "t" + int2str(label) ;
		syms.push(s3) ;
		PcodeType *p = new PcodeType(label++ , "not" , s1 , "-" , s3 ) ;
		pcodes.push_back(p) ;
	}
}

void Pcode::if_stmt( TreeNode * subroot )
{
	statment(subroot->getChild(0)) ;
	string s1 = syms.top() ;
	syms.pop() ;
	PcodeType *p1 = new PcodeType( label++ , "j1" , s1 , "-" , "true" ) ;
	pcodes.push_back(p1) ;
	PcodeType *p2 = new PcodeType( label++ , "jump" , "-" , "-" , "false" ) ;
	pcodes.push_back(p2) ;
	p1->setlastparam(int2str(label)) ;
	statment(subroot->getChild(1)) ;
	p2->setlastparam(int2str(label)) ;
	if( subroot->getChild(2) != NULL )
	{
		statment(subroot->getChild(2)) ;
	}
}

void Pcode::repeat_stmt( TreeNode * subroot )
{
	int curaddr = label ;
	statment(subroot->getChild(0)) ;
	statment(subroot->getChild(1)) ;
	string s = syms.top() ;
	syms.pop() ;
	PcodeType *p = new PcodeType( label++ , "j1" , s , "-" , int2str(curaddr) ) ;
	pcodes.push_back(p) ;
}

void Pcode::while_stmt( TreeNode * subroot )
{
	int curaddr = label ;
	statment(subroot->getChild(0)) ;
	string s = syms.top() ;
	syms.pop() ;
	PcodeType *p1 = new PcodeType(label++ , "j0" , s , "-" , "false" ) ;
	pcodes.push_back(p1) ;
	statment(subroot->getChild(1)) ;
	PcodeType *p2 = new PcodeType(label++ , "jump" , "-" , "-" , int2str(curaddr)) ;
	pcodes.push_back(p2) ;
	p1->setlastparam(int2str(label)) ;
}

void Pcode::read_stmt( TreeNode * subroot )
{
	string s = subroot->getToken()->getString() ;
	PcodeType *p = new PcodeType(label++ , "read" , "-" , "-" , s ) ;
	pcodes.push_back(p) ;
}

void Pcode::write_stmt( TreeNode * subroot )
{
	statment(subroot->getChild(0)) ;
	string s = syms.top() ;
	syms.pop() ;
	PcodeType *p = new PcodeType(label++ , "write" , "-" , "-" , s ) ;
	pcodes.push_back(p) ;
}

void Pcode::assign_stmt( TreeNode * subroot )
{
	string s1 = subroot->getToken()->getString() ;
	statment(subroot->getChild(0)) ;
	string s2 = syms.top() ;
	syms.pop() ;
	PcodeType *p = new PcodeType(label++, ":=" , s2 , "-" , s1 ) ;
	pcodes.push_back(p) ;
}

void Pcode::factor( TreeNode * subroot ) 
{
	string s = subroot->getToken()->getString() ;
	syms.push(s) ;
}