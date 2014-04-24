#include <iostream>
#include <stdlib.h> 
#include "process.h"

using namespace std;

void easy_malloc(process* processes, int num);
void buddy_manager(process* processes, int num);
void* my_malloc(memoryNode*, int size);
void my_free(void *space);

int main(){
	int num = 50;
	process processes[num];
	generateProcesses(processes, num);	
	printProcesses(processes,num);
	easy_malloc(processes, num);
}

void easy_malloc(process* processes, int num){
	int i = 0, count = 0, removed = 0, j = 0;
	process running[num];
	while(removed < num){
		if(i % 50 == 0 && count<50){
			cout << "Adding process: " << count << endl;
			running[count] = processes[count];
			running[count].space= malloc(running[count].memory * 1000);
			count++;
		}

		for(j = 0; j<count; j++){
			if(running[j].cycles >= 0){
				running[j].cycles--;	
			}
			if(running[j].cycles == 0){
				cout << "Removing process: " << j << endl;
				free(running[j].space);
				removed++;
			}
		}
		i++;
	}
}

void buddy_manager(process* processes, int num){
	
	memoryNode *root;
	root->start = malloc(10000000);

}

void* my_malloc (memoryNode *root, int size) {
	
}

void my_free (void* space) {
	
}