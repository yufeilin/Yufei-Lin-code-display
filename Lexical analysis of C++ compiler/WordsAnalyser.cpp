#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
using namespace std;

int Test(char[200]);
int AddIdentifier(char[20]);
int FindIdentifier(char[20]);
bool Isnum(char[20]);   //判断是否合法标识符

void Load(char[30]);
void WordSplit(char[30]);
void Display(char[200], int[10000], int&);
void Errors(int[10000], int&);

char Identifiers[1000][50];
int Counts;      //记录存储字符常量个数
int huanhang=0;  //记录每一行输出多少个了
bool isStr=false;//用于判断转义字符串是ID还是STR，因为STR的单引号没有放到word数组里

int main(){

	char filepath[30];
	cout<<"请输入要分析的程序文件名称：";
	while(cin>>filepath){
		Load(filepath);       //对指定文件进行词法分析
		cout<<'\n'<<'\n';
		cout<<"请输入要分析的程序文件名称：";
	}
	return 0;
}

int Test(char word[200]){
	char Keys[36][20]={"", "true", "false", "or", "and", "not", "int", "bool", "string", "while", "do", "if", "then", "else", "end", "repeat", "until", "read", "write", ">", "<=", ">=", ",", "'", "{", "}", ";", ":=", "+", "-", "*", "/", "(", ")", "<", "="};

	if(word[0]>='0'&&word[0]<='9'){
		if (word[0] == '0' && (word[1] == 'x' || word[1] == 'X'))
		{
			for (int i = 2; i < strlen(word)-1; i++)
				if (!(word[i]>='a'&&word[i]<='f'||word[i]>='A'&&word[i]<='F'||word[i]>='0'&&word[i]<='9'))
					return -6;//错误的十六进制数
			return 37;
		}
		else if(!Isnum(word))
			return -6;      //非法标示符，以数字开头，返回-2
		else return 37;      //识别为NUM
	}
	
	for(int j=1; j<36; j++){                       //识别为KEY
		if(strcmp(Keys[j], word)==0)
			return j;
	}
	if (word[0] == '\\'&& isStr==true)//识别STR出现“\”，也认为是STR 
		return 38;
	if (word[0] == '\\'&& isStr==false)//识别转义字符
		return 40;
	if(!(word[0]>='a'&&word[0]<='z'||word[0]>='A'&&word[0]<='Z'||word[0]>='0'&&word[0]<='9'||word[0]=='\''))
		return -1;
	if (word[0] != '\'' && word[strlen(word)-1] == '\'')//缺少左单引号
		return -2;
	if (word[0] == '\'' && word[strlen(word)-1] != '\'')//缺少右单引号
		return -3;
	if (word[0] != '{' && word[strlen(word)-1] == '}')//缺少左大括号
		return -4;
	if (word[0] == '{' && word[strlen(word)-1] != '}')//缺少右大括号
		return -5;
	if(isStr==false&&((word[0]>='a'&&word[0]<='z'||word[0]>='A'&&word[0]<='Z')))//识别为ID
		return 36;
	if (isStr==true)//识别为STR
		return 38;
	if (word[0] == '{' && word[strlen(word)-1] == '}')//识别为注释
		return 39;
	
	return 50;//未识别符号
}


bool Isnum(char word[20]){
	for(int i=1; word[i]!='\0'; i++){
		if(word[i]<'0'||word[i]>'9')
			return false;
	}
	return true;
}

void Load(char FilePath[30]){
	Counts=0;     //清空之前字符常量记录
	WordSplit(FilePath);
}

