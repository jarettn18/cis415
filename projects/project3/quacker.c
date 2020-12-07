/* =====================  Project 3 ================*/
/*
 * Project 3: Instaquack
 * Author: Jarett Nishijo
 * Date: 11/29/2020
 * 
 * Notes:
 * 	1.
 *
 */

/* ==================== Libraries and definitions ==============*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "quacker.h"
#include "command.h"
#include "queue.h"
#include "string_parser.h"

/* ======================== Global Variables and Structs =================*/
/*
typedef struct {
	int flag;
	int TID;
	pthread_t self;
}proxyThread;

//Proxy Thread Pool Inits
//	Publishers
proxyThread pub_pool[NUMPROXIES / 2];
pthread_t pub_thread[NUMPROXIES / 2];
//	Subscribers
proxyThread sub_pool[NUMPROXIES / 2];
pthread_t sub_thread[NUMPROXIES / 2];
*/
double DELTA = 15;
int numQueues = 0;
/* ======================== Init Functions ================================*/
void init_proxypool(proxyThread *proxy, int ID) 
{
	proxy->TID = ID;
	proxy->flag = 0;
}

/* ========================= Helper Functions ============================*/
int get_freeThread(proxyThread *pool)
{
	for (int i =  0 ; i < NUMPROXIES / 2 ; i++) {
		if (pool[i].flag == 0) {
			return i;
		}
	}
	return -1;
}

void display_pool(proxyThread *pool)
{
	for (int i = 0 ; i < NUMPROXIES / 2 ; i++) {
		printf("T%d: %d\t",pool[i].TID, pool[i].flag);
	}
}

/* ========================= Main Function ================================*/

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr,"Incorrect num of arguments\n");
		exit(EXIT_FAILURE);
	}

	//Cond Var and Cond Mut init
	pthread_cond_init(&cv, NULL);
	pthread_mutex_init(&cm, NULL);

	// Proxy pool INIT
	for (int i = 0 ; i < NUMPROXIES / 2 ; i++) {
		init_proxypool(&pub_pool[i], i);
		init_proxypool(&sub_pool[i], i);
	}

	//Begin Processing Commands
	size_t bufsize = 100;
	char *buffer = malloc(sizeof(char) * 100);
	FILE *fp = freopen("input.txt", "r+", stdin);
	int size;
	command_line command;

	while (size = getline(&buffer, &bufsize, stdin) >= 0)
	{
		command = str_filler(buffer, " \"");
		if (strcmp(command.command_list[0], "create") == 0) {
			if (strcmp(command.command_list[1], "topic") == 0) {
				int ID = atoi(command.command_list[2]);
				char *name = command.command_list[3];
				int len = atoi(command.command_list[4]);
				printf("Main Thread: Creating Topic: Name: %s - ID: %d\n",name,ID);
				create_topic(ID, name, len);
			}
			else {
				fprintf(stderr, "Error: Unable to create entity %s\n", command.command_list[1]);
			}
		}
		else if (strcmp(command.command_list[0], "add") == 0) {
			if (strcmp(command.command_list[1], "publisher") == 0) {
				int free_t = get_freeThread(pub_pool);
				if (free_t == -1 ) {
					start();
					while (free_t == -1) {
						sched_yield();
						free_t = get_freeThread(pub_pool);
					}
				}
				pub_pool[free_t].flag = 1;
				printf("Main Thread: Adding Pub: %d\n",free_t + 1);
				run_pub(command.command_list[2], free_t);
			}
			else if (strcmp(command.command_list[1], "subscriber") == 0) {
				int free_t = get_freeThread(sub_pool);
				while (free_t == -1) {
					start();
					while (free_t == -1) {
						sched_yield();
						free_t = get_freeThread(sub_pool);
					}
				}
				sub_pool[free_t].flag = 1;
				printf("Main Thread: Adding Sub: %d\n",free_t + 1);
				run_sub(command.command_list[2], free_t);
			}
			else {
				fprintf(stderr, "Error: Unable to add entity %s\n",command.command_list[1]);
			}
		}
		else if (strcmp(command.command_list[0], "delta") == 0) {
			int d = atoi(command.command_list[1]);
			printf("Main Thread: Delta = %d\n",d);
			delta(d);
		}
		else if (strcmp(command.command_list[0], "start") == 0) {
			start();
		}
	}

	//Frees and close functions and Clean up Join
	pthread_join(cl,NULL);
	fclose(fp);
	for (int i = 0 ; i < numQueues ; i++) {
		destroy(&registry[i]);
	}
	free_command_line(&command);
	memset(&command, 0, 0);

