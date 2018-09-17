//Vm.h

#pragma once

#include "Pcode.h"
#include "SymTable.h"

class VM
{
public:
	void Init(vector<PcodeType*> pcodes , SymTable * symtable) ;
	void Run();

private:
	int Pop() 
	{ 
		return stack[SP--]; 
	}
	void Push(int val) 
	{ 
		stack[++SP] = val; 
	}
	static const int STACK_MAX_SIZE = 2048;	// 栈的大小
	//PCode	*code;		// 代码区指针
	int		IP;		// 指令寄存器
	int		*stack; // 运行时栈
	int		SP;		// 栈顶寄存器
	//int		*globalData;	// 全局数据区
	//char	**staticData;	// 静态数据区
	vector<PcodeType*> pcodes ;	//代码
	SymTable * symtable ;
};
