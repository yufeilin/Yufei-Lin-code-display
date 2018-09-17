//Pcode.h

#pragma once

#include "TreeNode.h"
#include "PcodeType.h"
#include <vector>
#include <stack>

/**
 * 汇编代码产生类
 */

class Pcode
{
public:
	Pcode( TreeNode * root )
	{
		this->root = root ;
		label = 0 ;
	}
	void getpcode()
	{
		statment(root) ;
	}
	void output()
	{
		for( int i = 0 ; i < label ; i ++ )
			pcodes[i]->output() ;
	}
	vector<PcodeType*> getcode() 
	{
		return pcodes ;
	}
private:
	TreeNode * root ;
	int label ;
	vector<PcodeType*> pcodes ;
	stack<string> syms ;
	void stmt_sequence( TreeNode * subroot ) ;
	void statment( TreeNode * subroot ) ;
	void exp( TreeNode * subroot ) ;
	void compexp( TreeNode * subroot ) ;
	void artexp( TreeNode * subroot ) ;
	void if_stmt( TreeNode * subroot ) ;
	void repeat_stmt( TreeNode * subroot ) ;
	void while_stmt( TreeNode * subroot ) ;
	void read_stmt( TreeNode * subroot ) ;
	void write_stmt( TreeNode * subroot ) ;
	void assign_stmt( TreeNode * subroot ) ;
	void factor( TreeNode * subroot ) ;
};