/* BEGIN MULTi-THREADING ************ OLD MAIN *****************
	//Cond Var and Cond Mut init
	pthread_cond_init(&cv, NULL);
	pthread_mutex_init(&cm, NULL);
	//Topic Store init
	for (int i = 0 ; i < MAXQUEUES ; i++) {
		char str[8];
		sprintf(str, "Test %d", i);
		init_mutex(&registry[i]);
		init_topicQueue(&registry[i], str, MAXENTRY);
	}
	//Proxy Thread Pools
	//	a. Publishers

	int pos_array1[5] = {1, 0, 2, 3, 0};
	int pos_array2[5] = {1, 2, 3, 0, 1};
	int pos_array3[5] = {2, 1, 3, 0, 2};
	int pos_array4[5] = {0, 1, 2, 3, 3};

	pub_args parg[NUMPROXIES / 2];
	memcpy(parg[0].pos_array, pos_array1, 20);
	memcpy(parg[1].pos_array, pos_array2, 20);
	memcpy(parg[2].pos_array, pos_array3, 20);
	memcpy(parg[3].pos_array, pos_array4, 20);

	topicEntry *e11 = malloc(sizeof(topicEntry));
        topicEntry *e12 = malloc(sizeof(topicEntry));
        topicEntry *e13 = malloc(sizeof(topicEntry));
        topicEntry *e14 = malloc(sizeof(topicEntry));
        topicEntry *e15 = malloc(sizeof(topicEntry));

	topicEntry *e21 = malloc(sizeof(topicEntry));
        topicEntry *e22 = malloc(sizeof(topicEntry));
        topicEntry *e23 = malloc(sizeof(topicEntry));
        topicEntry *e24 = malloc(sizeof(topicEntry));
        topicEntry *e25 = malloc(sizeof(topicEntry));

	topicEntry *e31 = malloc(sizeof(topicEntry));
        topicEntry *e32 = malloc(sizeof(topicEntry));
        topicEntry *e33 = malloc(sizeof(topicEntry));
        topicEntry *e34 = malloc(sizeof(topicEntry));
        topicEntry *e35 = malloc(sizeof(topicEntry));

	topicEntry *e41 = malloc(sizeof(topicEntry));
        topicEntry *e42 = malloc(sizeof(topicEntry));
        topicEntry *e43 = malloc(sizeof(topicEntry));
        topicEntry *e44 = malloc(sizeof(topicEntry));
        topicEntry *e45 = malloc(sizeof(topicEntry));

        topicEntry *entry_array1[5] = {e11, e12, e13, e14, e15};
        topicEntry *entry_array2[5] = {e21, e22, e23, e24, e25};
        topicEntry *entry_array3[5] = {e31, e32, e33, e34, e35};
        topicEntry *entry_array4[5] = {e41, e42, e43, e44, e45};
	parg[0].entry_array = entry_array1;
	parg[1].entry_array = entry_array2;
	parg[2].entry_array = entry_array3;
	parg[3].entry_array = entry_array4;
	
	parg[0].numEntries = 5;
	parg[1].numEntries = 5;
	parg[2].numEntries = 5;
	parg[3].numEntries = 5;

	proxyThread pub_pool[NUMPROXIES / 2];
	pthread_t pub_thread[NUMPROXIES / 2];
	for (int i = 0 ; i < NUMPROXIES / 2 ; i++) {
		init_proxypool(&pub_pool[i]);
		pthread_create(&pub_thread[i],NULL,publisher,&parg[i]);
		pub_pool[i].flag = 1;
	}
	// Clean up Thread + CV broadcast
	sleep(1);
	pthread_cond_broadcast(&cv);
	sleep(16);
	pthread_t cl;
	pthread_create(&cl, NULL, cleanup, NULL);
	//	b. Subscribers
	sub_args sarg[NUMPROXIES / 2];
	sarg[0].lastEntry = 0;
	sarg[0].empty = malloc(sizeof(topicEntry));
	sarg[0].pos = 0;
	sarg[1].lastEntry = 0;
	sarg[1].empty = malloc(sizeof(topicEntry));
	sarg[1].pos = 1;
	sarg[2].lastEntry = 0;
	sarg[2].empty = malloc(sizeof(topicEntry));
	sarg[2].pos = 2;
	sarg[3].lastEntry = 0;
	sarg[3].empty = malloc(sizeof(topicEntry));
	sarg[3].pos = 3;
	proxyThread sub_pool[NUMPROXIES / 2];
	pthread_t sub_thread[NUMPROXIES / 2];
	for (int i = 0 ; i < NUMPROXIES / 2 ; i++) {
		init_proxypool(&sub_pool[i]);
		pthread_create(&sub_thread[i],NULL,subscriber,&sarg[i]);
		sub_pool[i].flag = 1;
	}

	// Clean up Thread + CV broadcast
//	pthread_t cl;
//	pthread_create(&cl, NULL, cleanup, NULL);
//	sleep(1);
//	pthread_cond_broadcast(&cv);
	//Thread Joining + Flag handling
	for (int i = 0 ; i < NUMPROXIES / 2 ; i++) {
		pthread_join(pub_thread[i], NULL);
		pthread_join(sub_thread[i], NULL);
		pub_pool[i].flag = 0;
		sub_pool[i].flag = 0;

	}
	pthread_join(cl,NULL);
	END MULTI-THREADING */
