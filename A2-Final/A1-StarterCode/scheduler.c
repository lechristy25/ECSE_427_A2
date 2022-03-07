#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"
#include "interpreter.h"
int badpolicy();

int badpolicy(){
	printf("%s\n", "Unknown Policy");
	return 5;
}
int scheduler(char* policy, ){
    if(strcmp(policy, "SJF") == 0){
        //step 1: make length array for each pcb
        int ctr = 0;
        pcb *curr = head;
        int length[4];

        while(curr != NULL){
            length[ctr] = curr->end - curr->start;
            curr = curr->next;
            ctr++;
        }

        //step 2: sort lengths in ascending order
        int temp;
        for (int i = 0; i < length; i++) {     
            for (int j = i+1; j < length; j++) {     
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
    }else if (strcmp(policy, "RR") == 0){

    }else{
        badpolicy();
    }
}

int 