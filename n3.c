#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>


int server;


void error(int point){
	printf("\033[31mERROR:\033[0m %s (%i)\n",strerror(errno),point);
	exit(1);
}


void sigint(int sig){
	close(server);
	printf("\n\033[33mbye\033[0m\n");
	exit(0);
}


int change_signal(int signal,void(*func)(int)){
	struct sigaction action;
	action.sa_handler=func;
	sigemptyset(&action.sa_mask);
	action.sa_flags=0;
	return sigaction(signal,&action,NULL);
}


int main(void){
	if(change_signal(SIGINT,sigint)==-1)
		error(-1);
	char *msg="\033[33mwrite something-\033[0m\n";
	char buffer[256]={};
	server=socket(PF_INET,SOCK_STREAM,0);
	if(server==-1)
		error(0);
	struct sockaddr_in addr;
	addr.sin_family=PF_INET;
	addr.sin_port=(in_port_t)htons(4848);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(server,(struct sockaddr*)&addr,sizeof addr)==-1)
		error(1);
	if(listen(server,3)==-1)
		error(2);
	printf("\033[33;1mServer is working...\033[0m\n");
	while(1){
		struct sockaddr_storage addr_;
		unsigned int size=sizeof addr_;
		int client=accept(server,(struct sockaddr*)&addr_,&size);
		if(client==-1){
			printf("\033[31mThe client couldn't connect \033[0m\n");
		}
		else{
			if(send(client,msg,strlen(msg),0)==-1)
				printf("\033[36mERROR: error sending data\033[0m\n");
			while(1){
				int was_reading=recv(client,buffer,sizeof buffer,0);
				if(was_reading==-1){
					printf("\033[36mERROR: error reading data\033[0m\n");
					break;
				}
				else if(was_reading>0){
					buffer[was_reading]='\0';
					printf("%s",buffer);
					for(int i=0;i<was_reading;i++){
						buffer[i]='\0';
					}
				}
				else
					break;
			}
			close(client);
		}
	}
}
