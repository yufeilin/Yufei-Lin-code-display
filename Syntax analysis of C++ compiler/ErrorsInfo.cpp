//ErrorsInfo.cpp

#include "ErrorsInfo.h"

#ifndef ERRORS_INFO
#define ERRORS_INFO
//������¼������Ϣ��ȫ�ֱ���
ErrorsInfo errorsInfo ;
#endif

/**
 * ��ȡ������Ϣ
 * @param str	������Ϣ
 * @param line	����������
 */
void ErrorsInfo::getErrorsInfo( string str , int line )
{
	this->str.push_back( str ) ;
	this->line.push_back( line ) ;
}

/**
 *���������Ϣ
 */
void ErrorsInfo::OuputErrorsInfo()
{
	for( int i = 0 ; i != str.size() ; i ++ )
	{
		if( line[i] == 0 )
		{
			cout << "Parse Errors:" << str[i] << '!' << endl ;
			return ;
		}
		cout << "Errors:line" << line[i] << ' ' << str[i] << '!' << endl ;
	}
}