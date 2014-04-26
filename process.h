#ifndef PROCESS_H
#define PROCESS_H

struct process{
		int id;
		int cycles;
		int memory;
		char* space;
}; 

struct memoryNode{
	char* start;
	int max_size;
	memoryNode* left;
	memoryNode* right;
	int occupied;

	memoryNode()
	{
		start = NULL;
        max_size = 0;
        left = NULL;
        right = NULL;
        occupied = 0;
	}

    memoryNode(char* init_start, int init_max_size, memoryNode* init_left, memoryNode* init_right, int init_occupied)
    {
        start = init_start;
        max_size = init_max_size;
        left = init_left;
        right = init_right;
        occupied = init_occupied;
    }
};

int getAverage(int*, int);
int* genNormalData(int, int, int, int);
void generateProcesses(process*, int);
void printProcesses(process*, int);

#endif