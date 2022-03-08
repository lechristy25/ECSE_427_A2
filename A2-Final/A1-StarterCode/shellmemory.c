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
	int length;
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

void sortReadyQueue(struct pcb* head){
	//Node current will point to head  
	struct pcb *curr = head, *index = NULL;  
	struct pcb *temp = NULL;  
		
	if(head == NULL) {  
		return;  
	}  
	else {  
		while(curr != NULL) {  
			index = curr->next;  
			while(index != NULL) {  
				//If current node's data is greater than index's node data, swap the data between them  
				if(curr->length > index->length) {  
					temp = index->next;  
					curr->next = temp;  
					index->next = curr;  
				}  
				index = index->next;  
			}  
			curr = curr->next;  
		}      
	}   
}

// specific mem set for a script
void mem_set_script(char *var_in, char *value_in[], int lineCount) {
	int i;
	int j;
	//Value does not exist, need to find a free spot.
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			bool spaceAvailable = true; 
			for(j = i; j <= i + lineCount; j++){
				if(strcmp(shellmemory[j].var, "none") != 0){
					spaceAvailable = false; 
					break; 
				}
			}
			if(spaceAvailable){
				for(j = i; j <= i + lineCount; j++){
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
				newPcb->end = i + lineCount;
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

void sjf_exec(){
	//step 1: make length array for each pcb
	int ctr = 0;
	struct pcb* curr = head;
	int length[4];

	while(curr != NULL){
		length[ctr] = curr->end - curr->start;
		curr = curr->next;
		ctr++;
	}
	curr = head; //reinitialize curr to head after traversal

	//step 2: sort lengths in ascending order
	int temp;
	for (int i = 0; i < ctr+1; i++) {     
		for (int j = i+1; j < ctr+1; j++) {     
			if(length[i] > length[j]) {    
				temp = length[i];    
				length[i] = length[j];    
				length[j] = temp;    
			}     
		}     
	}    

	//step 3: traverse length array - for each length
		// 3.1: if duplicate ignore (?)
		// 3.2: get first pcb with that length
		// 3.3: execute it using start and end
		// 3.4: remove 

	for(int i = 0; i<ctr+1; i++){
		curr = head;
		while(curr != NULL){
			if(curr->end - curr->start == length[i]){
				int i; 
				int startingLine = curr->start; 
				int endingLine = curr->end; 
				for(i=startingLine; i <= endingLine; i++){
					//printf("key: %s, value: %s: index: %d\n", shellmemory[i].var, shellmemory[i].value, i);
					parseInput(shellmemory[i].value);
				}
				//remove from readyqueue
				for(i=startingLine; i <= endingLine; i++){
					shellmemory[i].var = "none"; 
					shellmemory[i].value = "none"; 
				}
				//make it indetectable by making length negative
				curr->end = 0;
				curr->start = 1;
			}
			curr = curr->next;
		}
	}

}

void rr_exec(){
	while(head != NULL){
		int i; 
		int currLine = head->programCounter; 
		int startingLine = head->start + currLine; 
		bool programComplete = false; 

		for(i = startingLine; i < startingLine + 2; i ++){
			if(i > head->end){
				programComplete = true; 
				break; 
			}
			parseInput(shellmemory[i].value); 
		}
		currLine = (i - (head->start)); 

		if(programComplete){
			startingLine = head->start; 
			int endingLine = head->end; 

			for(i=startingLine; i <= endingLine; i++){
				shellmemory[i].var = "none"; 
				shellmemory[i].value = "none"; 
			}

			struct pcb* tmp = head->next; 
			head = tmp;
		}else{
			head->programCounter = currLine; 
			struct pcb* tmp = head; 
			while(tmp->next != NULL){
				struct pcb* nextTmp = tmp->next; 
				tmp = nextTmp; 
			}
			struct pcb* nextHead = head->next;
			tmp->next = head;
			head = nextHead; 
			(tmp->next)->next = NULL; 
		}
	}
}

void aging_exec(){
	//0 - add length to pcb (end - start)
	//1 - order as sjf
	//2 - complete 1 iteration of a process w sjf (first line of first process)
	// IN LOOP #############################################
	//3 - iterate from head->next to end
		// substract 1 from length field from each pcb
	//4 - if there is an element w length STRICTLY smaller than head 
		// make it the head -> SWAP them
	//5 - remove it from memory 

	int ctr = 0;
	struct pcb* curr = head;
	int length[4];

	while(curr != NULL){
		int lgt = curr->end - curr->start;
		curr->length = lgt;
		length[ctr] = lgt;
		curr = curr->next;
		ctr++;
	}
	curr = head; //reinitialize curr to head after traversal

	//step 2: sort lengths in ascending order
	/*int temp;
	for (int i = 0; i < ctr+1; i++) {     
		for (int j = i+1; j < ctr+1; j++) {     
			if(length[i] > length[j]) {    
				temp = length[i];    
				length[i] = length[j];    
				length[j] = temp;    
			}     
		}     
	}    */

	sortReadyQueue(head);
	struct pcb* sortingptr = head;
	while(sortingptr != NULL){
		printf("%s\n", sortingptr->key);
		struct pcb* tmp = sortingptr->next;
		sortingptr = tmp;
	}
	//execute first command of first file
	/*while(curr != NULL){
		if(curr->length == length[0]){
			int startingLine = curr->start; 
			parseInput(shellmemory[startingLine].value);
			curr->start = curr->start + 1;
		}
	}*/
	while(head != NULL){

		int startingLine = head->start; 
		parseInput(shellmemory[startingLine].value);
		head->start = head->start + 1;

		//check if program is complete
		if(head->start > head->end){
			//remove from memory
			startingLine = head->start; 
			int endingLine = head->end; 
			int i;
			for(i=startingLine; i <= endingLine; i++){
				shellmemory[i].var = "none"; 
				shellmemory[i].value = "none"; 
			}

			struct pcb* tmp = head->next; 
			head = tmp;
			continue;
		}
	
		curr = head->next;
		while(curr->next != NULL){
			curr->length = curr->length - 1;
			struct pcb *tmp = curr->next;
			curr = tmp;
		}
		curr->length = curr->length - 1;

		//check if head is still smallest
		int headlgt = head->length;
		curr = head->next; //reset to second smallest
		struct pcb *min = head;
		while(curr != NULL){
			if(min->length > curr->length){
				min = curr;
			}
			curr = curr->next;
		}

		//swap min and head;
		if(min->pid != head->pid){
			struct pcb *headcpy = head;
			struct pcb *nextofmin = min->next;
			min->next = headcpy;
			head->next = nextofmin;
			head = min;
		}

		//remove from memory
	}

}
