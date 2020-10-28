/*
* LAB 4
*
* Description: Lab 4 Submission
*
* Author: Jarett Nishijo
* Date: 10/29/2020
*
* Notes:
*	1.
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

//sigemptyset
//sigaddset
//sigprocmask

void script_print(pid_t* pid_ary, int size)
{
	FILE *fout;
	fout = fopen("top_script.sh", "w");
	fprintf(fout, "#!/bin/bash\ntop");
	for (int i =  0 ; i < size ; i++)
	{
		fprintf(fout, " -p %d", (int)(pid_ary[i]));
	}
	fprintf(fout, "\n");
	fclose(fout);
}

int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		fprintf(stderr, "Error: unspecified amount of child processes\n");
		fprintf(stderr, "Usage: ./a.out <amount of child processes\n");
		exit(-1);
	}

	int n = atoi(argv[1]);

	pid_t pid_array[n];

	for (int i = 0 ; i < n ; i++) {
		pid_array[i] = fork();
		printf("my pid = %d, my parent = %d\n",getpid(), getppid());
		execlp("./iobound", "./iobound", "-seconds", "5");
	}

	script_print(pid_array, 5);

	for (int i = 0 ; i < n ; i++)
	{
		wait(0);
	}
	//Step 1 and 2
	//pid_t child_pid = fork();
	//execlp("./iobound", "./iobound", "-seconds", "5");
}
