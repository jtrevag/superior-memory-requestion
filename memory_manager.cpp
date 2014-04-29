#include <iostream>
#include <stdlib.h> 
#include <stdio.h>
#include <time.h>
#include <queue>
#include "process.h"


using namespace std;

void basic_malloc(process* processes, int num);
void buddy_manager(memoryNode* root, process* processes, int num);
void basic_queue(process* processes, int num, int maxMem);
void buddy_queue(memoryNode* root, process* processes, int num, int maxMem);
char* my_malloc(memoryNode* root, int size);
char* my_free(memoryNode* root, void* space); 


bool operator>( const process& lhs, const process& rhs ) {
  return lhs.memory < rhs.memory;
}

bool operator<( const process& lhs, const process& rhs ) {
  return lhs.memory > rhs.memory;
}

int main(){
	clock_t t;
	int num = 50;
	process processes[num];
	generateProcesses(processes, num);	
	int totalMem = 10240000;
	memoryNode *root = new memoryNode((char*)malloc(totalMem), totalMem, NULL, NULL, 0);
	
	t = clock();
	basic_malloc(processes, num);
	t = clock() - t;
	cout << "noqueue_malloc time to completion: " << (float) t / CLOCKS_PER_SEC << " seconds." <<  endl;
	
	t = clock();
	buddy_manager(root, processes, num);
	t = clock() - t;
	cout << "noqueue_buddy_manager time to completion: " << (float) t / CLOCKS_PER_SEC << " seconds." <<  endl;
 	
		t = clock();
	basic_queue(processes, num, totalMem/1000);
	t = clock() - t;
	cout << "easy_queue time to completion: " << (float) t / CLOCKS_PER_SEC << " seconds." <<  endl;
	

	t = clock();
	buddy_queue(root, processes, num, totalMem/1000);
	t = clock() - t;
	cout << "buddy_queue time to completion: " << (float) t / CLOCKS_PER_SEC << " seconds." <<  endl;
	
	free(root->start);
}

//easy_malloc takes in the set of processes and how many processes it is to execute, and mallocs space for them as they enter. When a process is completed, it is freed. 
//Assumption: There is enough memory for all 50 processes to execute concurrently using this function.
void basic_malloc(process* processes, int num){
	int i = 0, count = 0, removed = 0, j = 0;
	process running[num];
	while(removed < num){ //Loops until all processes have completed and left the system. 
		if(i % 50 == 0 && count<50){ //Adds a process to the running array. 
			running[count] = processes[count];
			running[count].space = (char*) malloc(running[count].memory * 1000);
			count++;
		}

		for(j = 0; j<count; j++){ //Decrement cycles for running processes
			if(running[j].cycles >= 0){
				running[j].cycles--;	
			}
			if(running[j].cycles == 0){ //Removes completed processes. 
				free(running[j].space);
				removed++;
			}
		}
		i++;
	}
}

//buddy_manager takes in the set of processes and how many processes it is to execute, and my_mallocs space for them as they enter. When a process is completed, it is my_freed. 
//Assumption: There is enough memory for all 50 processes to execute concurrently using this function.
void buddy_manager(memoryNode* root, process* processes, int num){
	int i = 0, count = 0, removed = 0, j = 0;
	process running[num];
	int totalSize = 0;
	while(removed < num){
		if(i % 50 == 0 && count<num){
			running[count] = processes[count];
			running[count].space = my_malloc(root, running[count].memory * 1000);
			totalSize += running[count].memory * 1000;
			count++;
		}
		for(j = 0; j<count; j++){
			if(running[j].cycles >= 0){
				running[j].cycles--;	
			}
			if(running[j].cycles == 0){
				my_free(root, running[j].space);
				removed++;
			}
		}
		i++;
	}
}