void WordSplit(char fpath[30])
{
	ifstream inf(fpath, ios::in);//打开文件
	int row[10000]={0};  //记录每一行是否有错
	int r=1;             //记录行数
	char ch;
	ch=inf.get();
	int c=0;             //记录输出个数
	while(ch!=EOF)      //文件尾		
	{
		isStr=false;
		while(ch==' '||ch=='\t')
			ch=inf.get();     //过滤空格
		while(ch=='\n')
		{
			r++;             //换行
			ch=inf.get();
		}
		int i=0;
		char word[200]={' '};

		if(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z')
		{
			//##############首位为字母时###################
			while(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z'||ch>='0'&&ch<='9'||ch=='\''||ch=='}')
			{
				word[i++]=ch;
				ch=inf.get();
				if(ch==EOF) break;
			}
			word[i]='\0';
			Display(word, row, r);
		}
		else
		{
			if(ch>='0'&&ch<='9')
			{
				//##################首位为数字时###########################
				while(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z'||ch>='0'&&ch<='9')
				{
					word[i++]=ch;
					ch=inf.get();
					if(ch==EOF) break;
				}
				word[i]='\0';
				Display(word, row, r);
			}
			else
			{
				//########################首位为符号时#######################
				if(ch!=' '&&ch!='\n'&&ch!=EOF&&ch!='\t')
				{
					bool flag=true;       //标记是否需要输出，正确时或非注释时输出，否则不输出
					bool judge=false;     //标志单引号或者花括号是否成对出现
					bool zhuanyi=false;   //标志转移字符是否正确
					switch(ch){
						case ':':
							{
								word[i++]=ch;
								ch=inf.get();
								if(ch=='='){
									word[i++]=ch;
									ch=inf.get();
								}
							}
							break;
						case '<':
							{
								word[i++]=ch;
								ch=inf.get();
								if(ch=='='){
									word[i++]=ch;
									ch=inf.get();
								}
							}
							break;
						case '>':
							{
								word[i++]=ch;
								ch=inf.get();
								if(ch=='='){
									word[i++]=ch;
									ch=inf.get();
								}
							}
							break;
						case '{':
							{
								word[i++]=ch;
								ch=inf.get();
								while(ch!='}'&&ch!='\n')
								{

									ch=inf.get();
									if(ch=='}') judge=true;
									if(ch==EOF) break;
								}
								if(judge==false)
								{
										row[r]=-5;        //错误-5：非法注释符，注释符不匹对
										flag=false;     //标记无需输出
								}
								else
								{
										ch=inf.get();   //读取下一个字符
										flag=false;     //标记无需输出
								}
							}
							break;
						case '\''://单引号
							{
								ch=inf.get();						
								while(ch!='\''&&ch!='\n')
								{
									word[i++]=ch;
									ch=inf.get();
									if(ch=='\'')  { judge=true;isStr=true;}
									if(ch==EOF) break;
								}										
								if(judge==false)
								{
									row[r]=-3;         //错误-1：非法字符常量，缺省单引号
									flag=false;     //标记无需输出
								}
								else
								{
									//此时ch=='\''
									ch=inf.get();    //读取下一个字符
								}
							}
							break;
						case '\\':
							{		
								word[i++]=ch;
								ch=inf.get();
								while(ch!=' '&&ch!='\n'&&ch!='\t')
								{	
									if(ch=='\''||ch=='n'||ch=='t'||ch=='\\'||ch=='a'||ch=='b'||ch=='f'||ch=='r'||ch=='v'||ch=='0')
									{
										word[i++]=ch;
										ch=inf.get();
										if(ch==' '||ch=='\n'||ch=='\t'||ch==EOF)
											zhuanyi=true;
									}
									else	ch=inf.get();
									if(zhuanyi==true) judge=true;
									if(ch==EOF) break;
								}
								if(judge==false)
								{
										row[r]=-7;        //错误-7：非法转义字符
										flag=false;     //标记无需输出
								}
								else
								{
										ch=inf.get();   //读取下一个字符
								}
							}
							break;
						default:
							{
								word[i++]=ch;
								ch=inf.get();
							}
							break;
					}
					if(flag){
						word[i]='\0';
						Display(word, row, r);
					}
				}
			}
		}


	}
	cout<<endl;
	Errors(row, r);
}

void Display(char word[200], int row[10000], int & r){
	int t=Test(word);
	int length=strlen(word);
	word[length]=')';
	word[length+1]='\0';
	if(t>=0&&t<41) huanhang++;
	if(t<0)		row[r]=t;     //记录每行的错误类型
	else if(t<=18)
	{
		cout<<"(KEY,"<<setiosflags(ios::left)<<setw(10)<<word;
		if(huanhang%5==0) cout<<endl;
	}
	else if (t<36) 
	{
		cout<<"(SYM,"<<setiosflags(ios::left)<<setw(10)<<word;
		if(huanhang%5==0) cout<<endl;
	}
	if (t==36)
	{
		cout<<"(ID,"<<setiosflags(ios::left)<<setw(10)<<word;
		cout<<" ";
		if(huanhang%5==0) cout<<endl;
	}
	if (t==37)
	{
		cout<<"(NUM,"<<setiosflags(ios::left)<<setw(10)<<word;
		if(huanhang%5==0) cout<<endl;
	}
	if (t==38)
	{
		cout<<"(STR,"<<setiosflags(ios::left)<<setw(10)<<word;
		if(huanhang%5==0) cout<<endl;
	}
	if(t==40)
	{
		cout<<"(转义字符,"<<setiosflags(ios::left)<<setw(10)<<word;
		if(huanhang%5==0) cout<<endl;
	}
}
void Errors(int Rows[10000], int & r){
	bool flag=false;

	cout<<"错误信息："<<endl;

	for(int i=1; i<=r; i++){
		if(Rows[i]<0){
			flag=true;
			cout<<"第"<<i<<"行：";
			switch(Rows[i]){
				case -1: 
					cout<<"非法符号，未能识别。"<<endl;
					break;
				case -2:
					cout<<"缺少左单引号。"<<endl;
					break;
				case -3:
					cout<<"缺少右单引号。"<<endl;
					break;
				case -4:
					cout<<"缺少左大括号。"<<endl;
					break;
				case -5:
					cout<<"缺少右大括号。"<<endl;
					break;
				case -6:
					cout<<"非法标识符，以数字开头。"<<endl;
					break;
				case -7:
					cout<<"非法转义字符。"<<endl;
			}
		}
	}
	if(flag==false)
		cout<<"无"<<endl;
}