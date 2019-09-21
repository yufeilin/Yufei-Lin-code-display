#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> 
#include<string.h>
#include<wait.h>
#define NDEBUG
#define MAX_CMD_LEN 256//the max length of shell commands
#define MAX_TOKEN_NUM 80//the largest number of tokens
#define HOME "/"//home directory of shell
void printCwd(){/*{{{*/
	long size;
	char *buf;
	char *cwd;
	size=pathconf(".",_PC_PATH_MAX);
	if((buf=(char *)malloc((size_t)size))!=NULL)
		cwd=getcwd(buf,(size_t)size);
	printf("%s$",cwd);
}/*}}}*/
void myshell(){/*{{{*/
	printCwd();
	char cmd[MAX_CMD_LEN+1];//store a command line
	char *token[MAX_TOKEN_NUM+1];//store tokens from command
	int token_num;//number of tokens
	int DaemonToggle=0;//daemon toggle
	int i=-1;
	do{
		i++;
		cmd[i]=getchar();
	}while(cmd[i]!='\n'&&i<=MAX_CMD_LEN);
	cmd[i]='\0';
	if(i==0){//no input
		return;
	}
	#ifndef NDEBUG
		printf("DEBUG--%d\t\tcommand:%s\n",__LINE__,cmd);
	#endif
	token[0]=strtok(cmd," ");
	for(i=1;token[i-1]!=NULL&&i<MAX_TOKEN_NUM;i++){
		token[i]=strtok(NULL," ");
		#ifndef NDEBUG
			printf("DEBUG--%d\t\ttoken[%d]:%s\n",__LINE__,i,token[i]);
		#endif
	}
	if(token[i-1]==NULL){
		token_num=i-1;
	}
	else{
		token[i]=NULL;
		token_num=i;
	}
	if(strcmp(token[token_num-1],"&")==0)
		DaemonToggle=1;
	else 
		DaemonToggle=0;
	#ifndef NDEBUG
		printf("DEBUG--%d\t\ttokens:",__LINE__);
		for(i=0;token[i]!=NULL;i++)
			printf("%s ",token[i]);
		printf("\n");
		printf("DEBUG--%d\t\ttoken_num:%d\n",__LINE__,token_num);
	#endif
	//analyse tokens
	if(strcmp(token[0],"exit")==0){//exit/*{{{*/
		if(token[1]==NULL)
			exit(EXIT_SUCCESS);
		else
			printf("command error after \"exit\"\n");
	}/*}}}*/
	else if(strcmp(token[0],"cd")==0){//cd/*{{{*/
		if(token[1]==NULL)
			chdir(HOME);
		else if(token[2]==NULL){
			if(chdir(token[1])!=0){
				perror(NULL);
			}
		}
		else{
			printf("Usage:cd [path]\n");
			printf("If no path is given,shell will go to the HOME directory.\n");
		}
	}/*}}}*/
	else if(token[0][0]=='.'&&token[0][1]=='/'){//./command/*{{{*/
		pid_t pid;
		pid=fork();
		if(pid==0){
			char *cmd[MAX_TOKEN_NUM];
			for(i=0;i<token_num;i++){
				cmd[i]=token[i];
				#ifndef NDEBUG
					printf("DEBUG--%d\t\tcmd[%d]:%s\n",__LINE__,i,cmd[i]);
				#endif
			}
			cmd[i]=(char *)0;
			if(execv(cmd[0],cmd)==-1)
				perror(NULL);
			exit(EXIT_SUCCESS);
		}
		else if(pid>0){
			if(DaemonToggle==0)
				wait(NULL);
			#ifndef NDEBUG
				printf("DEBUG--%d\t\t./command end.\n",__LINE__);
			#endif
		}
		else{
			perror(NULL);
		}
	}/*}}}*/
	else{//error/*{{{*/
		printf("bad command!\n");
		return;
	}/*}}}*/
}/*}}}*/
int main ( int argc, char *argv[] )/*{{{*/
{
	printf("Starting ...\n");
	chdir(HOME);
	while(1)
		myshell();
	return EXIT_SUCCESS;
}/*}}}*/
