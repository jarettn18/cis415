#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

#define MAX_TOK_LEN 60

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
	char *token = malloc(sizeof(char) * MAX_TOK_LEN);
	char *saveptr = tokens;
	char *saveptr_tok = token;

	int num_args;
	char **args = (char **)malloc(sizeof(char *) * MAX_TOK_LEN);
	for (int i = 0 ; i < MAX_TOK_LEN ; i++) {
		args[i] = (char *)malloc(sizeof(char) * MAX_TOK_LEN);
	}

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
		token = strtok_r(buff, delimiters, &tokens);
		
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
			//Fill Array of strings with arguments until semicolon or NULL is reached
			num_args = 0;
			while (token && !(strncmp(token, ";", strlen(token)) == 0 && strlen(token) == strlen(";"))) {
				token = strtok(token, "\n");
				args[num_args] = token;
				token = strtok_r(NULL, delimiters, &tokens);
				num_args++;
			}
			if (args[0] == NULL) {
				args[0] = " ";
			}
			if (strncmp(args[0], "ls", strlen(args[0])) == 0 && strlen(args[0]) == strlen("ls")) {
				if (num_args > 1) {
					fprintf(stderr, "ERROR: TOO MANY ARGUMENTS\n");
					return -1;
				}
				else {
					listDir();
				}
			}
			else if (strncmp(args[0], "pwd", strlen(args[0])) == 0 && strlen(args[0]) == strlen("pwd")) {
				if (num_args > 1) {
					fprintf(stderr, "ERROR: TOO MANY ARGUMENTS\n");
					return -1;
				}
				else {
					showCurrentDir();
				}
			}
			else if (strncmp(args[0], "mkdir", strlen(args[0])) == 0 && strlen(args[0]) == strlen("mkdir")) {
				if (num_args > 2) {
					fprintf(stderr, "ERROR: TOO MANY ARGUMENTS\n");
					return -1;
				}
				else if (num_args < 2) {
					fprintf(stderr, "ERROR: TOO FEW ARGUMENTS\n");
					return -1;
				}
				else {
					makeDir(args[1]);
				}
			}
			else if (strncmp(args[0], "cd", strlen(args[0])) == 0 && strlen(args[0]) == strlen("cd")) {
				if (num_args > 2) {
					fprintf(stderr, "ERROR: TOO MANY ARGUMENTS\n");
					return -1;
				}
				else if (num_args < 2) {
					fprintf(stderr, "ERROR: TOO FEW ARGUMENTS\n");
					return -1;
				}
				else {
					changeDir(args[1]);
				}
			}
			else if (strncmp(args[0], "rm", strlen(args[0])) == 0 && strlen(args[0]) == strlen("rm")) {
				printf("rm called\n");
				if (num_args > 2) {
					fprintf(stderr, "ERROR: TOO MANY ARGUMENTS\n");
					return -1;
				}
				else if (num_args < 2) {
					fprintf(stderr, "ERROR: TOO FEW ARGUMENTS\n");
					return -1;
				}
				else {
					deleteFile(args[1]);
				}
			}
			else if (strncmp(args[0], "cat", strlen(args[0])) == 0 && strlen(args[0]) == strlen("cat")) {
				if (num_args > 2) {
					fprintf(stderr, "ERROR: TOO MANY ARGUMENTS\n");
					return -1;
				}
				else if (num_args < 2) {
					fprintf(stderr, "ERROR: TOO FEW ARGUMENTS\n");
					return -1;
				}
				else {
					displayFile(args[1]);
				}
			}
			else if (strncmp(args[0], "cp", strlen(args[0])) == 0 && strlen(args[0]) == strlen("cp")) {
				printf("cp called\n");
				if (num_args > 3) {
					fprintf(stderr, "ERROR: TOO MANY ARGUMENTS\n");
					return -1;
				}
				else if (num_args < 3) {
					fprintf(stderr, "ERROR: TOO FEW ARGUMENTS\n");
					return -1;
				}
				else {
					copyFile(args[1], args[2]);
				}
			}
			else if (strncmp(args[0], "mv", strlen(args[0])) == 0 && strlen(args[0]) == strlen("mv")) {
				printf("mv called\n");
				if (num_args > 3) {
					fprintf(stderr, "ERROR: TOO MANY ARGUMENTS\n");
					return -1;
				}
				else if (num_args < 3) {
					fprintf(stderr, "ERROR: TOO FEW ARGUMENTS\n");
					return -1;
				}
				else {
					moveFile(args[1], args[2]);
				}
			}
			else {
				printf("ERROR: Unrecognized Command!\n");
			}
		} while (token = strtok_r(NULL, delimiters, &tokens));
	}
	for (int i = 0 ; i < MAX_TOK_LEN ; i++) {
		free(args[i]);
	}
	free(args);
	free(buff);
	free(saveptr);
	free(saveptr_tok);
}
