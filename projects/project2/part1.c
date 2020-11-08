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
	char *saveptr = buff;
	char *delim = " ";

	pid_t pid_array[25];
	int pid_i = 0;

	int condition = 1;

//	char *cmd = malloc(sizeof(char) * MAX_LINE_LEN);
//	char **args = malloc(sizeof(char *) * MAX_LINE_LEN);
//	for (int i = 0 ; i < MAX_LINE_LEN ; i++) {
//		args[i] = malloc(sizeof(char) * MAX_LINE_LEN);
//	}

	while (condition != -1) {
		condition = getline(&buff, &n, stdin);
		if (condition == -1) {
			break;
		}
		pid_array[pid_i] = 0;
		if (pid_array[pid_i] < 0) {
			fprintf(stderr, "ERROR: PROCESS NOT CREATED");
			exit(EXIT_FAILURE);
		}
		if (pid_array[pid_i] == 0) {
			int args_i = 0;
			char *token;
			/*
			cmd = strtok_r(buff, delim, &buff);
			while (token = strtok_r(NULL, delim, &buff)) {
				args[args_i] = token;
				args_i++;
			}
			printf("cmd: %s\n",cmd);
			for (int i = 0 ; i < args_i ; i++) {
				printf("arg %d: %s\n", i, args[i]);
			}
			*/
		}
	}
	/*
	for (int i  = 0 ; i < MAX_LINE_LEN ; i++) {
		free(args[i]);
	}
	free(args);
	*/
	//free(cmd);
	fclose(stdin);
	free(buff);
}


