/*
* PROJECT 2
*	Part 4
*
* Description: MCP Part 4
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

void print_process_stats(pid_t pid) {
	char pid_s[100] = {0};
	sprintf(pid_s,"/proc/%d/status", pid);

	int fd = open(pid_s, O_RDONLY);
	if (fd == -1) {
		printf("***NO STATS***\nPROCESS HAS TERMINATED\n");
		return;
	}
	char *buff = malloc(sizeof(char) * 100);
	for (int i = 0 ; i < 6 ; i++) {
		int bytes = read(fd, buff, 1);
		while (strncmp(buff, "\n", 1) != 0) {
			write(1, buff, bytes);
			bytes = read(fd, buff, 1);
		}
		write(1, "\n", 1);
	}
	free(buff);
	close(fd);
}

void sig_child(pid_t *pid_pool, int size, int sig) {
	for (int i = 0 ; i < size ; i++) {
		kill(pid_pool[i], sig);
	}
}

int alive_process(pid_t *pid_pool, int size) {
	int status;
	for (int i = 0 ; i < size ; i++) {
		pid_t ret_pid = waitpid(pid_pool[i], &status, WNOHANG);
		if (ret_pid == 0) {
			return 1;
		}
	}
	return 0;
}

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

	int sig;
	sigset_t set;

	int parent_sig;
	sigset_t parent_set;

	sigemptyset(&parent_set);
	sigaddset(&parent_set, SIGALRM);
	sigprocmask(SIG_BLOCK, &parent_set, NULL);

	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);

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
			fprintf(stdout, "Process %d: WAITING FOR SIGUSR1\n",getpid());
			sigwait(&set, &sig);
			fprintf(stdout, "Process %d: RECEIVED SIGUSR1\n",getpid());
			int exec_succ = execvp(cmd,args);
			if (exec_succ == -1) {
				fprintf(stderr, "COMMAND \"%s\" INVALID\n",cmd);
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
	//fprintf(stdout, "SENDING SIGUSR1\n");
	sig_child(pid_array, pid_i, SIGUSR1);

	fprintf(stdout, "STOPPING ALL FORKED PROCESSES\n");
	sig_child(pid_array, pid_i, SIGSTOP);
	while (alive_process(pid_array, pid_i) == 1){
		printf("=====Processes alive still=====\n");
		for (int i = 0 ; i < pid_i ; i++) {
			alarm(1);

			printf("\nProcess %d: Continued\n",pid_array[i]);
			kill(pid_array[i], SIGCONT);

			printf("Parent waiting for ALRM\n");
			sigwait(&parent_set, &parent_sig);

			printf("Parent received ALRM: Stopping process %d\n",pid_array[i]);
			kill(pid_array[i], SIGSTOP);
		}
		for (int i = 0 ; i < pid_i ; i++) {
			printf("----------------------------------------\n");
			printf("PROCESS %d STATS\n\n",pid_array[i]);
			print_process_stats(pid_array[i]);
			printf("\n--------------------------------------\n");
		}
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


