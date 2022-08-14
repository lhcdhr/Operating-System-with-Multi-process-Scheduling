// Haochen Liu
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct memory_struct{
	char *var;
	char *value;
};

struct memory_struct shellmemory[1000];

// Shell memory functions

void mem_init(){

	int i;
	for (i=0; i<1000; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
}

// Set key value pair
int mem_set_value(char *var_in, char *value_in) {

	int i;
	int out = 0;
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return 0;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);

			return out;
		}
		out++; 
	}

	return 1;

}
// reset the designated spot in memory to none 
void mem_clean(char *var_in){
	int i;
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].var = "none";
			memset(shellmemory[i].value, 0, sizeof(shellmemory[i].value));
			shellmemory[i].value = "none";
		} 
	}
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
// to count the numebr of empty spots in shell memory
int empty_spot(){
	int count = 0;
	for(int i=0;i<1000;i++){
		if(strcmp(shellmemory[i].var,"none")==0 
			&& strcmp(shellmemory[i].value,"none")==0){
			count++;
		}
	}
	return count;
}