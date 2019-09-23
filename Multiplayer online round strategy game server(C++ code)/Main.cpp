#include <winsock2.h>//1 以后会用这种方式对特定代码进行注释
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>

#include "tinyxml2\tinyxml2.h"

#include "Global.h"

extern GameContext MainGameContext;

using namespace tinyxml2;

using namespace std;

#pragma comment(lib,"ws2_32.lib")  //2

int make_server_socket(int port)
{
	WSADATA inet_WsaData;//1
	WSAStartup(MAKEWORD(2, 0), &inet_WsaData);//1
	if (LOBYTE(inet_WsaData.wVersion) != 2 || HIBYTE(inet_WsaData.wVersion) != 0)//2 
	{
		WSACleanup();
		return -1;
	}

	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);//3
	SOCKET s;
	struct sockaddr_in saddr;//4
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if (::bind(tcp_socket, (const struct sockaddr*)&saddr, sizeof(saddr)) == -1)//5
	{
		cerr << "bind error" << endl;
		return -1;
	}
	if (::listen(tcp_socket, 5) == -1)//6
	{
		cerr << "listen error" << endl;
		return -1;
	}
	return tcp_socket;

}

void handleAccept(int socket_fd)
{
	cout << "Message received" << endl;

	char buf[1024] = { '\0' };	

	bool IsConnecting = true;

	while (IsConnecting)
	{
		buf[0] = '\0';

		cout << IsConnecting << endl;

		recv(socket_fd, buf, sizeof(buf), 0);//1
		stringstream sstream;//2
		sstream << buf;

		cout << buf << endl;

		string Message;

		sstream >> Message;

		//cout << Message << endl;

		if (Message == "DisConnect")
		{
			closesocket(socket_fd);
			cout << "Socket close." << endl;
			IsConnecting = false;
			return;
		}

		//cout << Message << endl;

		sstream >> Message;

		//cout << Message << endl;

		string NewMessage = "hahahahaha";

		send(socket_fd, NewMessage.c_str(), strlen(NewMessage.c_str()), 0);
	}
}


int main(int ac, char *av[])
{
	//Dictionary<Vector2Int, int> tmp;

	//Vector2Int m(1, 2);
	//Vector2Int n(1, 2);
	//
	//set<Vector2Int> s;

	//int aa = 1;
	//int bb = 2;
	//
	//int* ap = &aa;
	//int* bp = &aa;
	//
	//cout << (ap == bp) << endl;


	//IntParameter tmp;
	//int tt = 2;
	//cout << &tt << endl;
	//tmp.SetParameter(&3);
	//cout << *((int*)(tmp.GetParameter())) << endl;

	//Unit Tmp = Unit();
	//
	//cout << Tmp.GetMaxHealth() << endl;

	//Vector2Int s1(1, 2);
	//
	//vector<Vector2Int*> vv2;
	//
	//vv2.push_back(&s1);
	//
	//cout << (*vv2[0]).x <<" "<< (*vv2[0]).y << endl;
	//
	//s1.x = 7;
	//s1.y = 45;
	//cout << (*vv2[0]).x << " " << (*vv2[0]).y << endl;

	vector<PieceInstance> v1;

	PieceInstance NP;

	NP.SetPieceID("6laoshi");

	v1.push_back(NP);

	cout << v1[0].GetPieceID() << endl;

	NP.SetPieceID("33laoshi");

	cout << NP.GetPieceID() << endl;

	cout << v1[0].GetPieceID() << endl;

	tinyxml2::XMLDocument doc;

	if (!doc.LoadFile("XML/fdfdfd.xml"))
	{
	    cout << "12345" << endl;
	}

	tinyxml2::XMLElement* root = doc.FirstChildElement();

	if (root == NULL)
	{
		cout << "huaq" << endl;
	}

	for (tinyxml2::XMLElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		string elemName = elem->Value();
		const char* attr;
		attr = elem->Attribute("priority");
		if (strcmp(attr, "1") == 0)
		{
			tinyxml2::XMLElement* e1 = elem->FirstChildElement("bold");
			tinyxml2::XMLNode* e2 = e1->FirstChild();
		    cout << "priority=1\t" << e2->ToText()->Value() << endl;
		
		}
		else if (strcmp(attr, "2") == 0)
		{
			tinyxml2::XMLNode* e1 = elem->FirstChild();
		    cout << "priority=2\t" << e1->ToText()->Value() << endl;
		}
	}

	LibraryManager* NLM = new LibraryManager();

	delete NLM;

	MainGameContext.debug();

	DealDamage* sb = new DealDamage();

	delete sb;

	//NLM.LoadPiecePedestalFromXML();

	int tcp_socket = make_server_socket(8888);
	if (tcp_socket == -1)
	{
		exit(0);
	}
	thread t;//3
	while (1)
	{
		int socket_fd = accept(tcp_socket, nullptr, nullptr);  //4  
		t = thread(handleAccept, socket_fd);//3
		t.detach();//3
	}

	system("pause");
}