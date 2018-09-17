//Lexer.cpp

#include "Lexer.h"
#include <iomanip>

//string转int
int str2int( string str )
{
	int result = 0 ;
	int n = 0 ;
	for( int i = str.length() - 1 ; i >= 0 ; i -- )
	{
		int temp = int( str[i] - '0' ) ;
		for( int j = 0 ; j < n ; j ++ )
			temp *= 10 ;
		result += temp ;
		n ++ ;
	}
	return result ;
}

map<string,TypeKind> Lexer::KindMap ;

/**
 * 构造函数
 * @param buffer[]	待分析数组
 * @param n			字符数组长度
 */
Lexer::Lexer( char buffer[] , int n )
{
	this->buffer = buffer ;
	this->n = n ;
	initKindMap() ;
}

//返回token数组
vector<Token*> Lexer::getTokens()
{
	return tokens ;
}

//构造映射表
void Lexer::initKindMap()
{
	KindMap.insert(map<string,TypeKind>::value_type("true", TK_TRUE)) ;
	KindMap.insert(map<string,TypeKind>::value_type("false", TK_FALSE)) ;
	KindMap.insert(map<string,TypeKind>::value_type("or", TK_OR)) ;
	KindMap.insert(map<string,TypeKind>::value_type("and", TK_AND)) ;
	KindMap.insert(map<string,TypeKind>::value_type("not", TK_NOT)) ;
	KindMap.insert(map<string,TypeKind>::value_type("int", TK_INT)) ;
	KindMap.insert(map<string,TypeKind>::value_type("char", TK_CHAR)) ;
	KindMap.insert(map<string,TypeKind>::value_type("float", TK_FLOAT)) ;
	KindMap.insert(map<string,TypeKind>::value_type("bool", TK_BOOL)) ;
	KindMap.insert(map<string,TypeKind>::value_type("string", TK_STRING)) ;
	KindMap.insert(map<string,TypeKind>::value_type("while", TK_WHILE)) ;
	KindMap.insert(map<string,TypeKind>::value_type("do", TK_DO)) ;
	KindMap.insert(map<string,TypeKind>::value_type("if", TK_IF)) ;
	KindMap.insert(map<string,TypeKind>::value_type("then", TK_THEN)) ;
	KindMap.insert(map<string,TypeKind>::value_type("else", TK_ELSE)) ;
	KindMap.insert(map<string,TypeKind>::value_type("end", TK_END)) ;
	KindMap.insert(map<string,TypeKind>::value_type("repeat", TK_REPEAT)) ;
	KindMap.insert(map<string,TypeKind>::value_type("until", TK_UNTIL)) ;
	KindMap.insert(map<string,TypeKind>::value_type("read", TK_READ)) ;
	KindMap.insert(map<string,TypeKind>::value_type("write", TK_WRITE)) ;
	KindMap.insert(map<string,TypeKind>::value_type(">", TK_GTR)) ;
	KindMap.insert(map<string,TypeKind>::value_type("<=", TK_LEQ)) ;
	KindMap.insert(map<string,TypeKind>::value_type(">=", TK_GEQ)) ;
	KindMap.insert(map<string,TypeKind>::value_type(",", TK_COMMA)) ;
	KindMap.insert(map<string,TypeKind>::value_type(";", TK_SEMICOLON)) ;
	KindMap.insert(map<string,TypeKind>::value_type(":=", TK_ASSIGN)) ;
	KindMap.insert(map<string,TypeKind>::value_type("+", TK_ADD)) ;
	KindMap.insert(map<string,TypeKind>::value_type("-", TK_SUB)) ;
	KindMap.insert(map<string,TypeKind>::value_type("*", TK_MUL)) ;
	KindMap.insert(map<string,TypeKind>::value_type("/", TK_DIV)) ;
	KindMap.insert(map<string,TypeKind>::value_type("(", TK_LP)) ;
	KindMap.insert(map<string,TypeKind>::value_type(")", TK_RP)) ;
	KindMap.insert(map<string,TypeKind>::value_type("<", TK_LSS)) ;
	KindMap.insert(map<string,TypeKind>::value_type("=", TK_EQU)) ;
}

bool Lexer::islegal( char ch )
{
	if( isDigit(ch) ) return true ;
	if( isLetter(ch) ) return true ;
	if( isSpace(ch) ) return true ;
	if( isSym(ch) ) return true ;
	return false ;
}

