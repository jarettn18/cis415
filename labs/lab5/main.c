/*
* LAB 4
*
* Description: Lab 5 Submission
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

void sig_child(pid_t *pid_pool, int size, int sig) {

	sleep(2);
	for (int i = 0 ; i < size ; i++) {
		fprintf(stdout, "Parent process: %d - Sending signal: %s to child process: %d\n", getpid(), strsignal(sig), pid_pool[i]);
		kill(pid_pool[i], sig);
	}
}

int main(int argc, char *argv[]) 
{
	if (argc < 2) {
		fprintf(stderr, "Error: unspecified amount of child processes\n");
		fprintf(stderr, "Usage: ./a.out <amount of child processes>\n");
		exit(-1);
	}

	int n = atoi(argv[1]);
	int parent_pid = getpid();
	
	int sig;
	sigset_t set;

	pid_t pid_array[n];

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);

	//Step 3 and 4
	for (int i = 0 ; i < n ; i++) {
		if (getpid() == parent_pid){
			pid_array[i] = fork();
		}
		if (pid_array[i] == 0){
			fprintf(stdout, "Child Process:%d - Waiting for SIGUSR1\n",getpid());
			sigwait(&set, &sig);
			fprintf(stdout, "Child Process:%d - Received signal: SIGUSR1 - Calling exec()\n",getpid());
			execlp("./iobound", "./iobound", "-seconds", "5");
		}
	}
	if (getpid() == parent_pid) {
		sig_child(pid_array, n, SIGUSR1);
		sig_child(pid_array, n, SIGSTOP);
		sleep(5);
		sig_child(pid_array, n, SIGCONT);
		sleep(3);
		sig_child(pid_array, n, SIGINT);
	}
	script_print(pid_array, n);

	for (int i = 0 ; i < n ; i++) {
		wait(0);
	}

	//Step 1 and 2
	//pid_t child_pid = fork();
	//execlp("./iobound", "./iobound", "-seconds", "5");
}
