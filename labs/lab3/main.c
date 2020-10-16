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
#include "command.h"
/*----------------------------------------------------------------------------*/

/*------------------------------Program Main----------------------------------*/
int main() {
	/* Main Function Variable */

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

	char *lfcat_str = "lfcat";
	char *f_flag = "-f";

	/*main run loop*/
	while (1) {
		/* Print >>> then get the input string */
		do {
			printf(">>> ");
			int tok_len = getline(&buff, &n, stdin);
		} while (strcmp(buff, "\n") == 0);

		/* Tokenize the input string */
		char *token = strtok_r(buff, delimiters, &tokens);

		/* If the user entered <exit> then exit */
		if (strncmp(exit, token, 4) == 0 || token == NULL) {
			break;
		}

		/* If the user entered a command */
		if (strncmp(token, lfcat_str, strlen(token) - 1) == 0) {
			lfcat();
			break;
		}
		else {
			printf("Error: Unrecognized command!\n");
		}
	}
	/*Free the allocated memory*/
	free(saveptr);
	free(buff);
	return 1;
}
/*-----------------------------Program End------------------------------------*/


