//Sym.h

#pragma once

#include "Token.h"

//其中objtype可以取以下值：
enum ObjType
{
	OT_FUN,		// 函数
	OT_VAR,		// 变量
	OT_CONST,	// 常量
};

//其中valtype可以取以下值：
enum ValType
{
	VT_INT,		// 整型数类型
	VT_BOOL,	// 布尔类型
	VT_STRING,	// 字符串类型
	VT_CHAR ,	// 字符类型
	VT_FLOAT	// 浮点数类型
};

//首先定义一个结构体存放符号的各种属性：
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
	Token *tk ;		// 符号(词法分析器的返回值)
	ObjType objtype;	// 符号对象类型
	ValType valtype;	// 值类型
	//int	addr;		// 地址
	//int	size;		// 字节数
	//int	level;		// 层次
};
