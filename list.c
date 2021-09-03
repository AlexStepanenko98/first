#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct array{
	char* data;
	struct array* next;
}list;


list* create_l(char*string){
	list* new=(list*)malloc(sizeof(list));
	new->data=strdup(string);
	new->next=NULL;
	return new;
}


void delete_l(list* start){
	list* next;
	while(start!=NULL){
		next=start->next;
		free(start->data);
		free(start);
		start=next;
	}
}


void show_l(list* start){
	while(start!=NULL){
		printf("%s",start->data);
		start=start->next;
	}
}


int main(void){
	list* start=NULL;
	list* next=NULL;
	list* i=NULL;
	char buffer[1024]={};
	while(fgets(buffer,sizeof buffer,stdin)){
		next=create_l(buffer);
		if(start==NULL)
			start=next;
		if(i!=NULL)
			i->next=next;
		i=next;
	}
	show_l(start);
	delete_l(start);
	return 0;
}
