/*
@file
Algoritma Analizi 1. Ödev
N elemanlý bir dizide birbirine en yakýn deðere sahip 
iki elemanýn bulunmasý Brute Force(n**2) ile bulunmuþtur.
@author
Name: Emre Çelik
Student No: 15011902
Date: 06/11/2020
E-mail: emrecelikk97@gmail.com
Complier Used: TDM-GCC 4.9.2
IDE: Dev-C++ 5.11
Operating System: Windows 10
CPU: 2.6 GHz Intel Core i7
RAM: 16 GB
Video Card: GTX960M 4 GB
*/
#include <stdio.h>
#include <stdlib.h>

#define ARRAYSIZE 10	//Change Array Size

void printArray(int*, int);
void randArray(int*, int);
void findMinDiff(int*, int);

int main(){
	int array[ARRAYSIZE];
	int arrayLenght = sizeof(array) / sizeof(array[0]);

	randArray(array, arrayLenght);
	printArray(array, arrayLenght);
	findMinDiff(array, arrayLenght);
	return 0;
}

// printing array
void printArray(int* array, int arrayLenght){
	int i;
	printf("Input  : {");
	for(i = 0; i < arrayLenght; i ++){
		printf("%d ", array[i]);
	}
	printf("}\n");
}

// filling array with random value
void randArray(int* array, int arrayLenght){
	int i;
	for (i = 0; i < arrayLenght; i++ ){
		array[i] = rand() % 100;
	}
}

/*
finding min difference in array
@min: min diff temp value
@x: minimum number 1
@y: minimum number 2
*/
void findMinDiff(int* array, int arrayLength){
	int i; 
	int j; 
	int min = INT_MAX; 
	int x; 
	int y;
	for (i = 0; i < arrayLength - 1; i++){
		for (j = 0; j < arrayLength - i - 1; j++){
			if(i != j && abs(array[i] - array[j]) < min) {
				x = array[i];
				y = array[j];
				min = abs(array[i] - array[j]);
			}
		}
	}
	printf("Output : %d\n", min);
	printf("Minimum difference is between %d and %d", x, y);
}

