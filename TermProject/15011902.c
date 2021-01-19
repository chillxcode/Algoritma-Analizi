/*
@file
Algoritma Analizi Proje Ödevi
Kitap öneri sistemi uygulamasıdır.
@author
Name: Emre Çelik
Student No: 15011902
Date: 20/01/2021
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
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define FILE_PATH "recdataset.txt"
#define DEBUG false	// if it is true, debug print will work
#define ROW 26
#define COL 9
#define STRING_LENGHT 150

void readTXT(char[ROW][COL][STRING_LENGHT]);
void printMatrix(char[ROW][COL][STRING_LENGHT]);
int getUserIndex(char[STRING_LENGHT], char[ROW][COL][STRING_LENGHT]);
double averageUser(int, char[ROW][COL][STRING_LENGHT]);
double sim(int, int, char[ROW][COL][STRING_LENGHT]);
int* getSimilarUsers(int, char[ROW][COL][STRING_LENGHT]);
void pred(int, int*, char[ROW][COL][STRING_LENGHT]);

int K = 1;
char user[STRING_LENGHT];

int main(int argc, char const *argv[]) {
    char matrix[ROW][COL][STRING_LENGHT];   // Text file input matrix

    readTXT(matrix);
    if (DEBUG)
        printMatrix(matrix);
    
    printf("Kitap onerisi yapilacak kullanici adi: ");
    scanf("%s", user);
    printf("Benzer kullanici sayisi(k): ");
    scanf("%d", &K);

    int *p = getSimilarUsers(getUserIndex(user, matrix), matrix);

    pred(getUserIndex(user, matrix), p, matrix);

    return 0;
}

// Reading text file
void readTXT(char inputMatrix[ROW][COL][STRING_LENGHT]) {
    int i = 0;
    int j = 0;
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Unable to open the file.");
        exit(-1);
    }

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        char *token;
        token = strtok(line, ";");
        while (token != NULL) {
            if (strlen(token) > 0 && token[strlen(token) -1] == '\n')
                token[strlen(token) -1] = '\0';

            if (strcmp(token, " ") == 0)
                strcpy(inputMatrix[i][j++], "0");
            else
                strcpy(inputMatrix[i][j++], token);
            token = strtok(NULL, ";");
        }
        j = 0;
        i++;
    }
}

// Printing matrix for debug
void printMatrix(char inputMatrix[ROW][COL][STRING_LENGHT]) {
	int i; 
	int j;
	for (i = 0; i < ROW; i++){
		for (j = 0; j < COL; j++)
			printf("%s ", inputMatrix[i][j]);
		printf("\n");
	}
	printf("\n");
}

// Get specific user row in matrix
int getUserIndex(char user[STRING_LENGHT], char inputMatrix[ROW][COL][STRING_LENGHT]) {
    int i;
    for(i = 0; i < ROW; i++)
        if (strcmp(user, inputMatrix[i][0]) == 0)
            return i;
    return -1;
}

// Average of books voted by the user
double averageUser(int index, char inputMatrix[ROW][COL][STRING_LENGHT]) {
    int i;
    double mean = 0;
    double count = 0;
    for (i = 1; i < COL; i++) {     //Calculating meanA
        if (atoi(inputMatrix[index][i]) != 0){
            mean += atoi(inputMatrix[index][i]);
            count++;
        }
    }
    mean /= count;
    return mean;
}

// Pearson coefficient
double sim(int a, int b, char inputMatrix[ROW][COL][STRING_LENGHT]) {
    int i;
    double meanA = averageUser(a, inputMatrix);
    double meanB = averageUser(b, inputMatrix);
    
    double total1 = 0;
    double total2 = 0;
    double total3 = 0;

    for (i = 1; i < COL; i++) {
        if (atoi(inputMatrix[a][i]) != 0 && atoi(inputMatrix[b][i]) != 0) {
            double itemForA = atoi(inputMatrix[a][i]) - meanA;
            double itemForB = atoi(inputMatrix[b][i]) - meanB;

            total1 += (itemForA * itemForB);
            total2 += (itemForA * itemForA);
            total3 += (itemForB * itemForB);
        }
    }
    return total1 / (sqrt(total2 * total3));
}

// returns similar users from similar to different
int* getSimilarUsers(int user, char inputMatrix[ROW][COL][STRING_LENGHT]) {
    int *similarUsers = malloc(K * sizeof(int));
    int i = 1;
    int j;
    int k;
    double tmpSim = 0;

    for (j = 0; j < K; j++)     // reset similar user array
        similarUsers[j] = -1;

    while (inputMatrix[i][0][0] != 'N') {
        tmpSim = sim(user, i, inputMatrix);
        if (DEBUG)
            printf("%d %s: %f\n", i, inputMatrix[i][0], tmpSim);

        for (j = 0; j < K; j++) {   
            if (similarUsers[j] == -1) {    // Empty user, insert
                similarUsers[j] = i;
                j = K;
            }
            else if (tmpSim > sim(user, similarUsers[j], inputMatrix)) {    // if better, insert similar array
                for (k = K-1; k > j; k--)   // Shift to right
                    similarUsers[k] = similarUsers[k-1];
                similarUsers[j] = i;        // Insert
                j = K;
            }
        }
        i++;
    }

    printf("\n");
    for (i = 0; i < K; i++)
        printf("%d. %s, %f\n", i+1, inputMatrix[similarUsers[i]][0], sim(user, similarUsers[i], inputMatrix));
    printf("\n");

    return similarUsers;
}

// Prediction func
void pred(int user, int* similarUsers, char inputMatrix[ROW][COL][STRING_LENGHT]) {
    int i;
    int j;
    double meanA = averageUser(user, inputMatrix);
    double meanB;
    
    double totalUpper = 0; 
    double totalLower = 0;
    double pred;

    double bestRate = 0;
    char bookName[STRING_LENGHT];

    for (j = 1; j < COL; j++) {
        if (atoi(inputMatrix[user][j]) == 0) {
            for (i = 0; i < K; i++) {
                if (atoi(inputMatrix[similarUsers[i]][j]) != 0) {
                    meanB = averageUser(similarUsers[i], inputMatrix);
                    totalUpper += sim(user, similarUsers[i], inputMatrix) * (atoi(inputMatrix[similarUsers[i]][j]) - meanB);
                    totalLower += sim(user, similarUsers[i], inputMatrix);
                }
            }
            pred = meanA + totalUpper / totalLower;
            printf("%d. '%s', %f\n", j, inputMatrix[0][j], pred);

            if (pred > bestRate) {
                bestRate = pred;
                strcpy(bookName, inputMatrix[0][j]);
            }
            totalUpper = 0;
            totalLower = 0;
        }
    }
    printf("\nSonuc olarak onerilen kitap: %s\n\n", bookName);
}