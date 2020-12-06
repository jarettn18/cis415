/*
 * command.c
 *
 * Created on 12/4/2020
 * 	Author: Jarett Nishijo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "command.h"
#include "queue.h"
#include "quacker.h"
/* ======================= Definitions and Global Variables ===============*/
int cl_running = 0;
/* ======================= init/free + helper functions ===================*/
/* ====================== Pub/Sub Command Implementation =====================*/
int put(int ID, char *URL, char *caption)
{
	return 1;
}

int get(int ID)
{
	return 1;
}

void *t_sleep(int ms)
{

	sleep((ms/1000));
}

void *stop()
{
	printf("Stopping program\n");
}

/* ========================= Command Line Implementation =====================*/
int create_topic(int ID, char *name, int length)
{
	if (ID >= MAXQUEUES){
		fprintf(stderr,"ID %d: is not allowed\n", ID);
		return 0;
	}
	else if (length > MAXENTRY) {
		fprintf(stderr, "Unable to allocate %d entries\n",length);
		return 0;
	}
	else {
		init_mutex(&registry[ID-1]);
		init_topicQueue(&registry[ID-1], name, length);
		numQueues++;
		return 1;
	}
}

void *query(char *type)
{
	for (int i = 0 ; i < numQueues ; i++) {
		fprintf(stdout, "Topic Queue ID: %d || Length: %d\n",i,registry[i].length);
	}
}

int run_pub(char *cmd_file, int ID)
{
	pub_args *parg = malloc(sizeof(pub_args));
	strcpy(parg->file, cmd_file);
	printf("In run_pub. running cmd file\n");
	parg->TID = ID;
	pthread_create(&pub_thread[ID], NULL, publisher, parg);
	//pthread_join(pub_thread[ID], NULL);
	free(parg);
	return 1;
}

int run_sub(char *cmd_file, int ID)
{
	printf("In run_sub. running cmd file\n");
	sub_pool[ID].flag = 0;
	return 1;
}

int delta(int d)
{
	DELTA = d;
	return 1;
}

void *start()
{
	printf("START CALLED\n");
	sleep(1);
	pthread_cond_broadcast(&cv);
	//TODO FIX DELTA
	if (cl_running == 0) {
		printf("CLEANUP THREAD LAUNCHED\n");
        	pthread_create(&cl, NULL, cleanup, NULL);
		cl_running = 1;
	}
        //pthread_cond_broadcast(&cv);
       	pthread_join(pub_thread[0], NULL);
        pthread_join(pub_thread[1], NULL);
        pthread_join(pub_thread[2], NULL);
        pthread_join(pub_thread[3], NULL);
	pthread_join(cl, NULL);
}
