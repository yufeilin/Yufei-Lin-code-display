//ErrorsInfo.h

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * 错误消息类
 */
class ErrorsInfo
{
public:
	void getErrorsInfo( string str , int line ) ;
	void OuputErrorsInfo() ;
private:
	vector<string> str ;
	vector<int> line ;
};

//用来记录错误信息的全局变量
extern ErrorsInfo errorsInfo ;