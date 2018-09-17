//TreeNode.h

#pragma once

#include "Sym.h"

//�������
enum NodeType 
{
	PROGRAM,			// ���򣨿�ʼ���ţ��ڵ�
	STMT_SEQUENCE,		// ����б�ڵ�
	IF_STMT,			// �������ڵ�
	REPEAT_STMT,		// repeat���ڵ�
	ASSIGN_STMT,		// ��ֵ���ڵ�
	READ_STMT,			// read���ڵ�
	WRITE_STMT,			// write���ڵ�
	WHILE_STMT,			// while���ڵ�
	EQU_EXP,			// ���ڱ��ʽ�ڵ�
	GTR_EXP,			// ���ڱ��ʽ�ڵ�
	GEQ_EXP,			// ���ڵ��ڱ��ʽ�ڵ�
	LSS_EXP,			// С�ڱ��ʽ�ڵ�
	LEQ_EXP,			// С�ڵ��ڱ��ʽ�ڵ�
	LOG_OR_EXP,			// �߼�����ʽ�ڵ�
	LOG_AND_EXP,		// �߼�����ʽ�ڵ�
	LOG_NOT_EXP,		// �߼��Ǳ��ʽ�ڵ�
	ADD_EXP,			// �ӷ����ʽ�ڵ�
	SUB_EXP,			// �������ʽ�ڵ�
	MUL_EXP,			// �˷����ʽ�ڵ�
	DIV_EXP,			// �������ʽ�ڵ�
	FACTOR				// ԭ�ӽڵ�
};

//�﷨�����
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
	NodeType nodetype;	// �ڵ�����
	ValType valtype;	// �ڵ�ֵ����
	TreeNode *child[3];	// �ӽڵ��ָ��
	Token *tk;			// ���ڵ���FACTOR����ʱ�ó�Ա����Ч
} ;


