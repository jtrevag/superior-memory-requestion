#include <iostream>
#include <stdlib.h> 
#include <queue>
#include "process.h"

using namespace std;

void easy_malloc(process* processes, int num);
void easy_queue(process* processes, int num);
void buddy_manager(process* processes, int num);
void* my_malloc(memoryNode*, int size);
void my_free(void *space);

int main(){
	int num = 50;
	process processes[num];
	generateProcesses(processes, num);	
	printProcesses(processes,num);
	easy_queue(processes, num);
}

void easy_malloc(process* processes, int num){
	int i = 0, count = 0, removed = 0, j = 0;
	process running[num];
	while(removed < num){
		if(i % 50 == 0 && count<50){
			cout << "Adding process: " << count << endl;
			running[count] = processes[count];
			running[count].space= (char*) malloc(running[count].memory * 1000);
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

void easy_queue (process* processes, int num) {
	int i = 0, count = 0, runningCount = 0, removed = 0, j = 0, maxMem = 5000, curMem = 0;
	process running[num];
	queue<process> procQueue;
	
	while(removed < num){
		if(i % 50 == 0 && count<50){
			if (curMem + processes[count].memory > maxMem) {
				cout << "Queueing process: " << count << endl;
				procQueue.push(processes[count]);
			}
			else {	
				cout << "Adding process: " << count << endl;
				running[runningCount] = processes[count];
				running[runningCount].space= (char*) malloc(running[runningCount].memory * 1000);
				curMem += running[runningCount].memory;
				runningCount++;
			}
			count++;
		}

		for(j = 0; j < runningCount; j++){
			if(running[j].cycles >= 0){
				running[j].cycles--;	
			}
			if(running[j].cycles == 0){
				cout << "Removing process: " << j << endl;
				free(running[j].space);
				curMem -= running[j].memory;
				removed++;
			}
		}
		
		if (!procQueue.empty() && maxMem - curMem >= procQueue.front().memory) {
			running[runningCount] = procQueue.front();
			procQueue.pop();
			cout << "Dequeueing and adding process." << endl;
			running[runningCount].space = (char*) malloc(running[runningCount].memory * 1000);
			curMem += running[runningCount].memory;
			runningCount++;
		}
		i++;
	}
}

void buddy_manager(process* processes, int num){
	
	memoryNode *root;
	root->start = (char*) malloc(10000000);

}

void* my_malloc (memoryNode *root, int size) {
	
}

void my_free (void* space) {
	
}