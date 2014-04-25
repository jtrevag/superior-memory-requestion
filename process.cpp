#include <iostream>
#include <iomanip>     
#include <stdlib.h>     
#include <time.h>
#include "process.h"
using namespace std;

// Given a min value, max value, a mean and the number of elements, generate a pseudo normal set of data. 
int* genNormalData(int mean, int min, int max, int numElements){

		//if the mean is more than one standard deviation away from the middle of the range, shift the calculated mean by one paramater mean standard deviation so that the values generated are closer to the mean.
		int standardDiv = (max - min) / 6;
		int middle = (max + min) / 2;
		if(mean + standardDiv < middle){
			int meanStandardDiv = (mean - min)/3;
			mean -= meanStandardDiv;
		} else if(mean - standardDiv > middle){
			int meanStandardDiv = (max - mean)/3;
			mean += meanStandardDiv;
		}

		int* retArray = new int[numElements];
		srand (time(NULL));
		
		//generate random values, based on the standard deviation of the mean. 70% of the values will be within one standard deviation of the mean, while the rest will be 2-3 standard deviations away.
		for(int i = 0; i < numElements; i++){
			int valueToGen = rand() % 100 + 1;
			if(valueToGen <= 2){ //1-2, or 2%
				retArray[i]	= rand() % ((mean-min)/3) + min;
			} else if (valueToGen <= 16){ // 3-16, or 14%
				retArray[i]	= rand() % ((mean-min)/3) + (min + (mean-min)/3);
			} else if (valueToGen <= 50){ // 17-50, or 34%
				retArray[i]	= rand() % ((mean-min)/3) + (min + (2 * (mean-min)/3));
			} else if (valueToGen <= 84){ // 51-84, or 34%
				retArray[i]	= rand() % ((max-mean)/3) + mean;
			} else if (valueToGen <= 98){ // 85-98, or 14%
				retArray[i]	= rand() % ((max-mean)/3) + (mean + (max - mean)/3);
			} else{ // 1-2, or 2%
				retArray[i]	= rand() % ((max-mean)/3) + (mean + (2 * (max - mean)/3));
			}

		}

		return retArray;
		
}

//This function computes the average of the elements in the array.
int getAverage(int* array, int length){
	int average = 0;
	for(int i = 0; i < length; i++){
		average += array[i];
	}

	return average/length;
}


