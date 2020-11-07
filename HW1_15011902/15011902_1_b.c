/*
@file
Algoritma Analizi 1. Ödev
N elemanlý bir dizide birbirine en yakýn deðere sahip 
iki elemanýn bulunmasý nlogn ile bulunmuþtur.
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
void merge(int*, int, int, int);
void mergeSort(int*, int, int);

int main(){
	int array[ARRAYSIZE];	
	int arrayLenght = sizeof(array) / sizeof(array[0]);

	randArray(array, arrayLenght);
	//printArray(array, arrayLenght);
	mergeSort(array, 0, arrayLenght - 1);
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
	int min = INT_MAX; 
	int x; 
	int y;
	for (i = 0; i < arrayLength - 1; i++) {
		if(abs(array[i + 1] - array[i]) < min) {
			x = array[i + 1];
			y = array[i];
			min = abs(array[i + 1] - array[i]);
		}
	}
	printf("Output : %d\n", min);
	printf("Minimum difference is between %d and %d", x, y);
}

void merge(int* array, int l, int m, int r){
    int i; 
	int j; 
	int k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    int L[n1], R[n2];
 
    for (i = 0; i < n1; i++)
        L[i] = array[l + i];
    for (j = 0; j < n2; j++)
        R[j] = array[m + 1+ j];
 
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2){
        if (L[i] <= R[j]){
            array[k] = L[i];
            i++;
        }
        else{
            array[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1){
        array[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2){
        array[k] = R[j];
        j++;
        k++;
    }
}


void mergeSort(int* array, int l, int r){
    if (l < r){
        int m = l+(r-l)/2;
        mergeSort(array, l, m);
        mergeSort(array, m+1, r);
        merge(array, l, m, r);
    }
}

