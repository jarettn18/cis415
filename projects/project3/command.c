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
void *t_sleep(int ms)
{

	sleep((ms/1000));
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
	parg->TID = ID + 1;
	pthread_create(&pub_thread[ID], NULL, publisher, parg);
	return 1;
}

int run_sub(char *cmd_file, int ID)
{
	sub_args *sarg = malloc(sizeof(sub_args));
	strcpy(sarg->file, cmd_file);
	sarg->TID = ID + 1;
	pthread_create(&sub_thread[ID], NULL, subscriber, sarg);
	return 1;
}

int delta(int d)
{
	DELTA = d;
	return 1;
}

void *start()
{
	sleep(2);
	pthread_cond_broadcast(&cv);
	if (cl_running == 0) {
        	pthread_create(&cl, NULL, cleanup, NULL);
		cl_running = 1;
	}
        //pthread_cond_broadcast(&cv);
	for (int i = 0 ; i < NUMPROXIES / 2 ; i++) {
		pthread_join(pub_thread[i],NULL);
		pthread_join(sub_thread[i],NULL);
	}
}