/*
	topicEntry *e1 = malloc(sizeof(topicEntry));
        topicEntry *e2 = malloc(sizeof(topicEntry));
        topicEntry *e3 = malloc(sizeof(topicEntry));
        topicEntry *e4 = malloc(sizeof(topicEntry));
        topicEntry *e5 = malloc(sizeof(topicEntry));
        topicEntry *e6 = malloc(sizeof(topicEntry));
        topicEntry *empty = malloc(sizeof(topicEntry));
        init_topicEntry(e1, 54321);
        init_topicEntry(e2, 2345);
        init_topicEntry(e3, 23452);
        init_topicEntry(e4, 23453);
        init_topicEntry(e5, 23454);
        init_topicEntry(e6, 23455);

        topicEntry *entry_array[6] = {e1, e2, e3, e4, e5, e6};	

	pub_args *pub_arg = malloc(sizeof(pub_args));
        pub_arg->entry_array = entry_array;
	pub_arg->pos_array = malloc(sizeof(int) * 6);
	pub_arg->pos_array[0] = 0;
	pub_arg->pos_array[1] = 0;
	pub_arg->pos_array[2] = 0;
	pub_arg->pos_array[3] = 0;
	pub_arg->pos_array[4] = 0;
	pub_arg->pos_array[5] = 0;
        pub_arg->numEntries = 6;

        cleanup_args *cl_args = malloc(sizeof(cleanup_args));
        cl_args->empty = malloc(sizeof(topicEntry));
	cl_args->queue_i = 0;

        sub_args *sub_arg = malloc(sizeof(sub_args));
        sub_arg->lastEntry = 0;
	sub_arg->pos = 0;
        sub_arg->empty = malloc(sizeof(topicEntry));
        sub_arg->empty->entryNum = 0;

	pthread_t p1, p2, s1, s2, cl1;
        pthread_create(&p1, NULL, publisher, pub_arg);
        pthread_create(&s1, NULL, subscriber, sub_arg);
        pthread_create(&cl1, NULL, cleanup, cl_args);
        pthread_join(p1,NULL);
        pthread_join(s1,NULL);
        pthread_join(cl1,NULL);
	display_Q(&registry[0]);
	*/
	return 0;
}
