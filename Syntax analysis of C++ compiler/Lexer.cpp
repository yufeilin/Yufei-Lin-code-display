//Lexer.cpp

#include "Lexer.h"
#include <iomanip>

//stringתint
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
 * ���캯��
 * @param buffer[]	����������
 * @param n			�ַ����鳤��
 */
Lexer::Lexer( char buffer[] , int n )
{
	this->buffer = buffer ;
	this->n = n ;
	initKindMap() ;
}

//����token����
vector<Token*> Lexer::getTokens()
{
	return tokens ;
}

//����ӳ���
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

//�ʷ�����
//TODO�����ڸ������ַ����ķ�����δ���
void Lexer::lexer()
{
	string tempstr = "" ;
	char tempchar ;
	int line = 1 ;
	int i = 0 ;
	while( i < n ) 
	{
		tempchar = buffer[i] ;

		//�Ƿ�
		if( ! islegal(tempchar) ) 
		{//����
			errorsInfo.getErrorsInfo( "�Ƿ��ַ�" , line ) ;
			while( i < n && ! isSpace(tempchar) )
			{
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
				i ++ ;
			}
			continue ;
		}

		//�հ��ַ�
		if( isSpace( tempchar ) )
		{
			i ++ ;
			if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
			continue ;
		}

		//����
		if( isDigit( tempchar ) )
		{
			tempstr += tempchar ;
			i ++ ;
			tempchar = buffer[i] ;
#if 1
			while( i < n && (isDigit(tempchar) || tempchar=='.') ) 
			{
				if( tempchar=='.')//float���ͼ��
				{
					tempstr += tempchar ;
					i ++ ;
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
					while( i < n && isDigit(tempchar) ) 
					{
						tempstr += tempchar ;
						i ++ ;
						tempchar = buffer[i] ;
						if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
					}
					break;//float���ͼ�⵽�����break����ѭ��
				}
				//int����
				tempstr += tempchar ;
				i ++ ;
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
			}
#endif
			if( isSym(tempchar) || isSpace(tempchar) )//�ټ��tempchar�Ƿ�Ϊ���Ż��߿ո�
			{
				Token *temptoken = new Token( NUM , tempstr ) ;
#if 1 
				//�޸ĵ�,��string����ת��Ϊint����
				int temnum = str2int(tempstr) ;
				temptoken->setNum(temnum) ;
#endif
				tokens.push_back( temptoken ) ;//��Token���ŵ�tokens������
				tempstr = "" ;
				if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
				continue ;
			}
			//����
			errorsInfo.getErrorsInfo( "�Ƿ�����" , line ) ;
			while( i < n && ! isSpace(tempchar) )
			{
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
				i ++ ;
			}
			tempstr = "" ;
			continue ;
		}

		//������ʶ��&�ؼ���
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
				//����
				errorsInfo.getErrorsInfo( "�Ƿ�ID" , line ) ;
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
					i ++ ;
				}
				tempstr = "" ;
				continue ;
			}
			map<string,TypeKind>::iterator it = KindMap.find(tempstr) ;//����map��find��������tempstr�Ƿ�����map���ж�Ӧ��KEY
			if( it != KindMap.end() )//tempstr��map���ж�Ӧ��KEY����ô���ǹؼ���
			{
				Token *temptoken = new Token( it->second , tempstr ) ;
				tokens.push_back( temptoken ) ;
			}
			else//tempstr��map��û�ж�Ӧ��KEY����ô����ID
			{
				Token *temptoken = new Token( ID , tempstr ) ;
				tokens.push_back( temptoken ) ;
			}
			tempstr = "" ;
			//if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
			if( tempchar=='\''|| tempchar=='{' || tempchar=='}') continue;
			continue ;
		}

		//�����ַ���
		if( tempchar == '\'' )
		{
			i ++ ;
			if ( i == n )
			{
				//����
				errorsInfo.getErrorsInfo( "ȱ��\"'\"" , line ) ;
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
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
				//����
				errorsInfo.getErrorsInfo( "ȱ��\"'\"" , line ) ;
				if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
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

		//����ע��
		if( tempchar == '}' )
		{
			//����
			errorsInfo.getErrorsInfo( "ȱ��\"{\"" , line ) ;
			while( i < n && ! isSpace(tempchar) )
			{
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
				i ++ ;
			}
			continue ;
		}
		if( tempchar == '{' )
		{
			i ++ ;
			if( i == n ) 
			{
				//����
				errorsInfo.getErrorsInfo( "ȱ��\"}\"" , line ) ;
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
					i ++ ;
				}
			}
			tempchar = buffer[i] ;
			while( tempchar != '}' && tempchar != 0x0A )
			{
				i ++ ;
				if( i == n ) break ;
				tempchar = buffer[i] ;
				if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
			}
			if( tempchar != '}' )
			{
				//����
				errorsInfo.getErrorsInfo( "ȱ��\"}\"" , line ) ;
				if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
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

		//����������
		if( isSym(tempchar) )
		{
			tempstr += tempchar ;
			i ++ ;
			if( tempchar == ':' )
			{
				if ( i == n )
				{
					//����
					errorsInfo.getErrorsInfo( "�Ƿ�����\":\"" , line ) ;
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
					//����
					errorsInfo.getErrorsInfo( "�Ƿ�����\":\"" , line ) ;
					if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
					while( i < n && ! isSpace(tempchar) )
					{
						tempchar = buffer[i] ;
						if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
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
				//û�ҵ�
				errorsInfo.getErrorsInfo( "�Ƿ�����" + tempstr , line ) ;
				while( i < n && ! isSpace(tempchar) )
				{
					tempchar = buffer[i] ;
					if( tempchar == 0X0A ) line ++ ;//tempchar���ڻ���
					i ++ ;
				}
				tempstr = "" ;
				continue ;
			}
		}
	}
}

//����ʷ��������Token
void Lexer::Output() 
{
	for( int i = 0 ; i < tokens.size() ; i ++ )
	{
		cout << *tokens[i] << '\t' ;
		if((i+1)%4==0) cout<<endl;
	}
	cout << endl ;
	//���������Ϣ
	//errorsInfo.OuputErrorsInfo() ;
}