//basic_queue follows the basic structure of the other functions, with the exception that there is a queue added.
//If the process would cause curMem to exceed maxMem, it is put on procQueue instead of the running[] array. 
//After process cycles have been decremented and completed processes removed, we check to see if a process can be dequeued, and add it to running[] if we can. 
void basic_queue (process* processes, int num, int maxMem) {
	int i = 0, count = 0, runningCount = 0, removed = 0, j = 0, curMem = 0;
	process running[num];
	queue<process> procQueue;
	
	while(removed < num){
		if(i % 50 == 0 && count< num){
			if (curMem + processes[count].memory > maxMem) { //Queue a process if there is no room. 
				procQueue.push(processes[count]);
			}
			else {	
				running[runningCount] = processes[count];
				running[runningCount].space = (char*) malloc(running[runningCount].memory * 1000);
				curMem += running[runningCount].memory;
				runningCount++;
			}
			count++;
		}

		for(j = 0; j < runningCount; j++){ //Decrement cycles for each running process
			if(running[j].cycles >= 0){
				running[j].cycles--;	
			}
			if(running[j].cycles == 0){ //Remove process from running[] if it is finished. 
				free(running[j].space);
				curMem -= running[j].memory;
				removed++;
			}
		}
		
		if (!procQueue.empty() && maxMem - curMem >= procQueue.front().memory) { //Check to see if we can dequeue and add to running[]
			running[runningCount] = procQueue.front();
			procQueue.pop();
			running[runningCount].space = (char*) malloc(running[runningCount].memory * 1000);
			curMem += running[runningCount].memory;
			runningCount++;
		}
		i++;
	}
}

//Similar to basic_queue, but adds a process to a queue if the my_malloc call fails
//The my_malloc call will fail if a level in the binary tree runs out of nodes.  		
void buddy_queue (memoryNode* root, process* processes, int num, int maxMem) {
	int i = 0, count = 0, runningCount = 0, removed = 0, j = 0, curMem = 0, checkQueue = 0;
	process running[num];
	priority_queue<process> procQueue;
	
	while(removed < num){
		if(i % 50 == 0 && count< num){
			if (curMem + processes[count].memory > maxMem) {
				procQueue.push(processes[count]);
				checkQueue++;
			}
			else {
				running[runningCount] = processes[count];
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
				my_free(root, running[j].space);
				curMem -= running[j].memory;
				removed++;
			}
		}

		if (!procQueue.empty()) {
			if(checkQueue>0){ //something has been freed, so we have space on the tree somewhere
				if((maxMem - curMem) >= procQueue.top().memory){
					running[runningCount] = procQueue.top();
					running[runningCount].space = (char*) my_malloc(root, running[runningCount].memory * 1000);
					if(running[runningCount].space != NULL){ //Only dequeues the process if the my_malloc call is successful. 
						procQueue.pop();
						curMem += running[runningCount].memory;
						runningCount++;
						checkQueue--;	
					} 
				}
			}
		}
		i++;
	}
}

char* my_malloc (memoryNode* root, int size) {
	//If there are child nodes and the current node is not occupied, visit them
	if(root->left != NULL && root->right != NULL && root->occupied == 0){
			if(my_malloc(root->left, size) == NULL){
				my_malloc(root->right, size);	
			}
	} else{ //we are at a leaf node
		if(root->max_size >= size && root->max_size/2 < size && root->occupied == 0){ //if the node is the "perfect" size
			root->occupied = 1;
			return root->start;
		} else if(!(root->max_size > size && root->max_size/2 < size) && root->max_size > 0 && root->occupied == 0 && root->left == NULL && root->right == NULL && root->max_size > size) { //Grow the tree
			root->left = new memoryNode(root->start, root->max_size/2, NULL, NULL, 0); 
			root->right = new memoryNode(&((root->start)[root->max_size/2]), root->max_size/2, NULL, NULL, 0);
			return my_malloc(root->left, size);
		} else{ //No node was found to insert into; continue from the left node. 
			return NULL;
		}
	}
}

char* my_free(memoryNode* root, void* space) {
	//If root is null, node was not found
	if(root == NULL){
		return NULL;
	}
	//If the root node is being removed, check for that first.
	if(root->occupied == 1 && root->start == space){
		root->occupied = 0;
		root->left = NULL;
		root->right = NULL;
	}
	//
	if(root->right != NULL){
		if(space >= root->right->start){
			if(root->right->occupied == 1 && root->right->start == space){
				root->right->occupied = 0;
				if(root->left->occupied == 0){
					root->right = NULL;
					root->left = NULL;
				}
				return (char*)space;
			} else{
				return my_free(root->right, space);
			}
		} else {
			if(root->left->occupied == 1 && root->left->start == space){
				root->left->occupied = 0;
				if(root->right->occupied == 0){
					root->right = NULL;
					root->left = NULL;
				}
				return (char*)space;
			} else{
				return my_free(root->left, space);
			}
		}
	}
} 
