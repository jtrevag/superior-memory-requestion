#include <iostream>
#include <stdlib.h> 
#include <stdio.h>
#include <memory>
#include "process.h"


using namespace std;

void easy_malloc(process* processes, int num);
void buddy_manager(memoryNode* root, process* processes, int num);
char* my_malloc(memoryNode* root, int size);
char* my_free(memoryNode* root, char* space);


int first = 1;

int main(){
	int num = 50;
	process processes[num];
	generateProcesses(processes, num);	
	printProcesses(processes,num);
	//easy_malloc(processes, num);
	memoryNode *root = new memoryNode((char*)malloc(10240000), 10240000, NULL, NULL, 0);
	buddy_manager(root, processes, num);
}

void easy_malloc(process* processes, int num){
	int i = 0, count = 0, removed = 0, j = 0;
	process running[num];
	while(removed < num){
		if(i % 50 == 0 && count<50){
			cout << "Adding process: " << count << endl;
			running[count] = processes[count];
			running[count].space = (char*) malloc(running[count].memory * 1000);
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

void buddy_manager(memoryNode* root, process* processes, int num){
	//NOTE: Your code is not being pushed to the server! Remember to do that. 
	int i = 0, count = 0, removed = 0, j = 0;
	process running[num];
	int totalSize = 0;
	while(removed < num){ //removed < num
		if(i % 50 == 0 && count<num){ //count < num
			cout << "Adding process: " << count << endl;
			running[count] = processes[count];
			running[count].space = my_malloc(root, running[count].memory * 1000);
			if(running[count].space == NULL){
				printf("Oh drat! %d did not get an address!", count);
			}
			if(count > 0){
				running[count].space+=8;	
			}
			printf("Made new node @ %p \n", running[count].space);
			totalSize += running[count].memory * 1000;
			count++;
		}
		for(j = 0; j<count; j++){
			if(running[j].cycles >= 0){
				running[j].cycles--;	
			}
			if(running[j].cycles == 0){
				cout << "Removing process: " << j << endl;
				my_free(root, running[j].space);
				removed++;
			}
		}
		i++;
	}
}

char* my_malloc (memoryNode* root, int size) {
	//printf("size: %d \n", size);
	//If there are child nodes and the current node is not occupied, visit them!
	if(root->left != NULL && root->right != NULL && root->occupied == 0){
			if(my_malloc(root->left, size) == NULL){
				my_malloc(root->right, size);	
			}
	} else{ //we are at a leaf node
		if(root->max_size >= size && root->max_size/2 < size && root->occupied == 0){ //if the node is good for insertion{
			printf("address of new node: %p with size: %d and element size: %d \n", root->start, root->max_size, size);
			root->occupied = 1;
			return root->start;
		} else if(!(root->max_size > size && root->max_size/2 < size) && root->max_size > 0 && root->occupied == 0 && root->left == NULL && root->right == NULL && root->max_size > size) { //Grow the tree
			root->left = new memoryNode(root->start, root->max_size/2, NULL, NULL, 0); 
			root->right = new memoryNode((root->start + (root->max_size)/2), root->max_size/2, NULL, NULL, 0);
			return my_malloc(root->left, size);
		} else{
			return NULL;
		}
	}
}

char* my_free(memoryNode* root, char* space) {
	if(root == NULL){
		return NULL;
	}
	printf("looking for node at address %p, currently at node %p \n",space, root->start);
	if(root->start == space && root->occupied == 1){
		cout << "Removing Node" << endl;
		printf("%p \n", space);
		root->start == NULL;
		root->occupied = 0;
		return space;
	} else{
		if(root->right != NULL){
			if(root->right->start > space){
				return my_free(root->right, space);
			} else{
				return my_free(root->left, space);
			}
		} else{
			return NULL;
		}
	}
} 
		

	
