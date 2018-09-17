//Sym.h

#pragma once

#include "Token.h"

//����objtype����ȡ����ֵ��
enum ObjType
{
	OT_FUN,		// ����
	OT_VAR,		// ����
	OT_CONST,	// ����
};

//����valtype����ȡ����ֵ��
enum ValType
{
	VT_INT,		// ����������
	VT_BOOL,	// ��������
	VT_STRING,	// �ַ�������
	VT_CHAR ,	// �ַ�����
	VT_FLOAT	// ����������
};

//���ȶ���һ���ṹ���ŷ��ŵĸ������ԣ�
class Sym
{
public:
	Sym( Token * tk )
	{
		this->tk = tk ;
	}
	Token * getToken()
	{
		return tk ;
	}
	void setObjType( ObjType objtype )
	{
		this->objtype = objtype ;
	}
	ObjType getObjType()
	{
		return objtype ;
	}
	void setValType( ValType valtype )
	{
		this->valtype = valtype ;
	}
	ValType getValType()
	{
		return valtype ;
	}
private :
	Token *tk ;		// ����(�ʷ��������ķ���ֵ)
	ObjType objtype;	// ���Ŷ�������
	ValType valtype;	// ֵ����
	//int	addr;		// ��ַ
	//int	size;		// �ֽ���
	//int	level;		// ���
};
