/*
@file
Veri yapıları ve Algoritma 3. Ödev Problem 2
Sözlük uygulaması yapılmıştır. Girilen cümledeki kelimeleri
sözlükte arar. Sözlükte yoksa önceden kullanıcının girdiği bad dictionary' de
o kelimeyi arar ve önerir. Kullanıcının önceden seçimi yoksa sözlük içinden
benzer kelimeler kullanıcıya sunulur.
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
#include <string.h>
#include <locale.h>
#include <math.h>
#include <limits.h>
#include<stdbool.h>  

#define DEBUG false		// If it is true, debug print will work
#define MAXCHAR 100		// String char size
#define DICTSIZE 997	// Dictionary array length
#define PRIME 13		// Prime number for Horner's Method
#define DICTFILE "smallDictionary.txt"
#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

typedef struct HashLine {		// Dictionary line struct
	int key;
	char value[MAXCHAR];
} HashLine;

typedef struct BadHashLine {	// Users choice dictionary line struct
	int key;
	char value[MAXCHAR];
	char choice[MAXCHAR];
} BadHashLine;

void clearDict(HashLine*);
void clearBadDict(BadHashLine*);
FILE *fileOpen(char*, char*);
void readFileToDict(FILE*, HashLine*);
int getHornerValue(const char*);
int levenshtein(char*, char*, int);
void insertHash(const char*, int, HashLine*);
void insertBadHash(const char*, const char*, int, BadHashLine*);
int searchHash(const char*, HashLine*);
int searchBadHash(const char*, BadHashLine*, char[MAXCHAR]);
void printDict(HashLine*);
void printBadDict(BadHashLine*);
void clearStrArr(char [DICTSIZE][MAXCHAR]);
void printWords(char [DICTSIZE][MAXCHAR]);
void getWords(char[MAXCHAR], char[DICTSIZE][MAXCHAR]);
int getSimilarWordsInDict(char [MAXCHAR], char [DICTSIZE][MAXCHAR], HashLine*, int);
void printRecommendation(char [MAXCHAR], char [DICTSIZE][MAXCHAR]);
void resolve(char [DICTSIZE][MAXCHAR], HashLine*, BadHashLine*);

int loadFactor = 0;		// Dictionary load factor saved value
int badLoadFactor = 0;	// Bad Dictionary load factor saved value

int main(int argc, char const *argv[]) {
    FILE *file;
    struct HashLine dictionary[DICTSIZE];		// smallDictionary.txt words
    struct BadHashLine badDictionary[DICTSIZE];	// users choice words
	char sentence[MAXCHAR];						// user input sentence
	char words[DICTSIZE][MAXCHAR];				// user input sentence words (splitted with ' ')

	clearDict(dictionary);
	clearBadDict(badDictionary);
    file = fileOpen(DICTFILE, "r");
    readFileToDict(file, dictionary);

	if (DEBUG)
		printDict(dictionary);
	
	while (true){
		fflush(stdin);					// This is for scanf bug
		scanf("%100[^\n]s", sentence);	// 100 for buffer overflow
		fflush(stdin);

		getWords(sentence, words);

		resolve(words, dictionary, badDictionary);

		printWords(words);
		if(DEBUG)
			printBadDict(badDictionary);

		strcpy(sentence, "");			// Clear user sentence
		clearStrArr(words);				// Clear user sentence words
	}

	fclose(file);
    return 0;
}

// Cleaning the dictionary, default dict key is -1
void clearDict(HashLine *dictionary) {
	int i;
	for(i = 0; i < DICTSIZE; i++) {
		dictionary[i].key = -1;
		strcpy(dictionary[i].value, "*");
	}
	if(DEBUG)
		printf("Dictionary Cleaned\n");
}

// Cleaning the bad dictionary, default dict key is -1
void clearBadDict(BadHashLine *dictionary) {
	int i;
	for(i = 0; i < DICTSIZE; i++) {
		dictionary[i].key = -1;
		strcpy(dictionary[i].value, "*");
		strcpy(dictionary[i].choice, "*");
	}
	if(DEBUG)
		printf("Bad Dictionary Cleaned\n");
}

FILE *fileOpen(char *name, char *type) {
	FILE *tmp;
	tmp = fopen(name, type);
	if(tmp == NULL) {
		perror("fopen");	// Prints the error reason
		exit(404);
	}
	if(DEBUG)
		printf("%s file opened.\n\n", DICTFILE);
	return tmp;
}

// Filling the dictionary word by word from DICTFILE
void readFileToDict(FILE *file, HashLine *dictionary) {
	char line[MAXCHAR];	//tmp line value
	const char *word;	//splitted 1 word
	if(DEBUG)
		printf("readFileToDict called\n");
	while (fgets(line, MAXCHAR, file) != NULL) {
		word = strtok(line, " ");
		while (word != NULL) {
			insertHash(word, 1, dictionary);	// Word by word inserting the dictionary
            word = strtok(NULL, " ");
        }
	}
	if(DEBUG)
		printf("\n");
}

//Horner's Method	a * 13**3 + a * 13**2 ...
int getHornerValue(const char *word) {
	int i;
	int value = 0;
	int length = strlen(word);		//this is for exponential number
	for(i = length - 1; i >= 0; i--) {
		int ascii = word[i];
		value += ascii * pow(PRIME, i);
		value = value % DICTSIZE;
	}
	return value;
}

// Levenshtein for two words
int levenshtein(char *s1, char *s2, int maxEditDistance) {
    unsigned int x, y, s1len, s2len, i, min;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++)
        matrix[x][0] = matrix[x-1][0] + 1;
    for (y = 1; y <= s1len; y++)
        matrix[0][y] = matrix[0][y-1] + 1;
    for (x = 1; x <= s2len; x++) {
        for (y = 1; y <= s1len; y++)
			matrix[x][y] = MIN3(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));

		min = INT_MAX;					// If edit distance reach the maxEditDistance, returns -1
		for(i = 1; i <= s2len; i++) {	// Getting the current edit distance
			if(matrix[x][i] < min)
				min = matrix[x][i];
		}
		if(min > maxEditDistance)
			return -1;
	}
    return(matrix[s2len][s1len]);		// Returns the edit distance
}

// Inserts the word from DICTFILE to dictionary
// Double Hashing!
void insertHash(const char* value, int i, HashLine *dictionary){
	int hornerValue = getHornerValue(value);
	int h1key = hornerValue;
	int h2key = 1 + (h1key % (DICTSIZE - 1));
	int hashIndex = (h1key + i * h2key) % DICTSIZE;	//this is hash index
	if(loadFactor == DICTSIZE) {
		printf("HASH FULL!!!\n");
		exit(502);
	}
	else if(loadFactor > (DICTSIZE * 0.8))
		printf("Hash is almost full!\n");

	if(dictionary[hashIndex].key == -1) {			//hash index is free
		dictionary[hashIndex].key = hornerValue;
		strcpy(dictionary[hashIndex].value, value);
		loadFactor++;
		if(DEBUG)
			printf("Key: %d Val: %s Load Factor: %.5f\n", hornerValue, value, (float)loadFactor / (float)DICTSIZE);
	}
	else if(dictionary[hashIndex].key != hornerValue)		// hash index full with another key
		insertHash(value, i + 1, dictionary);
	else{												
		if(strcmp(dictionary[hashIndex].value, value) == 0)	// hash index has same value
			printf("Already in hashtable!\n");
		else												// hash index has same key but different value
			insertHash(value, i + 1, dictionary);
	}
}

// Inserts the word from users choice to bad dictionary
// Double Hashing!
void insertBadHash(const char* value, const char* choice, int i, BadHashLine *badDictionary){
	int hornerValue = getHornerValue(value);
	int h1key = hornerValue;
	int h2key = 1 + (h1key % (DICTSIZE - 1));
	int hashIndex = (h1key + i * h2key) % DICTSIZE;	//this is hash index
	if(badLoadFactor == DICTSIZE) {
		printf("HASH FULL!!!\n");
		exit(502);
	}
	else if(badLoadFactor > (DICTSIZE * 0.8))
		printf("Hash is almost full!\n");

	if(badDictionary[hashIndex].key == -1) {		//hash index is free
		badDictionary[hashIndex].key = hornerValue;
		strcpy(badDictionary[hashIndex].value, value);
		strcpy(badDictionary[hashIndex].choice, choice);
		badLoadFactor++;
		if(DEBUG)
			printf("Key: %d Val: %s Load Factor: %.5f\n", hornerValue, value, (float)badLoadFactor / (float)DICTSIZE);
	}
	else if(badDictionary[hashIndex].key != hornerValue)		// hash index full with another key
		insertBadHash(value, choice, i + 1, badDictionary);
	else{
		if(strcmp(badDictionary[hashIndex].value, value) == 0){	// hash index has same value, we changes the current value
			badDictionary[hashIndex].key = hornerValue;
			strcpy(badDictionary[hashIndex].value, value);
			strcpy(badDictionary[hashIndex].choice, choice);
			printf("Hashtable choice is changed!\n");
		}
		else													// hash index has same key but different value
			insertBadHash(value, choice, i + 1, badDictionary);
	}
}

// Searching hash for user input word inside dictionary, it is not working recursively but has double hashing
int searchHash(const char* value, HashLine *dictionary) {
	int i = 1;										// hash index recursive i
	bool foundFlag = false;
	int hornerValue = getHornerValue(value);
	int h1key = hornerValue;
	int h2key = 1 + (h1key % (DICTSIZE - 1));
	int hashIndex = (h1key + i * h2key) % DICTSIZE;	// this is hash index

	while (dictionary[hashIndex].key != -1 && foundFlag == false) {
		if(dictionary[hashIndex].key != hornerValue){
			i++;
			hashIndex = (h1key + i * h2key) % DICTSIZE;
		}
		else{
			if(strcmp(dictionary[hashIndex].value, value) == 0){
				foundFlag = true;
			}
			else{
				i++;
				hashIndex = (h1key + i * h2key) % DICTSIZE;
			}
		}
	}
	if(foundFlag)
		return 1;
	else
		return 0;
}

// Searching hash for user input word inside bad dictionary, it is not working recursively but has double hashing
int searchBadHash(const char* value, BadHashLine *badDictionary, char recommendedWord[MAXCHAR]) {
	int i = 1;										// hash index recursive i
	bool foundFlag = false;
	int hornerValue = getHornerValue(value);
	int h1key = hornerValue;
	int h2key = 1 + (h1key % (DICTSIZE - 1));
	int hashIndex = (h1key + i * h2key) % DICTSIZE;	// this is hash index

	while (badDictionary[hashIndex].key != -1 && foundFlag == false) {
		if(badDictionary[hashIndex].key != hornerValue){
			i++;
			hashIndex = (h1key + i * h2key) % DICTSIZE;
		}
		else{
			if(strcmp(badDictionary[hashIndex].value, value) == 0){
				foundFlag = true;
				strcpy(recommendedWord, badDictionary[hashIndex].choice);
			}
			else{
				i++;
				hashIndex = (h1key + i * h2key) % DICTSIZE;	//this is hash index
			}
		}
	}
	if(foundFlag)
		return 1;
	else
		return 0;
}

// Prints the DICTFILE for debug
void printDict(HashLine *dictionary) {
	int i;
	printf("\nDICTIONARY (index, key, value)\n");
	for(i = 0; i < DICTSIZE; i++) {
		if(dictionary[i].key != -1)
			printf("%d: %d - %s\n", i, dictionary[i].key, dictionary[i].value);
	}
	printf("\n");
}

// Prints the bad dictionary for debug
void printBadDict(BadHashLine *badDictionary) {
	int i;
	printf("\nBAD DICTIONARY (index, key, value, choice)\n");
	for(i = 0; i < DICTSIZE; i++) {
		if(badDictionary[i].key != -1)
			printf("%d: %d - %s - %s\n", i, badDictionary[i].key, badDictionary[i].value, badDictionary[i].choice);
	}
	printf("\n");
}

// Cleaning the string array
void clearStrArr(char words[DICTSIZE][MAXCHAR]) {
	int i;
	for(i = 0; i < DICTSIZE; i++)
		strcpy(words[i], "");
}

// Printing the string array
void printWords(char words[DICTSIZE][MAXCHAR]) {
	int i;
	if(DEBUG)
		printf("\nReaded Words:\n");
	for(i = 0; i < DICTSIZE; i++) {
		if(strcmp(words[i], "") != 0)
			printf("%s\n", words[i]);
	}
	printf("\n");
}

// Splitting user input to word by word (' ')
void getWords(char sentence[MAXCHAR], char words[DICTSIZE][MAXCHAR]) {
	clearStrArr(words);
	int init_size = strlen(sentence);
	char delim[] = " ";
	char *ptr = strtok(sentence, delim);
	int i = 0;

	while (ptr != NULL) {
		strcpy(words[i++], ptr);
		ptr = strtok(NULL, delim);
	}
	if(DEBUG)
		printWords(words);
}

// If user input word is not in dictionary and bad dictionary, we search for similar words in dictionary
int getSimilarWordsInDict(char word[MAXCHAR], char recommendedWords[DICTSIZE][MAXCHAR], HashLine *dictionary, int maxEditDistance) {
	int i;
	int count = 0;	// founded words count
	for (i = 0; i < DICTSIZE; i++) {
		if(dictionary[i].key != -1) {	// If dictionary index is not empty
			int levenshteinValue = levenshtein(word, dictionary[i].value, maxEditDistance);
			if(levenshteinValue == maxEditDistance) {
				strcpy(recommendedWords[count], dictionary[i].value);
				count++;
			}
		}
	}
	return count;
}

// Printing the founded similar words for user
void printRecommendation(char word[MAXCHAR], char recommendedWords[DICTSIZE][MAXCHAR]) {
	int i;
	bool isFirstWord = true;	// This is for pretty print
	printf("'%s' is not in the dictionary. Did you mean: ", word);
	for(i = 0; i < DICTSIZE; i++) {
		if(strcmp(recommendedWords[i], "") != 0) {
			if(!isFirstWord) {
				printf(" or ");
				isFirstWord = false;
			}
			printf("%s ", recommendedWords[i]);
		}
	}
	printf("\n");
}

void resolve(char words[DICTSIZE][MAXCHAR], HashLine *dictionary, BadHashLine *badDictionary) {
	int i;
	char recommendedWords[DICTSIZE][MAXCHAR];
	char recommendedWord[MAXCHAR];
	int wordCount;
	char reReadedWord[MAXCHAR];
	clearStrArr(recommendedWords);
	for(i = 0; i < DICTSIZE; i++) {
		if(strcmp(words[i], "") != 0) {
			if(searchHash(words[i], dictionary) == 0){	// Not found in dictionary

				if(searchBadHash(words[i], badDictionary, recommendedWord) == 0){	// Not found in bad dictionary
				
					int wordCount = getSimilarWordsInDict(words[i], recommendedWords, dictionary, 1);
					if(wordCount > 0) {													// Searching dict words with 1 letter distance
						printRecommendation(words[i], recommendedWords);
					}
					else {																// Searching dict words with 2 letter distance
						wordCount = getSimilarWordsInDict(words[i], recommendedWords, dictionary, 2);
						if(wordCount > 0) {
							printRecommendation(words[i], recommendedWords);
						}
						else {															// Not found anywhere
							printf("'%s' is not found in bad dictionary, 1 and 2 letter distance suggestion.\n", words[i]);
						}
					}
				}
				else {																// Found in bad dictionary
					printf("'%s' is not in the dictionary. Did you mean: '%s'\n", words[i], recommendedWord);
				}
				scanf("%s", reReadedWord);		// Read choice from user
				insertBadHash(words[i], reReadedWord, 1, badDictionary);
				strcpy(words[i], reReadedWord);
				printf("\n");
			}
			else {										// Found in dictionary
				if(DEBUG)
					printf("%s is found in dictionary\n", words[i]);
			}
		}
	}
}