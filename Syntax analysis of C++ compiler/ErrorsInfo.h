//ErrorsInfo.h

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * ������Ϣ��
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

//������¼������Ϣ��ȫ�ֱ���
extern ErrorsInfo errorsInfo ;