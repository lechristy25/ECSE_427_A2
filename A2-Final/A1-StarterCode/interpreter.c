#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"

int MAX_ARGS_SIZE = 7;

int help();
int quit();
int badcommand();
int badcommandTooManyTokens(); 
int set(char* command_args[], int args_size);
int print(char* var);
int run(char* script);
int badcommandFileDoesNotExist();
int my_ls();
int echo(char* var); 

// Interpret commands and their arguments
int interpreter(char* command_args[], int args_size){
	int i;

	if ( args_size < 1){
		return badcommand();
	}


	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size < 3) return badcommand();
		if (args_size > MAX_ARGS_SIZE) return badcommandTooManyTokens();
		return set(command_args, args_size);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "echo")==0) {
		if (args_size != 2) return badcommand();
		return echo(command_args[1]);

	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size != 1) return badcommand();
		return my_ls();

	}else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
echo VAR		Displays the STRING assigned to VAR, if its preceeded by a \"$\", or simply displays VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

int badcommandTooManyTokens(){
	printf("%s\n", "Bad command: Too many tokens"); 
	return 1; 
}

// For run command only
int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int set(char* command_args[], int args_size){
	char *link = "=";
	char buffer[1000]; 
	char *var = command_args[1];
	char values[1000];

	strcpy(buffer, var);
	strcat(buffer, link);

	for(int i = 2; i < args_size; i++){
		strcat(values, command_args[i]);
		strcat(values, " "); 
	}

	strcat(buffer, values); 

	mem_set_value(var, values); 

	values[0] = 0;

	return 0;

}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int echo(char* var){ 
	if(strncmp(var, "$", 1) == 0){
		char searchVar[100];  
		for(int i = 1; i < strlen(var); i++){
			searchVar[i - 1] = var[i]; 
		}
		print(searchVar);
	}
	else{
		printf("%s\n", var); 
	}

	return 0;
}

int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	fgets(line,999,p);
	while(1){
		errCode = parseInput(line);	// which calls interpreter()
		memset(line, 0, sizeof(line));

		if(feof(p)){
			break;
		}
		fgets(line,999,p);
	}

    fclose(p);

	return errCode;
}



int my_ls(){
	system("ls | tr ' ' '\n'");
	return 0;
}
