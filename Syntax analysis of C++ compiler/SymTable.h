//SymTable.h

#pragma once

#include <vector>
#include "Sym.h"

/**
 * 符号表
 */

class SymTable
{
public:
	//实现插入新符号到符号表的操作。插入成功后返回该符号的Sym指针，失败则返回null。
	Sym * insertSym( Token * tk )
	{
		Sym * sym = new Sym(tk) ;
		syms.push_back(sym) ;
		return sym ;
	}

	//实现查找功能。在符号表中查找token string并返回该符号的Sym指针，失败则返回null。
	Sym * fineSym( Token * tk )
	{
		vector<Sym*>::iterator iter = syms.begin() ;
		while( iter != syms.end() )
		{
			if( tk->getString() == (*iter)->getToken()->getString() )
			{
				return *iter ;
			}
			iter ++ ;
		}
		return NULL ;
	}

	//实现删除功能。在符号表中删除符号tk
	void delSym( Token * tk )
	{
		vector<Sym*>::iterator iter = syms.begin() ;
		while( iter != syms.end() )
		{
			if( tk->getString() == (*iter)->getToken()->getString() )
			{
				syms.erase( iter ) ;
				return ;
			}
			iter ++ ;
		}
	}
private:
	vector<Sym*> syms ;
};