/*
@file
Veri yapıları ve Algoritma 3. Ödev Problem 1
Worlds Series probleminin recursive ve dynamic 
programming yaklaşımı ile çözümünü içermektedir.
@author
Name: Emre Çelik
Student No: 15011902
Date: 20/12/2020
E-mail: emrecelikk97@gmail.com
Complier Used: GCC 4.2.1 Apple LLVM Version 12.1.0 
IDE: Visual Studio Code v1.52.1
Operating System: MacOS Big Sur
CPU: 1.6 GHz Intel Core i5
RAM: 8 GB 1600 MHz DDR3
Video Card: Intel HD Graphics 6000 1536 MB
*/
#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>  

#define DEBUG false	// if it is true, debug print will work
#define WINRATE 0.4
#define n 4			// Win number

double P(int, int);
void clearMatrix(double [n+1][n+1]);
void printMatrix(double [n+1][n+1]);
double series();

int main(int argc, char const *argv[]) {
	printf("Recursive Approach: %f\n", P(4,4));
	printf("Dynamic Approach:   %f", series());
    return 0;
}

// Recursive approach
// p(i,j) = p(i-1,j)*p + p(i,j-1)*(1-p)
double P(int i, int j) {
	if (i == 0)		// Team that we are looking for, because of that, we return 1
		return 1;
	else if(j == 0)
		return 0;
	else
		return P(i-1, j)*WINRATE + P(i,j-1)*(1-WINRATE);
}

// Filling zeros to matrix
void clearMatrix(double matrix[n+1][n+1]) {
	int i; 
	int j;
	for (i = 0; i <= n; i++) {
		for (j = 0; j <= n; j++)
			matrix[i][j] = 0;
	}
}

// Printing matrix for debug
void printMatrix(double matrix[n+1][n+1]) {
	int i; 
	int j;
	printf("Dynamic Table\n");
	for (i = 0; i <= n; i++){
		for (j = 0; j <= n; j++)
			printf("%f ", matrix[i][j]);
		printf("\n");
	}
}

// Dynamic programming approach
double series() {
	int i;
	int j;
	double matrix[n+1][n+1];
	clearMatrix(matrix);
	matrix[0][0] = 1;
	for (i = 1; i <= n; i++)	// Team that we are looking for, because of that, we return 1
		matrix[0][i] = 1;

	for (i = 1; i <= n; i++) {	// Calculating the win rate
		for (j = 1; j <= n; j++)
			matrix[i][j] = (WINRATE*matrix[i-1][j]) + ((1-WINRATE)*matrix[i][j-1]);	
	}
	if(DEBUG)
		printMatrix(matrix);
	return matrix[n][n];		// returns the win rate
}