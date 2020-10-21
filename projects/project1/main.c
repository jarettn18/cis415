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
			int tok_len = getline(&buff, &n, stdin);
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
		int x = 1;
		//Run token through a strcmp for each command
		do {
			int num_args = 0;
			char **args;
			while (token && !(strncmp(token, ";", strlen(token)) == 0 && strlen(token) == strlen(";"))) {
				token = strtok(token, "\n");
				args[num_args] = token;
				token = strtok_r(NULL, delimiters, &tokens);
				num_args++;
			}
			for (int j = 0 ;  j < num_args ; j++) {
				printf("%s IN ARG ARRAY\n",args[j]);
			}
			printf("\n");
			if (args[0] == NULL) {
				args[0] = " ";
			}
			if (strncmp(args[0], "ls", strlen(args[0])) == 0 && strlen(args[0]) == strlen("ls")) {
				printf("ls called\n");
			}
			else if (strncmp(args[0], "pwd", strlen(args[0])) == 0 && strlen(args[0]) == strlen("pwd")) {
				printf("pwd called\n");
			}
			else if (strncmp(args[0], "mkdir", strlen(args[0])) == 0 && strlen(args[0]) == strlen("mkdir")) {
				printf("mkdir called\n");
			}
			else if (strncmp(args[0], "cd", strlen(args[0])) == 0 && strlen(args[0]) == strlen("cd")) {
				printf("cd called\n");
			}
			else if (strncmp(args[0], "rm", strlen(args[0])) == 0 && strlen(args[0]) == strlen("rm")) {
				printf("rm called\n");
			}
			else if (strncmp(args[0], "cat", strlen(args[0])) == 0 && strlen(args[0]) == strlen("cat")) {
				printf("cat called\n");
			}
			else if (strncmp(args[0], "cp", strlen(args[0])) == 0 && strlen(args[0]) == strlen("cp")) {
				printf("cp called\n");
			}
			else if (strncmp(args[0], "mv", strlen(args[0])) == 0 && strlen(args[0]) == strlen("mv")) {
				printf("mv called\n");
			}
			else {
				printf("ERROR: Unrecognized Command!\n");
			}
		} while (token = strtok_r(NULL, delimiters, &tokens));
	}


}
