//main.cpp

/**
 * TINY+
 * @author:Fantom
 * @version:1.0.11.20
 */

#include <iostream>
#include <cstdio>

#include "Lexer.h"
#include "Parse.h"
#include "Pcode.h"

using namespace std ;

#define	DEBUG_ 1
#define SCANF_BUFFER 1
#define OUTPUT_BUFFER 1
#define LEXER_ 1
#define LEXER_RESULT 1
#define PARSING_ 1
#define PARSING_RESULT 1
#define P_CODE 1
#define	P_CODE_RESULT 1
#define VM_ENABLE	0

#define CHAR_NUM 100000

int main()
{
	int count = 0 ;
	char buffer[CHAR_NUM] ;
#if DEBUG_
	freopen( "in.txt" , "r" , stdin ) ;
#endif

#if SCANF_BUFFER
	//get buffer from txt
	while( scanf( "%c" , &buffer[count] ) == 1 )
	{
		count ++ ;
	}
#endif

#if	OUTPUT_BUFFER
	cout << "Input File: \n";
	for( int i = 0 ; i < count ; i ++ )
	{
		cout << buffer[i] ;
	}
	cout << endl << endl;
#endif

#if	LEXER_	
	Lexer *aLexer = new Lexer( buffer , count) ;
	aLexer->lexer() ;

#if LEXER_RESULT
	cout << "Lexer Result: \n";
	aLexer->Output() ;
	cout << endl;
#endif

#if PARSING_
	Parse *parse = new Parse( aLexer->getTokens() ) ;
	TreeNode *root = parse->program() ;
#if PARSING_RESULT
	cout << "PRASING RESULT: \n";
	parse->output() ;
	cout << endl;
#endif

	errorsInfo.OuputErrorsInfo() ;

#if P_CODE
	Pcode *pCode = new Pcode( root ) ;
	pCode->getpcode() ;
#if P_CODE_RESULT
	cout << "Assembly Code: \n";
	pCode->output() ;

#if VM_ENABLE
	VM *vm ;
	vm->Run() ;
#endif 

#endif
#endif

#endif

#endif
	return 0 ;
}