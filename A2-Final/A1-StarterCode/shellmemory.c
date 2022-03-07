#include<stdlib.h>
#include<string.h>
#include<stdio.h>


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
void mem_set_script(char *var_in, char *value_in) {
	
	int i;
	//Value does not exist, need to find a free spot.
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);

			pidCount++; // to get new pid each time
			struct pcb *newPcb = (struct pcb*)malloc(sizeof(struct pcb)); // allocating space for struct
			newPcb->pid = pidCount; // setting pcb values
			printf("%d\n", (*newPcb).pid);
			newPcb->key = var_in; 
			printf("%s\n", (*newPcb).key);
			newPcb->programCounter = 0; 
			newPcb->next = NULL;

			// adding pcb to linkedlist
			if(head == NULL){
				head = newPcb; 
			}else{
				head->next = newPcb; 
			}
			break;
		} 
	}

	// get the string back and read it until a line break 
	char *currProcess = mem_get_value(head->key); 
	printf("%s", mem_get_value((*head).key));
	char *p, *temp;
	p = strtok_r(currProcess, "\n", &temp);

	// reading the string that was stored line by line
	do {
		if(strcmp(p, "\0") == 0){
			break;
		}
		printf("current line = %s", p);
	} while ((p = strtok_r(NULL, "\n", &temp)) != NULL);

	// for some reason this part causes the error, it's trying to take the head process that was just "executed" and removes it from memory
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, ((*head).key)) == 0){
			shellmemory[i].var = "none"; 
			shellmemory[i].value = "none";
			break;
		} 
	}
	// head = head->next;
	//free memory
	

}