//PcodeType.h

/**
 * »ã±à´úÂëÀà
 */

#include "Sym.h"

class PcodeType
{
public :
	PcodeType( int addr , string optype , string param1 , string param2 , string param3 )
	{
		this->optype = optype ;
		this->params[0] = param1 ;
		this->params[1] = param2 ;
		this->params[2] = param3 ;
		this->addr = addr ;
	}
	void setaddr( int addr )
	{
		this->addr = addr ;
	}
	void setlastparam( string param3 )
	{
		this->params[2] = param3 ;
	}
	string getop()
	{
		return optype ;
	}
	string getparam( int i )
	{
		if( i < 0 || i > 2 ) return NULL ;
		return params[i] ;
	}
	void output()
	{
		cout << addr << ") (" << optype ;
		cout << "," << params[0] ;
		cout << "," << params[1] ;
		cout << "," << params[2] ;
		cout << ")" << endl ;
	}
private:
	string optype ;
	string params[3] ;
	int addr ;
};