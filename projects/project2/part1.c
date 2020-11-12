/*
* PROJECT 2
*	Part 1
*
* Description: MCP Part 1
*
* Author: Jarett Nishijo
* Date:
* 
* Notes:
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_LINE_LEN 50

int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "ERROR: WRONG NUM OF ARGUMENTS BITCH\n");
		exit(EXIT_FAILURE);
	}
	
	freopen(argv[1], "r+", stdin);

	size_t n = (size_t) MAX_LINE_LEN;
	char *buff = malloc(sizeof(char) * MAX_LINE_LEN);
	char *buff2 = malloc(sizeof(char) * MAX_LINE_LEN);
	char *saveptr = buff;
	char *saveptr2 = buff2;

	pid_t pid_array[25];
	int pid_i = 0;

	int condition = 1;

	char *cmd = malloc(sizeof(char) * MAX_LINE_LEN);
	char *cmd_sp = cmd;
	
	char **args = malloc(sizeof(char *) * MAX_LINE_LEN);
	char **args_sp = args;
	for (int i = 0 ; i < MAX_LINE_LEN ; i++) {
		args[i] = malloc(sizeof(char) * MAX_LINE_LEN);
	}
	
	while (condition != -1) {
		condition = getline(&buff, &n, stdin);

		if (condition == -1) {
			break;
		}

		pid_array[pid_i] = fork();

		if (pid_array[pid_i] < 0) {
			fprintf(stderr, "ERROR: PROCESS NOT CREATED");
			exit(EXIT_FAILURE);
		}
		if (pid_array[pid_i] == 0) {
			int args_i = 1;
			char *token;

			cmd = strtok_r(buff, " \n", &buff2);
			strcpy(args[0], cmd);
			while ((token = strtok_r(NULL, " \n", &buff2)) != NULL) {
				strcpy(args[args_i], token);
				args_i++;
			}
			char *args_sp = args[args_i];
			args[args_i] = NULL;
			int exec_succ = execvp(cmd,args);
			if (exec_succ == -1) {
				fprintf(stderr,"COMMAND \"%s\" INVALID\n", cmd);
				for (int i  = 0 ; i < MAX_LINE_LEN ; i++) {
					free(args[i]);
				}
				free(args_sp);
				free(args);
				free(cmd_sp);
				fclose(stdin);
				free(saveptr);
				free(saveptr2);
				exit(-1);
			}
			for (int i  = 0 ; i < MAX_LINE_LEN ; i++) {
				free(args[i]);
			}
			free(args);
			free(cmd_sp);
			fclose(stdin);
			free(saveptr);
			free(saveptr2);
			exit(0);
		}
		pid_i++;
	}
	for(int i = 0 ; i < pid_i ; i++) {
		wait(0);
	}
	for (int i  = 0 ; i < MAX_LINE_LEN ; i++) {
		free(args[i]);
	}
	free(args);
	free(cmd_sp);
	fclose(stdin);
	free(saveptr);
	free(saveptr2);
	exit(0);
}


