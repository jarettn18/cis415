/*
 * command.c
 *
 * Created on 12/4/2020
 * 	Author: Jarett Nishijo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quacker.c"
#include "queue.c"
#include "string_parser.c"

/* ======================= Definitions and Global Variables ===============*/
/* ======================= init/free + helper functions ===================*/
/* ========================= Command Implementation ======================*/
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
		init_mutex(&registry[ID-1]->mutex);
		init_topicQueue(&registry[ID-1], name, length);
		numQueues++;
		return 1;
	}
}

void *query(char *type)
{
	for (int i = 0 ; i < numQueues ; i++) {
		fprintf(stdout, "Topic Queue ID: %d || Length: %d\n",i,registry[i]->length);
	}
}

int add_pub(void *cmd_file)
{
}

int add_sub(void *cmd_file)
{
}

int delta(int d)
{
	DELTA = d;
	return 1;
}

void *start()
{
}
