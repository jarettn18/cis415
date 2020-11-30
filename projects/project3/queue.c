/*
 * queue.c
 *
 * Created on: 11/29/2020
 * 	Author: Jarett Nishijo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "queue.h"

#define MAXENTRY 10

/* ================= init/free and helper functions ===============*/
void init_mutex(topicQueue *queue)
{
	int success = pthread_mutex_init(&queue->mutex, NULL);
	if (success != 0) {
		fprintf(stderr, "Error: MUTEX NOT INITIALIZED\n");
		exit(EXIT_FAILURE);
	}
}

void init_topicEntry(topicEntry *entry, int num, int pubID)
	//TODO Add args URL and CAP add to init function
{
	entry->entryNum = num;
	entry->pubID = pubID;
}

void init_topicQueue(topicQueue *queue, char *name)
{
	strcpy(queue->name, name);
	queue->buffer = malloc(sizeof(topicEntry *) * MAXENTRY);
	queue->head = 0;
	queue->tail = 0;
	queue->length = 0;
}

void display_Q(topicQueue *queue)
{
	if (queue->length == 0) {
		printf("QUEUE is empty\n");
	}
	else {
		printf("QUEUE: %s\n",queue->name);
		for (int i = 0 ; i < queue->length ; i++) {
			printf("\tTicket %d: %d\n",i,queue->buffer[i]->pubID);
		}
	}
}

void destroy(topicQueue *queue)
{
	free(queue->buffer);
	free(queue);
}

/* ====================== Main Functions ===================== */

int enqueue(topicQueue *queue, topicEntry *entry)
{
	//TODO write enqueue
}

int main()
{
	topicQueue *q = malloc(sizeof(topicQueue));
	init_topicQueue(q, "Test");

	topicEntry *e1 = malloc(sizeof(topicEntry));
	topicEntry *e2 = malloc(sizeof(topicEntry));
	init_topicEntry(e1, 3, 54321);
	init_topicEntry(e2, 4, 2345);

	q->buffer[0] = e1;
	q->buffer[1] = e2;
	q->length = 2;
	display_Q(q);

	destroy(q);
	free(e1);
	free(e2);
}
