#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
using namespace std;

int Test(char[200]);
int AddIdentifier(char[20]);
int FindIdentifier(char[20]);
bool Isnum(char[20]);   //�ж��Ƿ�Ϸ���ʶ��

void Load(char[30]);
void WordSplit(char[30]);
void Display(char[200], int[10000], int&);
void Errors(int[10000], int&);

char Identifiers[1000][50];
int Counts;      //��¼�洢�ַ���������
int huanhang=0;  //��¼ÿһ��������ٸ���
bool isStr=false;//�����ж�ת���ַ�����ID����STR����ΪSTR�ĵ�����û�зŵ�word������

int main(){

	char filepath[30];
	cout<<"������Ҫ�����ĳ����ļ����ƣ�";
	while(cin>>filepath){
		Load(filepath);       //��ָ���ļ����дʷ�����
		cout<<'\n'<<'\n';
		cout<<"������Ҫ�����ĳ����ļ����ƣ�";
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
					return -6;//�����ʮ��������
			return 37;
		}
		else if(!Isnum(word))
			return -6;      //�Ƿ���ʾ���������ֿ�ͷ������-2
		else return 37;      //ʶ��ΪNUM
	}
	
	for(int j=1; j<36; j++){                       //ʶ��ΪKEY
		if(strcmp(Keys[j], word)==0)
			return j;
	}
	if (word[0] == '\\'&& isStr==true)//ʶ��STR���֡�\����Ҳ��Ϊ��STR 
		return 38;
	if (word[0] == '\\'&& isStr==false)//ʶ��ת���ַ�
		return 40;
	if(!(word[0]>='a'&&word[0]<='z'||word[0]>='A'&&word[0]<='Z'||word[0]>='0'&&word[0]<='9'||word[0]=='\''))
		return -1;
	if (word[0] != '\'' && word[strlen(word)-1] == '\'')//ȱ��������
		return -2;
	if (word[0] == '\'' && word[strlen(word)-1] != '\'')//ȱ���ҵ�����
		return -3;
	if (word[0] != '{' && word[strlen(word)-1] == '}')//ȱ���������
		return -4;
	if (word[0] == '{' && word[strlen(word)-1] != '}')//ȱ���Ҵ�����
		return -5;
	if(isStr==false&&((word[0]>='a'&&word[0]<='z'||word[0]>='A'&&word[0]<='Z')))//ʶ��ΪID
		return 36;
	if (isStr==true)//ʶ��ΪSTR
		return 38;
	if (word[0] == '{' && word[strlen(word)-1] == '}')//ʶ��Ϊע��
		return 39;
	
	return 50;//δʶ�����
}


bool Isnum(char word[20]){
	for(int i=1; word[i]!='\0'; i++){
		if(word[i]<'0'||word[i]>'9')
			return false;
	}
	return true;
}

void Load(char FilePath[30]){
	Counts=0;     //���֮ǰ�ַ�������¼
	WordSplit(FilePath);
}

void WordSplit(char fpath[30])
{
	ifstream inf(fpath, ios::in);//���ļ�
	int row[10000]={0};  //��¼ÿһ���Ƿ��д�
	int r=1;             //��¼����
	char ch;
	ch=inf.get();
	int c=0;             //��¼�������
	while(ch!=EOF)      //�ļ�β		
	{
		isStr=false;
		while(ch==' '||ch=='\t')
			ch=inf.get();     //���˿ո�
		while(ch=='\n')
		{
			r++;             //����
			ch=inf.get();
		}
		int i=0;
		char word[200]={' '};

		if(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z')
		{
			//##############��λΪ��ĸʱ###################
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
				//##################��λΪ����ʱ###########################
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
				//########################��λΪ����ʱ#######################
				if(ch!=' '&&ch!='\n'&&ch!=EOF&&ch!='\t')
				{
					bool flag=true;       //����Ƿ���Ҫ�������ȷʱ���ע��ʱ������������
					bool judge=false;     //��־�����Ż��߻������Ƿ�ɶԳ���
					bool zhuanyi=false;   //��־ת���ַ��Ƿ���ȷ
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
										row[r]=-5;        //����-5���Ƿ�ע�ͷ���ע�ͷ���ƥ��
										flag=false;     //����������
								}
								else
								{
										ch=inf.get();   //��ȡ��һ���ַ�
										flag=false;     //����������
								}
							}
							break;
						case '\''://������
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
									row[r]=-3;         //����-1���Ƿ��ַ�������ȱʡ������
									flag=false;     //����������
								}
								else
								{
									//��ʱch=='\''
									ch=inf.get();    //��ȡ��һ���ַ�
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
										row[r]=-7;        //����-7���Ƿ�ת���ַ�
										flag=false;     //����������
								}
								else
								{
										ch=inf.get();   //��ȡ��һ���ַ�
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
	if(t<0)		row[r]=t;     //��¼ÿ�еĴ�������
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
		cout<<"(ת���ַ�,"<<setiosflags(ios::left)<<setw(10)<<word;
		if(huanhang%5==0) cout<<endl;
	}
}
void Errors(int Rows[10000], int & r){
	bool flag=false;

	cout<<"������Ϣ��"<<endl;

	for(int i=1; i<=r; i++){
		if(Rows[i]<0){
			flag=true;
			cout<<"��"<<i<<"�У�";
			switch(Rows[i]){
				case -1: 
					cout<<"�Ƿ����ţ�δ��ʶ��"<<endl;
					break;
				case -2:
					cout<<"ȱ�������š�"<<endl;
					break;
				case -3:
					cout<<"ȱ���ҵ����š�"<<endl;
					break;
				case -4:
					cout<<"ȱ��������š�"<<endl;
					break;
				case -5:
					cout<<"ȱ���Ҵ����š�"<<endl;
					break;
				case -6:
					cout<<"�Ƿ���ʶ���������ֿ�ͷ��"<<endl;
					break;
				case -7:
					cout<<"�Ƿ�ת���ַ���"<<endl;
			}
		}
	}
	if(flag==false)
		cout<<"��"<<endl;
}