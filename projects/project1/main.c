#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

int main(int argc, char *argv[]) {

	//INTERACTIVE MODE
	if (argc == 1) {
		//DO NOTHING
	}
	//FILE MODE	
	else if (argc == 3) {
		//ERROR CHECK FILE
		if (strncmp("-f",argv[1], 2) != 0) {
			fprintf(stderr, "ERROR: Unrecognized flag '%s'\n",argv[1]);
			fprintf(stderr, "Usage: ./pseudo-shell -f <filename>\n");
			exit(EXIT_FAILURE);
		}
		else {
			printf("Made it here chief\n");
			freopen("output.txt", "w+", stdout);
		}
	}
	else {
		fprintf(stderr, "ERROR: Too many arguments\n");
		fprintf(stderr, "Usage: ./pseudo-shell or ./pseudo-shell -f <filename>");
		exit(EXIT_FAILURE);
	}

	/*Allocate memory for input buffer*/
	char *buff = malloc(sizeof(char));
	char *tokens = malloc(sizeof(buff) + 1);
	char *saveptr = tokens;

	size_t n = 1;

	char *exit = "exit";
	char *delimiters = " ";
	
	while (1) {
		/* Print >>> then get the input string */
		do {
			printf(">>> ");
			getline(&buff, &n, stdin);
		} while (strcmp(buff, "\n") == 0);

		/* Tokenize the input string */
		char *token = strtok_r(buff, delimiters, &tokens);
		
		/* If the user entered <exit> then exit */
		if (strncmp(exit, token, 4) == 0 || token == NULL) {
			break;
		}

		/* If the user entered a command */
		/*
		*	ls : 0
		*	pwd : 0
		*	mkdir : 1
		*	cd : 1
		*	rm : 1
		*	cat : 1
		*	cp : 2
		*	mv : 2
		*/

		//Run token through a strcmp for each command
		do {
			token = strtok(token, "\n");
			if (strlen(token) == 1 || token == NULL) {
				//Not a valid command or end of input. do nothing
			}
			else if (strncmp(token, "ls", strlen(token)) == 0 && strlen(token) == strlen("ls")) {
				printf("ls called\n");
			}
			else if (strncmp(token, "pwd", strlen(token)) == 0 && strlen(token) == strlen("pwd")) {
				printf("pwd called\n");
			}
			else if (strncmp(token, "mkdir", strlen(token)) == 0 && strlen(token) == strlen("mkdir")) {
				printf("mkdir called\n");
			}
			else if (strncmp(token, "cd", strlen(token)) == 0 && strlen(token) == strlen("cd")) {
				printf("cd called\n");
			}
			else if (strncmp(token, "rm", strlen(token)) == 0 && strlen(token) == strlen("rm")) {
				printf("rm called\n");
			}
			else if (strncmp(token, "cat", strlen(token)) == 0 && strlen(token) == strlen("cat")) {
				printf("cat called\n");
			}
			else if (strncmp(token, "cp", strlen(token)) == 0 && strlen(token) == strlen("cp")) {
				printf("cp called\n");
			}
			else if (strncmp(token, "mv", strlen(token)) == 0 && strlen(token) == strlen("mv")) {
				printf("mv called\n");
			}
			else {
				printf("ERROR: Unrecognized Command!\n");
			}
		} while (token = strtok_r(NULL, delimiters, &tokens));
	}


}
