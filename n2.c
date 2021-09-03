#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>


int server=0;
int client=0;


void error(int point){
	printf("\033[31mERROR:\033[0m %s (%i)\n",strerror(errno),point);
	exit(1);
}


void sigint(int signal){
	if(server>0)
		close(server);
	if(client>0)
		close(client);
	printf("\n\033[32mbye\n\033[0m");
	exit(0);
}


int change_signal(int signal,void (*func)(int)){
	struct sigaction action;
	action.sa_handler=func;
	sigemptyset(&action.sa_mask);
	action.sa_flags=0;
	return sigaction(signal,&action,NULL);
}


int main(void){
	if(change_signal(SIGINT,sigint)==-1)
		error(-1);
	char msg[1024]="done...\n";
	char buf[1024]={};
	server=socket(PF_INET,SOCK_STREAM,0);
	if(server==-1)
		error(0);
	struct sockaddr_in addr_server;
	addr_server.sin_family=PF_INET;
	addr_server.sin_port=(in_port_t)htons(4848);
	addr_server.sin_addr.s_addr=htonl(INADDR_ANY);
	int option=1;
	if(setsockopt(server,SOL_SOCKET,SO_REUSEADDR,(char*)&option,sizeof(int))==-1)
		error(1);
	if(bind(server,(struct sockaddr*)&addr_server,sizeof addr_server)==-1)
		error(2);
	if(listen(server,3)==-1)
		error(3);
	printf("\033[32mServer is working...\033[0m\n");
	while(1){
		struct sockaddr_storage addr_client;
		unsigned int size=sizeof addr_client;
		client=accept(server,(struct sockaddr*)&addr_client,&size);
		if(client==-1)
			printf("\033[31mERROR:\033[0m %s (%i)\n",strerror(errno),4);
		else{
			if(send(client,msg,strlen(msg),0)==-1)
				printf("\033[31mERROR:\033[0m %s (%i)\n",strerror(errno),5);
			close(client);
		}
	}
}
