#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>


void error(int point){
	printf("\033[31mERROR: %s (%i)\033[0m\n",strerror(errno),point);
	exit(1);
}


int main(void){
	char *msg="\n\033[32mhello alex\033[0m\n";
	int server=socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family=PF_INET;
	addr.sin_port=(in_port_t)htons(4848);
	addr.sin_addr.s_addr=htonl(INADDR_ANY);
	bind(server,(struct sockaddr*)&addr,sizeof addr);
	listen(server,2);
	printf("\033[33mServer is working ...\033[0m\n");
	while(1){
		struct sockaddr_storage client_addr;
		unsigned int size=sizeof client_addr;
		int client=accept(server,(struct sockaddr*)&client_addr,&size);
		send(client,msg,strlen(msg),0);
		close(client);
	}
}
