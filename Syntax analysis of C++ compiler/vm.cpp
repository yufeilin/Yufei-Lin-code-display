//Vm.cpp

#include "Vm.h"

#if 0
void VM::Init(vector<PcodeType*> pcodes , SymTable * symtable)
{
	this->pcodes = pcodes ; 
	IP = 0 ;
	this->symtable = symtable ;
	stack = new int[STACK_MAX_SIZE];
	SP = 0;
}

void VM::Run()
{
	PcodeType *c ;
	int opnd1 , opnd2 ;
	string param1 , param2 ;
	do
	{
		c = pcodes[IP++] ;	// 取一条指令
		string op = c->getop() ;
		if( op == "+" )
		{
			param1 = c->getparam(0) ;
			param2 = c->getparam(1) ;
		}
		else if( op == "-" )
		{
			//
		}
		else if( op == "*" )
		{
			//
		}
		else if( op == "/" )
		{
			//
		}
		else if( op == "" )
		{
			//
		}
		switch( c.type )
		{
		case CT_ADD:
			opnd1 = Pop();
			opnd2 = Pop();
			Push(opnd1+opnd2);
			break;
		case CT_SUB:
			opnd1 = Pop();
			opnd2 = Pop();
			Push(opnd1-opnd2);
			break;
		case CT_MUL:
			opnd1 = Pop();
			opnd2 = Pop();
			Push(opnd1*opnd2);
			break;
		case CT_DIV:
			opnd1 = Pop();
			opnd2 = Pop();
			Push(opnd1/opnd2);
			break;
		case CT_HALT:
			break;
		case CT_JMP:
			IP += c.opnd-1;		// 前面有个 IP++, 故这里需减1
			break;
		case CT_JZ:
			opnd1 = Pop();
			if(opnd1 == 0)
				IP += c.opnd-1;	// 前面有个 IP++, 故这里需减1
			break;
		case CT_JNZ:
			opnd1 = Pop();
			if(opnd1 != 0)
				IP += c.opnd-1;	// 前面有个 IP++, 故这里需减1
			break;
		case CT_IN:
			cin >> opnd1;
			Push(opnd1);
			break;
		case CT_OUT:
			opnd1 = Pop();
			cout << opnd1;
			break;
		case CT_OUTSTR:
			cout << staticData[c.opnd];
			break;
		case CT_LDC:
			Push(c.opnd);
			break;
		case CT_LDV:
			Push(globalData[c.opnd]);
			break;
		case CT_ST:
			globalData[c.opnd] = Pop();
			break;
		case default:
			printf("Bad CodeType\n");
			return;
		}
	}while(c.type != CT_HALT)
}

#endif