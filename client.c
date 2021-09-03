#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>


int sock=0;


void error(int point){
	printf("\033[31mERROR: %s (%i)\n\033[0m",strerror(errno),point);
	if(sock!=0)
		close(sock);
	exit(1);
}


int main(void){
	char buf[1024]={};
	char msg[]="GET /\r\n\r\n";
	int len;
	struct addrinfo* list;
	struct addrinfo example;
	example.ai_family=PF_UNSPEC;
	example.ai_socktype=SOCK_STREAM;
	if(getaddrinfo("www.google.com","80",&example,&list)==-1)
		error(0);
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1)
		error(1);
	if(connect(sock,list->ai_addr,list->ai_addrlen)==-1)
		error(2);
	if(send(sock,msg,strlen(msg),0)==-1)
		error(3);
	while(len=recv(sock,buf,sizeof buf,0)){
		buf[len]='\0';
		if(len==-1)
			error(4);
		printf("%s",buf);
	}
	close(sock);
}
