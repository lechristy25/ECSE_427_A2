#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>

#include "shell.h"


struct memory_struct{
	char *var;
	char *value;
};

struct memory_struct shellmemory[1000];

struct pcb{
	int pid; 
	char *key; 
	int programCounter; 
	struct pcb* next; 
	int start; 
	int end; 
};

struct pcb* head = NULL; 
int pidCount = 0; 



// Helper functions
int match(char *model, char *var) {
	int i, len=strlen(var), matchCount=0;
	for(i=0;i<len;i++)
		if (*(model+i) == *(var+i)) matchCount++;
	if (matchCount == len)
		return 1;
	else
		return 0;
}

char *extract(char *model) {
	char token='=';    // look for this to find value
	char value[1000];  // stores the extract value
	int i,j, len=strlen(model);
	for(i=0;i<len && *(model+i)!=token;i++); // loop till we get there
	// extract the value
	for(i=i+1,j=0;i<len;i++,j++) value[j]=*(model+i);
	value[j]='\0';
	return strdup(value);
}



// Shell memory functions

void mem_init(){

	int i;
	for (i=0; i<1000; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
	// head = (struct pcb*)malloc(sizeof(struct pcb));

}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {
	
	int i;

	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	return;

}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;

	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){

			return strdup(shellmemory[i].value);
		} 
	}
	return "Variable does not exist";

}

// specific mem set for a script
void mem_set_script(char *var_in, char *value_in[], int lineCount) {
	printf("setting script");
	int i;
	int j;
	//Value does not exist, need to find a free spot.
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			bool spaceAvailable = true; 
			for(j = i; j < i + lineCount; j++){
				if(strcmp(shellmemory[j].var, "none") != 0){
					spaceAvailable = false; 
					break; 
				}
			}
			if(spaceAvailable){
				for(j = i; j < i + lineCount; j++){
					shellmemory[j].var = strdup(var_in); 
					shellmemory[j].value = strdup(value_in[j-i]);
				}
				pidCount++; 
				struct pcb *newPcb = (struct pcb*)malloc(sizeof(struct pcb));
				newPcb->key = var_in;  
				newPcb->pid = pidCount; 
				newPcb->programCounter = 0; 
				newPcb->next = NULL; 
				newPcb->start = i; 
				newPcb->end = i + lineCount - 1;
				if(head == NULL){
					head = newPcb; 
				}else{
					struct pcb* tmp = head; 
					while(tmp->next != NULL){
						struct pcb* nextTmp = tmp->next; 
						tmp = nextTmp; 
					}
					tmp->next = newPcb;
				}
				break; 
			}
		} 
	}

	// get the string back and read it until a line break 
	char *currProcess = mem_get_value(head->key); 

}

void fcfs_run(){
	while(head != NULL){
		int i; 
		int startingLine = head->start; 
		int endingLine = head->end; 
		for(i=startingLine; i <= endingLine; i++){
			printf("key: %s, value: %s: index: %d\n", shellmemory[i].var, shellmemory[i].value, i);
		}

		for(i=startingLine; i <= endingLine; i++){
			shellmemory[i].var = "none"; 
			shellmemory[i].value = "none"; 
		}

		struct pcb* tmp = head->next; 
		head = tmp; 
	}
	printf("head is null rn");
}

void fcfs_exec(){
	printf("in exec");
	while(head != NULL){
		int i; 
		int startingLine = head->start; 
		int endingLine = head->end; 
		for(i=startingLine; i <= endingLine; i++){
			//printf("key: %s, value: %s: index: %d\n", shellmemory[i].var, shellmemory[i].value, i);
			parseInput(shellmemory[i].value);
		}

		for(i=startingLine; i <= endingLine; i++){
			shellmemory[i].var = "none"; 
			shellmemory[i].value = "none"; 
		}

		struct pcb* tmp = head->next; 
		head = tmp; 
	}
	printf("head is null rn");
}