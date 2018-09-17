//TreeNode.cpp

#include "TreeNode.h"

/**
 * 语法树构造函数
 */
TreeNode::TreeNode( NodeType nodetype , TreeNode * t1 , TreeNode * t2 , TreeNode * t3 )
{
	this->nodetype = nodetype ;
	child[0] = t1 ;
	child[1] = t2 ;
	child[2] = t3 ;
}

void TreeNode::setValType( ValType valtype )
{
	this->valtype = valtype ;
}

ValType TreeNode::getValType()
{
	return valtype ;
}

void TreeNode::setToken( Token * tk )
{
	this->tk = tk ;
}

Token * TreeNode::getToken()
{
	return tk ;
}

/**
 * 返回第i个孩子
 */
TreeNode * TreeNode::getChild( int i )
{
	if( i < 0 || i > 2 ) return NULL ;
	return child[i] ;
}

NodeType TreeNode::getNodeType()
{
	return nodetype ;
}