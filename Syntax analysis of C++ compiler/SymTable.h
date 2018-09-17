//SymTable.h

#pragma once

#include <vector>
#include "Sym.h"

/**
 * ���ű�
 */

class SymTable
{
public:
	//ʵ�ֲ����·��ŵ����ű�Ĳ���������ɹ��󷵻ظ÷��ŵ�Symָ�룬ʧ���򷵻�null��
	Sym * insertSym( Token * tk )
	{
		Sym * sym = new Sym(tk) ;
		syms.push_back(sym) ;
		return sym ;
	}

	//ʵ�ֲ��ҹ��ܡ��ڷ��ű��в���token string�����ظ÷��ŵ�Symָ�룬ʧ���򷵻�null��
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

	//ʵ��ɾ�����ܡ��ڷ��ű���ɾ������tk
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