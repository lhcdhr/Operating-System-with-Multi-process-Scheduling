// Haochen Liu
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
int badcommandFileDoesNotExist();
int badcommandLoadingSameFile();
int set(char* var, char* value);
int print(char* var);
int run(char* script);
int exec(char* command_args[], int args_size);
int my_ls();
int echo();
// ensures unique pid
// after each pcb is constructed, 
// this count will increment by 1.
int pid_count = 0;

// struct of PCB
typedef struct pcb
{
	char pid[4];
	int length;
	int pc;
	int age;
	struct pcb *next;
}pcb_t;

pcb_t *head;
pcb_t *tail;

int interpreter(char* command_args[], int args_size){
	int i;

	if ( args_size < 1 || args_size > MAX_ARGS_SIZE){
		if (strcmp(command_args[0], "set")==0 && args_size > MAX_ARGS_SIZE) {
			return badcommandTooManyTokens();
		}
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
		char* value = (char*)calloc(1,150);
		char spaceChar = ' ';

		for(int i = 2; i < args_size; i++){
			strncat(value, command_args[i], 30);
			if(i < args_size-1){
				strncat(value, &spaceChar, 1);
			}
		}
		return set(command_args[1], value);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size > 2) return badcommand();
		return my_ls();
	
	}else if (strcmp(command_args[0], "echo")==0) {
		if (args_size > 2) return badcommand();
		return echo(command_args[1]);
	
	} 
	else if(strcmp(command_args[0], "exec")==0){
		// check argument length
		if(args_size <=2 || args_size >5) return badcommand();
		// check validity of scheduling policies
		if(strcmp(command_args[args_size-1], "FCFS") != 0 && 
			strcmp(command_args[args_size-1], "SJF") != 0 &&
			strcmp(command_args[args_size-1], "RR") != 0 &&
			strcmp(command_args[args_size-1], "AGING") != 0)
		{
			return badcommand();
		}
		return exec(command_args, args_size);
	}else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
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
	return 2;
}

int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}
int badcommandLoadingSameFile(){
	printf("%s\n", "Bad command: same file name");
	return 4;
}

int set(char* var, char* value){

	char *link = "=";
	char buffer[1000];
	strcpy(buffer, var);
	strcat(buffer, link);
	strcat(buffer, value);

	mem_set_value(var, value);

	return 0;

}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
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
	pcb_t *pcb_current = NULL;
	pcb_current = malloc(sizeof(pcb_t));
	// only one script, then it is both head and tail
	head = pcb_current;
	tail = pcb_current;
	char new_pid[4];
	sprintf(new_pid, "%d", pid_count);
	strcpy(pcb_current->pid, new_pid);

	int line_count = 0;
	// save into shellmemory
	while(1){	
		char count[4] = {};
		sprintf(count, "%d", line_count);
		char var[10] = {};
		strcat(var, new_pid);
		strcat(var, count);
		int length = strlen(line);
		if(line[length-1]!='\n'){
			line[length] = '\n';
			line[length+1] = '\0';
		}
		// shellmemory var format: pid + line number
		mem_set_value(var, line);
		memset(line, 0, sizeof(line));
		
		line_count++;
		if(feof(p)){
			break;
		}
		fgets(line,999,p);
		
	}
	pcb_current->length = line_count;
	pcb_current->pc = 0;
	pid_count++;
	//run the saved code
	for(int i=0;i<line_count; i++){
		char count[4] = {};
		sprintf(count, "%d", i);
		char var[10] = {};
		strcat(var, new_pid);
		strcat(var, count);
		char* command = mem_get_value(var);
		errCode = parseInput(command);// which calls interpreter()
		pcb_current->pc++;
	}
	// clean up, remove the script code from shell memory
	for(int i=0;i<line_count;i++){
		char count[4] = {};
		sprintf(count, "%d", i);
		char var[10] = {};
		strcat(var, new_pid);
		strcat(var, count);
		mem_clean(var);
	}
    fclose(p);
	// free the dynamically allocated memory
	free(pcb_current);
	
	head = NULL;
	tail = NULL;
	return errCode;
}

