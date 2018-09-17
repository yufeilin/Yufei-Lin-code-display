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
	static const int STACK_MAX_SIZE = 2048;	// ջ�Ĵ�С
	//PCode	*code;		// ������ָ��
	int		IP;		// ָ��Ĵ���
	int		*stack; // ����ʱջ
	int		SP;		// ջ���Ĵ���
	//int		*globalData;	// ȫ��������
	//char	**staticData;	// ��̬������
	vector<PcodeType*> pcodes ;	//����
	SymTable * symtable ;
};
