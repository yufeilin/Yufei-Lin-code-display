//ErrorsInfo.cpp

#include "ErrorsInfo.h"

#ifndef ERRORS_INFO
#define ERRORS_INFO
//用来记录错误信息的全局变量
ErrorsInfo errorsInfo ;
#endif

/**
 * 获取错误信息
 * @param str	错误信息
 * @param line	错误所在行
 */
void ErrorsInfo::getErrorsInfo( string str , int line )
{
	this->str.push_back( str ) ;
	this->line.push_back( line ) ;
}

/**
 *输出错误信息
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