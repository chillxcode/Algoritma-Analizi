/*
@file
Algoritma Analizi 1. Ödev
Von Neumann’s Neighborhood” kuralýný gözeterek verilen N deðerine 
göre matrisleri oluþturan program yazýmý.
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

#define N 1		//Von Neumann’s Neighborhood parameter

int **mallocMatrix(int**, int);
void printMatrix(int**, int);
void reallocMatrix(int**, int);
void fillDefaultValues(int**, int);
void vonNeumannNeighborhood(int**, int);

/*
@size: matrix size
@matrix: matrix
*/
int main(){
	int size = 2*N + 3;
	int **matrix;
	matrix = mallocMatrix(matrix, size);
	fillDefaultValues(matrix, size);
	vonNeumannNeighborhood(matrix, size);
	printMatrix(matrix, size);
	reallocMatrix(matrix, size);
	return 0;
}

// creating matrix
int** mallocMatrix(int** matrix, int size){
    matrix = (int**)malloc(size * sizeof(int*));
    int row;
    for (row = 0; row < size; row++){
        matrix[row] = (int *)malloc(size * sizeof(int));
    }
    return matrix;
}

/*
printing matrix with von neumann
@rowCount: total row 1 cell count
@totalCount: total 1 cell count
*/
void printMatrix(int** matrix, int size){
	int i; 
	int j; 
	int rowCount = 0; 
	int totalCount = 0;
	printf("\n");
    for (i = 0; i < size; i++){
    	printf(" ");
        for (j = 0; j < size; j++){
            printf("%d  ", matrix[i][j]);
            if(matrix[i][j] == 1) {
            	totalCount++;
            	rowCount++;
			}
        }
        printf(" :%d", rowCount);
        rowCount = 0;
        printf("\n\n");
    }
    printf("\nTotal : %d", totalCount);
}

// free matrix
void reallocMatrix(int** matrix, int size){
	int i;
	for (i = 0; i < size; i++){
        free(matrix[i]);
    }
    free(matrix);
}

// filling matrix with 0
void fillDefaultValues(int** matrix, int size){
	int i;
	int j;
	for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
            matrix[i][j] = 0;
        }
    }
}

// finding neighborhood to origin. If, change value to 1
void vonNeumannNeighborhood(int** matrix, int size) {
	int i; 
	int j;
	int middle = (size - 1) / 2;
	for (i = 0; i < size; i++){
        for (j = 0; j < size; j++){
        	int x = abs(middle - i);
        	int y = abs(middle - j);
            if(x + y <= N){
            	matrix[i][j] = 1;
			}
        }
    }
}

