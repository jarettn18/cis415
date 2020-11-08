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
	
	//freopen(argv[1], "r+", stdin);

	int condition = 1;
	size_t n = (size_t) MAX_LINE_LEN;
	char *buff = malloc(sizeof(char) * MAX_LINE_LEN);

	while (condition != -1) {
		condition = getline(&buff, &n, stdin);
		printf("%s",buff);
		condition = -1;
	}
	fclose(stdin);
	free(buff);
}