bool Lexer::isSym( char ch )
{
	switch( ch )
	{
	case '>':
	case '<':
	case ',':
	case '{':
	case '}':
	case ';':
	case ':':
	case '+':
	case '-':
	case '*':
	case '/':
	case '(':
	case ')':
	case '=':
	case '\'' :
		return true ;
	}
	return false ;
}

bool Lexer::isLetter( char ch )
{
	if( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <='Z') )
		return true ;
	else return false ;
}

bool Lexer::isDigit( char ch )
{
	if( ch >= '0' && ch <= '9' ) return true ;
	else return false ;
}

bool Lexer::isSpace( char ch )
{
	switch( ch )
	{
	case 0x20:
	case 0x09:
	case 0x0b:
	case 0x0c:
	case 0x0d:
	case 0x0a:
		return true ;
	default: 
		return false ;
	}
}

//词法分析
//TODO：对于各个部分分析的分离尚未解决
void Lexer::lexer()
{
	string tempstr = "" ;
	char tempchar ;
	int line = 1 ;
	int i = 0 ;
	while( i < n ) 
	{
		tempchar = buffer[i] ;

		//非法
		if( ! islegal(tempchar) ) 
		{//错误
			errorsInfo.getErrorsInfo( "非法字符" , line ) ;
			while( i < n && ! isSpace(tempchar) )
			{
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
				i ++ ;
			}
			continue ;
		}

		//空白字符
		if( isSpace( tempchar ) )
		{
			i ++ ;
			if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
			continue ;
		}

		//数字
		if( isDigit( tempchar ) )
		{
			tempstr += tempchar ;
			i ++ ;
			tempchar = buffer[i] ;
#if 1
			while( i < n && (isDigit(tempchar) || tempchar=='.') ) 
			{
				if( tempchar=='.')//float类型检测
				{
					tempstr += tempchar ;
					i ++ ;
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
					while( i < n && isDigit(tempchar) ) 
					{
						tempstr += tempchar ;
						i ++ ;
						tempchar = buffer[i] ;
						if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
					}
					break;//float类型检测到这里就break结束循环
				}
				//int类型
				tempstr += tempchar ;
				i ++ ;
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
			}
#endif
			if( isSym(tempchar) || isSpace(tempchar) )//再检测tempchar是否为符号或者空格
			{
				Token *temptoken = new Token( NUM , tempstr ) ;
#if 1 
				//修改点,将string类型转换为int类型
				int temnum = str2int(tempstr) ;
				temptoken->setNum(temnum) ;
#endif
				tokens.push_back( temptoken ) ;//将Token串放到tokens容器里
				tempstr = "" ;
				if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
				continue ;
			}
			//错误
			errorsInfo.getErrorsInfo( "非法数字" , line ) ;
			while( i < n && ! isSpace(tempchar) )
			{
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
				i ++ ;
			}
			tempstr = "" ;
			continue ;
		}

		//分析标识符&关键字
		if( isLetter(tempchar) )
		{
			while( i < n && ( isLetter(tempchar) || isDigit(tempchar) ) )
			{
				tempstr += tempchar ;
				i ++ ;
				tempchar = buffer[i] ;
			}
			if( ! isSpace(tempchar) && ! isSym(tempchar) && i < n )
			{
				//出错
				errorsInfo.getErrorsInfo( "非法ID" , line ) ;
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
					i ++ ;
				}
				tempstr = "" ;
				continue ;
			}
			map<string,TypeKind>::iterator it = KindMap.find(tempstr) ;//调用map的find函数查找tempstr是否有在map中有对应的KEY
			if( it != KindMap.end() )//tempstr在map中有对应的KEY，那么就是关键字
			{
				Token *temptoken = new Token( it->second , tempstr ) ;
				tokens.push_back( temptoken ) ;
			}
			else//tempstr在map中没有对应的KEY，那么就是ID
			{
				Token *temptoken = new Token( ID , tempstr ) ;
				tokens.push_back( temptoken ) ;
			}
			tempstr = "" ;
			//if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
			if( tempchar=='\''|| tempchar=='{' || tempchar=='}') continue;
			continue ;
		}

		//分析字符串
		if( tempchar == '\'' )
		{
			i ++ ;
			if ( i == n )
			{
				//出错
				errorsInfo.getErrorsInfo( "缺少\"'\"" , line ) ;
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
					i ++ ;
				}
				continue ;
			}
			tempchar = buffer[i] ;
			while( i < n && tempchar != '\'' && tempchar != 0x0A )
			{
				tempstr += tempchar ;
				i ++ ;
				if ( i == n ) break ;
				tempchar = buffer[i] ;
			}
			if( tempchar != '\'' )
			{
				//出错
				errorsInfo.getErrorsInfo( "缺少\"'\"" , line ) ;
				if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					i ++ ;
				}
				tempstr = "" ;
				i ++ ;
				continue ;
			}
			Token *temptoken = new Token( STR , tempstr ) ;
			tokens.push_back( temptoken ) ;
			tempstr = "" ;
			i++;
			continue ;
		}

		//分析注释
		if( tempchar == '}' )
		{
			//出错
			errorsInfo.getErrorsInfo( "缺少\"{\"" , line ) ;
			while( i < n && ! isSpace(tempchar) )
			{
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
				i ++ ;
			}
			continue ;
		}
		if( tempchar == '{' )
		{
			i ++ ;
			if( i == n ) 
			{
				//出错
				errorsInfo.getErrorsInfo( "缺少\"}\"" , line ) ;
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
					i ++ ;
				}
			}
			tempchar = buffer[i] ;
			while( tempchar != '}' && tempchar != 0x0A )
			{
				i ++ ;
				if( i == n ) break ;
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
			}
			if( tempchar != '}' )
			{
				//出错
				errorsInfo.getErrorsInfo( "缺少\"}\"" , line ) ;
				if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					i ++ ;
			continue ;
				}
				tempstr = "" ;
				i ++ ;
				continue ;
			}
			i ++ ;
		}

		//分析操作符
		if( isSym(tempchar) )
		{
			tempstr += tempchar ;
			i ++ ;
			if( tempchar == ':' )
			{
				if ( i == n )
				{
					//出错
					errorsInfo.getErrorsInfo( "非法符号\":\"" , line ) ;
					while( i < n && ! isSpace(tempchar) )
					{
						tempchar = buffer[i] ;
						i ++ ;
					}
					tempstr = "" ;
					continue ;
				}
				tempchar = buffer[i] ;
				if( tempchar != '=' )
				{
					//出错
					errorsInfo.getErrorsInfo( "非法符号\":\"" , line ) ;
					if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
					while( i < n && ! isSpace(tempchar) )
					{
						tempchar = buffer[i] ;
						if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
						i ++ ;
					}
					tempstr = "" ;
					continue ;
				}
				tempstr += tempchar ;
				Token *temptoken = new Token( TK_ASSIGN , tempstr ) ;
				tokens.push_back( temptoken ) ;
				tempstr = "" ;
				i ++ ;
				continue ;
			}
			if( tempchar == '>' )
			{
				tempchar = buffer[i] ;
				if( i < n && tempchar != '=' )
				{
					Token *temptoken = new Token( TK_GTR , tempstr ) ;
					tokens.push_back( temptoken ) ;
					tempstr = "" ;
					continue ;
				}
				tempstr += tempchar ;
				Token *temptoken = new Token( TK_GEQ , tempstr ) ;
				tokens.push_back( temptoken ) ;
				tempstr = "" ;
				i ++ ;
				continue ;
			}
			if( tempchar == '<' )
			{
				tempchar = buffer[i] ;
				if( i < n && tempchar != '=' )
				{
					Token *temptoken = new Token( TK_LSS , tempstr ) ;
					tokens.push_back( temptoken ) ;
					tempstr = "" ;
					continue ;
				}
				tempstr += tempchar ;
				Token *temptoken = new Token( TK_LEQ , tempstr ) ;
				tokens.push_back( temptoken ) ;
				tempstr = "" ;
				i ++ ;
				continue ;
			}
			map<string,TypeKind>::iterator it = KindMap.find(tempstr) ;
			if( it != KindMap.end() )
			{
				Token *temptoken = new Token( it->second , tempstr ) ;
				tokens.push_back( temptoken ) ;
				tempstr = "" ;
				continue ;
			}
			else
			{
				//没找到
				errorsInfo.getErrorsInfo( "非法符号" + tempstr , line ) ;
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar等于换行
					i ++ ;
				}
				tempstr = "" ;
				continue ;
			}
		}
	}
}

//输出词法分析结果Token
void Lexer::Output() 
{
	for( int i = 0 ; i < tokens.size() ; i ++ )
	{
		cout << *tokens[i] << '\t' ;
		if((i+1)%4==0) cout<<endl;
	}
	cout << endl ;
	//输出错误信息
	//errorsInfo.OuputErrorsInfo() ;
}