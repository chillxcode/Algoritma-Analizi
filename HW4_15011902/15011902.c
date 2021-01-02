/*
@file
Algoritma Analizi 4. Ödev
Dinamik programlama ile renk sıralama yapılmaktadır.
@author
Name: Emre Çelik
Student No: 15011902
Date: 03/01/2021
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

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define DEBUG false	// if it is true, debug print will work
#define N 6

void clearMatrix(int[N][N]);
void copyMatrix(int[N][N], int[N][N]);
void printMatrix(int[N][N]);
bool isSafe(int[N][N], int, int);
bool solveUtil(int[N][N], int, int);
bool solve(int[N][N], char*);
void fillMatrix(int[N][N], int[N], char*);
void printColourfulMatrix(int[N][N], char*);
int findRowIndex(int[N], int);

int main(int argc, char const *argv[]) {
	char colors[7] = {'R', 'G', 'Y', 'B', 'M', 'C', 'W'};
	// int matrix[N][N] = {
	// 	{4, 5, 3},
	// 	{4, 5, 3},
	// 	{3, 4, 5}
	// };
	// int matrix[N][N] = {
	// 	{3, 4, 1, 2},
	// 	{1, 2, 3, 4},
	// 	{3, 4, 1, 1},
	// 	{1, 2, 3, 4}
	// };
	// int matrix[N][N] = {
	// 	{5, 3, 1, 2, 4},
	// 	{2, 4, 5, 3, 1},
	// 	{4, 5, 3, 1, 2},
	// 	{5, 3, 1, 2, 4},
	// 	{3, 1, 2, 4, 5}
	// };
	int matrix[N][N] = {
		{6, 1, 2, 3, 4, 5},
		{5, 6, 1, 2, 3, 4},
		{6, 1, 2, 3, 4, 5},
		{1, 2, 3, 4, 5, 6},
		{1, 2, 3, 4, 5, 6},
		{2, 3, 4, 5, 6, 1}
	};


	printMatrix(matrix);
	solve(matrix, colors);

    return 0;
}

// Clear matrix except matrix[0][0]
void clearMatrix(int matrix[N][N]) {
	int firstValue = matrix[0][0];
	int i; 
	int j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)
			if (matrix[i][j] != firstValue)
				matrix[i][j] = 0;
	}
}

void copyMatrix(int dest[N][N], int source[N][N]) {
	int i; 
	int j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)
			dest[i][j] = source[i][j];
	}
}

// Print matrix for debug
void printMatrix(int matrix[N][N]) {
	int i; 
	int j;
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
	printf("\n");
}

// Checking upper columns for availability
bool isSafe(int board[N][N], int row, int col) { 
	int i;
	int j;
	//Check this column on top side
	for (i = 0; i < row; i++) 
		if (board[i][col] != 0)
			return false;
	return true; 
}

bool solveUtil(int board[N][N], int row, int firstValue) { 
	int i;
	// If all colors are placed then return true
	if (row >= N) 
		return true; 

	int skipSize = findRowIndex(board[row], firstValue);
	board[row][skipSize] = 0;

	if (DEBUG)
		printf("col: %d\n", row);

	for (i = skipSize; i < N; i++) { // Column
		// Check if the color can be placed on board[row][i]

		if (isSafe(board, row, i)) { 
			// Place this color in board[row][i]
			board[row][i] = firstValue;

			if (DEBUG)
				printMatrix(board);

			// Recursive to place rest of the colors
			if (solveUtil(board, row + 1, firstValue)) 
				return true; 

			board[row][i] = 0; // Backtrack 
		}
	} 
	return false; 
}

// Main solve function
bool solve(int matrix[N][N], char* colors) { 
	int board[N][N];	// Solution matrix
	copyMatrix(board, matrix);
	clearMatrix(board); 

	if (solveUtil(board, 0, board[0][0]) == false) { 
		printf("Solution does not exist"); 
		return false; 
	}
	printf("Solution:\n");
	printMatrix(board); 
	fillMatrix(board, matrix[0], colors);
	return true; 
} 

// Fill solution matrix with color sequence
void fillMatrix(int matrix[N][N], int sequence[N], char* colors) { 
	int index;
	int i;
	int j;
	for(i = 0; i < N; i++) {
		int skipSize = findRowIndex(matrix[i], matrix[0][0]);
		for(j = 0; j < N; j++) {
			index = (j + skipSize) % N;
			matrix[i][index] = sequence[j];
		}
	}
	if (DEBUG)
		printMatrix(matrix);
	printColourfulMatrix(matrix, colors);
}

// Print solution matrix with colors
void printColourfulMatrix(int matrix[N][N], char* colors) {
	int i;
	int j;
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			switch (colors[matrix[i][j]]) {
			case 'R':
				printf(ANSI_COLOR_RED"R"ANSI_COLOR_RESET" ");
				break;
			case 'G':
				printf(ANSI_COLOR_GREEN"G"ANSI_COLOR_RESET" ");
				break;
			case 'Y':
				printf(ANSI_COLOR_YELLOW"Y"ANSI_COLOR_RESET" ");
				break;
			case 'B':
				printf(ANSI_COLOR_BLUE"B"ANSI_COLOR_RESET" ");
				break;
			case 'M':
				printf(ANSI_COLOR_MAGENTA"M"ANSI_COLOR_RESET" ");
				break;
			case 'C':
				printf(ANSI_COLOR_CYAN"C"ANSI_COLOR_RESET" ");
				break;
			case 'W':
				printf(ANSI_COLOR_WHITE"W"ANSI_COLOR_RESET" ");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
}

// Returns array index of value
int findRowIndex(int array[N], int value) {
	int i;
	for(i = 0; i < N; i++)
		if (array[i] == value)
			return i;
	return -1;
}