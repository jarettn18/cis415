/*
 * Description: <write a brief description
 *
 * Author: Jarett Nishijo
 *
 * Date: 10/06/2020
 *
 * Notes:
 * 1.
 */

/*--------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/

/*------------------------------Program Main----------------------------------*/
int main(int argc, char *argv[]) {
	/* Main Function Variable */
	if (argc < 2) {
		printf("Usage: ./a.out (filename)\n");
		exit(EXIT_FAILURE);
		return -1;
	}
	char *filename = argv[1];

	FILE *fp = fopen(filename, "r");
	FILE *fp2 = fopen("output.txt", "w");
	if (fp == NULL) {
		printf("Error opening file '%s'\n",filename);
		exit(EXIT_FAILURE);
		return -1;
	}

	/*Allocate memory for the input buffer. */
	//size of char since getline will realloc memory
	char *buff = malloc(sizeof(char));
	//max # of tokens = size of buff 
	char *tokens = malloc(sizeof(buff) + 1);
	//Keep track of beginning of tokens to prevent memory leaks
	char *saveptr = tokens;
	size_t n = 1;
	char *exit = "exit";
	char *delimiters = " ";
	int i = 1;
	/*main run loop*/
	while (1) {
		/* Print >>> then get the input string */
		do {
			int tok_len = getline(&buff, &n, fp);
		} while (strcmp(buff, "\n") == 0);
		/* Tokenize the input string */
		char *token = strtok_r(buff, delimiters, &tokens);
		/* If the user entered <exit> then exit */
		if (strncmp(exit, token, 4) == 0 || token == NULL) {
			break;
		}
		/*Display each token */
		fprintf(fp2,"T%d: %s\n",0,token);
		while (token = strtok_r(NULL, delimiters, &tokens)) {
			fprintf(fp2, "T%d: %s\n",i ,token);	
			i++;
		}
	}
	fseek(fp, 0, SEEK_SET);
	fseek(fp2, 0, SEEK_SET);
	fclose(fp);
	fclose(fp2);
	/*Free the allocated memory*/
	free(saveptr);
	free(buff);
	return 1;
}
/*-----------------------------Program End------------------------------------*/


