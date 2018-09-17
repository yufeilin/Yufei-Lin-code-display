//Parse.h

#pragma once

#include <vector>
#include "TreeNode.h"
#include "SymTable.h"

/**
 * 语法分析类
 * 接口为构造函数，需要vector<Token*>;
 */

class Parse
{
public:
	Parse ( vector<Token*> & v ) ;
	TreeNode * program() ;
	SymTable * getsymTable() ;
	void output() ;
private:
	TreeNode * root ;
	vector<Token*> tokens ;
	SymTable * symtable ;
	unsigned iter ;
	Token * token ;
	Token * getToken() ;
	void declarations() ;
	bool match( TypeKind expected ) ;
	TreeNode * stmt_sequence() ;
	TreeNode * statement() ;
	TreeNode * if_stmt() ;
	TreeNode * repeat_stmt() ;
	TreeNode * assign_stmt() ;
	TreeNode * read_stmt() ;
	TreeNode * write_stmt() ;
	TreeNode * while_stmt() ;
	TreeNode * exp() ;
	TreeNode * comparison_exp() ;
	TreeNode * arithmetic_exp() ;
	TreeNode * simple_exp() ;
	TreeNode * factor() ;
	void outputhelp( TreeNode * , int) ;
};