int exec(char* command_args[], int args_size){
	pcb_t *(pcbQueue[args_size-2]);
	// when only one scirpt is given, the call
	// run directly, because whatever scheduling
	// policy given, the result will be the same
	int errCode = 0;
	if(args_size == 3){
		return run(command_args[1]);
	}
	// dynamically allocate each pcb
	for(int i=0;i<args_size-2;i++){
		pcbQueue[i]=(pcb_t *)malloc(sizeof(pcb_t));
	}
	// check same file
	for(int i=0;i<args_size -3; i++){
		for(int j=i;j<args_size-3;j++){
			if(strcmp(command_args[i+1], command_args[j+2])==0){
				return badcommandLoadingSameFile();
			}
		}
	}
	// load script to memory
	for(int i=0; i<args_size-2; i++){
		char line[1000];
		FILE *p = fopen(command_args[i+1],"rt");  // the program is in a file
		if(p == NULL){
			return badcommandFileDoesNotExist();
		}
		fgets(line,999,p);
		char new_pid[4];
		sprintf(new_pid, "%d", pid_count);
		int line_count = 0;
		// save scripts into shellmemory
		while(1){	
			char count[4] = {};
			sprintf(count, "%d", line_count);
			char var[10] = {};
			strcat(var, new_pid);
			strcat(var, count);
			int length = strlen(line);
			if(line[length-1]!='\n'){
				line[length] = '\n';
				line[length+1] = '\0';
			}
			mem_set_value(var, line);
			memset(line, 0, sizeof(line));
			line_count++;
			if(feof(p)){
				break;
			}
			fgets(line,999,p);
		}
		
		strcpy(pcbQueue[i]->pid, new_pid);
		// set up the pcb
		pcbQueue[i]->length = line_count;
		pcbQueue[i]->pc = 0;
		pcbQueue[i]->age = line_count;
		pcbQueue[i]->next = NULL;
		pid_count++;
		fclose(p);
	}
	// FCFS scheduling policy
	if(strcmp(command_args[args_size-1],"FCFS")==0){
		head = pcbQueue[0];
		tail = pcbQueue[args_size-3];
		// set up the readyQueue
		for(int i=0;i<args_size-3;i++){
			pcbQueue[i]->next = pcbQueue[i+1];
		}
		pcb_t *current = head;
		// execute one by one (FCFS)
		for(int i=0;i<args_size-2;i++){
			int line_count=current->length;
			for(int i=0;i<line_count; i++){
				char current_pid[4] = {};
				sprintf(current_pid, "%s", current->pid);
				char count[4] = {};
				sprintf(count, "%d", i);
				char var[10] = {};
				strcat(var, current_pid);
				strcat(var, count);
				char* command = mem_get_value(var);
				errCode = parseInput(command);// which calls interpreter()
				current->pc++;
			}
			current = current->next;
		}
	}
	// SJF scheduling policy
	if(strcmp(command_args[args_size-1],"SJF")==0){
		// first, sort by job length (ascending lines of code)
		for(int i=0;i<args_size-3;i++){
			for(int j=0;j<args_size-3;j++){
				if(pcbQueue[j]->length>pcbQueue[j+1]->length){
					pcb_t *temp = pcbQueue[j];
					pcbQueue[j]=pcbQueue[j+1];
					pcbQueue[j+1]=temp;
				}
			}
		}
		// set up the readyQueue based on sorted order
		head = pcbQueue[0];
		tail = pcbQueue[args_size-3];
		for(int i=0;i<args_size-3;i++){
			pcbQueue[i]->next = pcbQueue[i+1];
		}
		// execute following the order
		pcb_t *current = head;
		for(int i=0;i<args_size-2;i++){
			int line_count=current->length;
			for(int i=0;i<line_count; i++){
				char current_pid[4] = {};
				sprintf(current_pid, "%s", current->pid);
				char count[4] = {};
				sprintf(count, "%d", i);
				char var[10] = {};
				strcat(var, current_pid);
				strcat(var, count);
				char* command = mem_get_value(var);
				errCode = parseInput(command);// which calls interpreter()
				current->pc++;
			}
			current = current->next;
		}
	}
	// RR scheduling policy
	if(strcmp(command_args[args_size-1],"RR")==0){
		int rr = 2;
		int completed=0;
		// set up the readyQueue
		head = pcbQueue[0];
		tail = pcbQueue[args_size-3];
		for(int i=0;i<args_size-3;i++){
			pcbQueue[i]->next = pcbQueue[i+1];
		}
		// execute
		while(1){
			// all process finished
			if(completed == args_size-2){
				break;
			}
			// remove the finished process from the readyQueue
			if(head->pc==head->length){
				rr=2;
				pcb_t *temp = head;
				head = head->next;
				temp->next=NULL;
				completed++;
				continue;
			}
			char current_pid[4] = {};
			sprintf(current_pid, "%s", head->pid);
			char count[4] = {};
			sprintf(count, "%d", head->pc);
			char var[10] = {};
			strcat(var, current_pid);
			strcat(var, count);
			char* command = mem_get_value(var);
			errCode = parseInput(command);
			head->pc++;
			rr--;
			// reset rr, and the head process shall
			// be moved to tail
			if(rr==0){
				rr=2;
				pcb_t *temp = head;
				head = head->next;
				tail->next = temp;
				tail = tail->next;
				tail->next = NULL;
			}
		}
	}
	// AGING scheduling policy
	if(strcmp(command_args[args_size-1],"AGING")==0){
		// first sort by age
		for(int i=0;i<args_size-3;i++){
			for(int j=0;j<args_size-3;j++){
				if(pcbQueue[j]->age > pcbQueue[j+1]->age){
					pcb_t *temp = pcbQueue[j];
					pcbQueue[j]=pcbQueue[j+1];
					pcbQueue[j+1]=temp;
				}
			}
		}
		// set up the readyQueue
		head = pcbQueue[0];
		tail = pcbQueue[args_size-3];
		for(int i=0;i<args_size-3;i++){
			pcbQueue[i]->next = pcbQueue[i+1];
		}
		int completed = 0;
		// execute
		while(1){
			// all process finished
			if(completed == args_size - 2){
				break;
			}
			// compare age, and the one with the smallest age
			// and relatively close to head should be the next one
			// when head's age is not the smallest
			// also, move head to the end of the readyQueue
			if(head->next != NULL && head->age > head->next->age){
				pcb_t *temp = head;
				head = head->next;
				temp->next = NULL;
				tail->next = temp;
				tail = tail->next;
			}			
			
			char current_pid[4] = {};
			sprintf(current_pid, "%s", head->pid);
			char count[4] = {};
			sprintf(count, "%d", head->pc);
			char var[10] = {};
			strcat(var, current_pid);
			strcat(var, count);
			char* command = mem_get_value(var);
			errCode = parseInput(command);
			head->pc++;
			pcb_t *p = head;
			// update age for non-head pcbs, and only those
			// with age greater than 0 shall be updated
			while(p->next!=NULL){
				if(p->next->age>0){
					p->next->age--;
				}
				p=p->next;
			}
			// the pcb finished executing shall be removed
			// from the readyQueue
			if(head->pc==head->length){
				completed++;
				head->age = 1001;
				pcb_t *temp = head;
				head = head->next;
				temp->next =NULL;
			}

		}
	}
	
	// clean mysh memory and free these dym-allocated structs
	for(int i=0;i<args_size-2; i++){
		
		int line_count = pcbQueue[i]->length;

		for(int j=0;j<line_count;j++){
			char count[4] = {};
			sprintf(count, "%d", j);
			char var[10] = {};
			strcat(var, pcbQueue[i]->pid);
			strcat(var, count);
			mem_clean(var);
		}
		
		free(pcbQueue[i]);
	}
	head=NULL;
	tail=NULL;
	return errCode;
}

int my_ls(){
	int errCode = system("ls | sort");
	return errCode;
}

int echo(char* var){
	if(var[0] == '$'){
		var++;
		printf("%s\n", mem_get_value(var)); 
	}else{
		printf("%s\n", var); 
	}
	return 0; 
}