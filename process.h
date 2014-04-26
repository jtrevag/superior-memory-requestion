#ifndef PROCESS_H
#define PROCESS_H

struct process{
		int id;
		int cycles;
		int memory;
		char* space;
}; 

struct memoryNode{
	void* start;
	int max_size;
	memoryNode* left;
	memoryNode* right;
};

int getAverage(int*, int);
int* genNormalData(int, int, int, int);
void generateProcesses(process*, int);
void printProcesses(process*, int);

#endif