//TreeNode.h

#pragma once

#include "Sym.h"

//结点类型
enum NodeType 
{
	PROGRAM,			// 程序（开始符号）节点
	STMT_SEQUENCE,		// 语句列表节点
	IF_STMT,			// 条件语句节点
	REPEAT_STMT,		// repeat语句节点
	ASSIGN_STMT,		// 赋值语句节点
	READ_STMT,			// read语句节点
	WRITE_STMT,			// write语句节点
	WHILE_STMT,			// while语句节点
	EQU_EXP,			// 等于表达式节点
	GTR_EXP,			// 大于表达式节点
	GEQ_EXP,			// 大于等于表达式节点
	LSS_EXP,			// 小于表达式节点
	LEQ_EXP,			// 小于等于表达式节点
	LOG_OR_EXP,			// 逻辑或表达式节点
	LOG_AND_EXP,		// 逻辑与表达式节点
	LOG_NOT_EXP,		// 逻辑非表达式节点
	ADD_EXP,			// 加法表达式节点
	SUB_EXP,			// 减法表达式节点
	MUL_EXP,			// 乘法表达式节点
	DIV_EXP,			// 除法表达式节点
	FACTOR				// 原子节点
};

//语法树结点
class TreeNode
{
public:
	TreeNode( NodeType nodetype , TreeNode * t1 = NULL , TreeNode * t2 = NULL , TreeNode * t3 = NULL ) ;
	void setValType( ValType valtype ) ;
	ValType getValType() ;
	void setToken( Token * tk ) ;
	Token * getToken() ;
	TreeNode * getChild( int i ) ;
	NodeType getNodeType() ;
private:
	NodeType nodetype;	// 节点类型
	ValType valtype;	// 节点值类型
	TreeNode *child[3];	// 子节点的指针
	Token *tk;			// 当节点是FACTOR类型时该成员才有效
} ;


