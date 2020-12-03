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
#include <sys/time.h>
#include "queue.h"

/* ================ Definitions and Global Variables ==============*/
#define MAXENTRY 4
#define DELTA 4
topicQueue q;


/* ================= init/free and helper functions ===============*/
void init_mutex(topicQueue *queue)
{
	int success = pthread_mutex_init(&queue->mutex, NULL);
	if (success != 0) {
		fprintf(stderr, "Error: MUTEX NOT INITIALIZED\n");
		exit(EXIT_FAILURE);
	}
}

void init_topicEntry(topicEntry *entry, int pubID)
	//TODO Add args URL and CAP add to init function
{
	entry->entryNum = 0;
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
			printf("\tTicket %d: %d\n",queue->buffer[i]->entryNum,queue->buffer[i]->pubID);
		}
	}
}

void destroy(topicQueue *queue)
{
	free(queue->buffer);
}

/* ====================== Pubs and Subs ======================*/

void *publisher(void *args)
{
	pub_args *pub_args = args;
	for (int i = 0 ; i < pub_args->numEntries ; i++) {
		fprintf(stdout,"TID %lu: pushing\n",pthread_self());
		int enq_success = enqueue(&q, pub_args->entry_array[i]);
		if (enq_success == 0) {
			while (enq_success == 0) {
				sched_yield();
				enq_success = enqueue(&q, pub_args->entry_array[i]);
			}
		}
	}
		
}

void *subscriber(void *args)
{
	sub_args *sub_args = args;
	while (1) {
		int get_entry = getEntry(sub_args->lastEntry, sub_args->empty);
		if (get_entry == 0) {
			sleep(3);
		}
		else {
			printf("TID %lu: Read Entry %d\n",pthread_self(), sub_args->lastEntry);
			sub_args->lastEntry++;
			sleep(1);
		}
	}
}

void *cleanup(void *args)
{
	cleanup_args *cl_args = args;
	while(1) {
		sleep(5);
		dequeue(&q, cl_args->empty);
	}
}

/* ====================== Main Functions ===================== */

int enqueue(topicQueue *queue, topicEntry *entry)
{
	int aquire = pthread_mutex_lock(&queue->mutex);

	if (queue->length == MAXENTRY) 
	{
		//fprintf(stderr, "Error: Queue %s is full\n",queue->name);
		pthread_mutex_unlock(&queue->mutex);
		return 0;
	}
	else {
		fprintf(stdout, "Pushing entry %d to queue %s\n",queue->length+1, queue->name);
		queue->length++;
		queue->buffer[queue->head] = entry;
		entry->entryNum = queue->length;
		gettimeofday(&entry->timeStamp, NULL);
		queue->head++;
		if (queue->head == MAXENTRY) {
			queue->head = 0;
		}
		pthread_mutex_unlock(&queue->mutex);
		return 1;
	}
}

int dequeue(topicQueue *queue, topicEntry *empty)
{
	int aquire = pthread_mutex_lock(&queue->mutex);

	if (queue->length == 0)
	{
		fprintf(stderr, "Error: Queue %s is empty\n", queue->name);
		pthread_mutex_unlock(&queue->mutex);
		return 0;
	}
	else {
		fprintf(stdout, "Dequeueing from queue %s\n",queue->name);
		empty->entryNum = queue->buffer[queue->tail]->entryNum;
		empty->pubID = queue->buffer[queue->tail]->pubID;
		queue->buffer[queue->tail]->pubID = 0;
		queue->buffer[queue->tail]->entryNum = 9999;
		queue->tail++;
		if (queue->tail == MAXENTRY)
		{
			queue->tail = 0;
		}
		queue->length--;
		pthread_mutex_unlock(&queue->mutex);
		return 1;
	}
}

int getEntry(int lastEntry, topicEntry *empty)
{
	int aquire = pthread_mutex_lock(&q.mutex);
	int lt = 1;

	//Case 1 empty queue
	if (q.length == 0) {
		pthread_mutex_unlock(&q.mutex);
		return 0;
	}
	//Case 3b first entry has number >= last entry + 1
	if (q.buffer[q.tail]->entryNum >= (lastEntry + 1)) 
	{
		empty = q.buffer[q.tail];
		pthread_mutex_unlock(&q.mutex);
		return empty->entryNum;
	}
	//Case 2 last entry + 1 in queue
	for (int i = 0 ; i < q.length ; i++) {
		int current_i =  q.tail + i;
		if (current_i > MAXENTRY) {
			current_i = 0;
		}
		if (q.buffer[current_i]->entryNum == (lastEntry + 1)) {
			empty = q.buffer[current_i];
			pthread_mutex_unlock(&q.mutex);
			return 1;
		}
		if (q.buffer[current_i]->entryNum > (lastEntry + 1)) {
			lt = 0;
		}
	}
	//Case 3 last entry + 1 is not in queue
	//	a. all entries are <= last entry + 1
	if (lt == 1) {
		pthread_mutex_unlock(&q.mutex);
		return 0;
	}
}

int main()
{
	init_mutex(&q);
	init_topicQueue(&q, "Test");

	topicEntry *e1 = malloc(sizeof(topicEntry));
	topicEntry *e2 = malloc(sizeof(topicEntry));
	topicEntry *e3 = malloc(sizeof(topicEntry));
	topicEntry *e4 = malloc(sizeof(topicEntry));
	topicEntry *e5 = malloc(sizeof(topicEntry));
	topicEntry *empty = malloc(sizeof(topicEntry));
	init_topicEntry(e1, 54321);
	init_topicEntry(e2, 2345);
	init_topicEntry(e3, 23452);
	init_topicEntry(e4, 23453);
	init_topicEntry(e5, 23454);

	topicEntry *entry_array[5] = {e1, e2, e3, e4, e5};
	pub_args *pub_arg = malloc(sizeof(pub_args));
	pub_arg->entry_array = entry_array;
	pub_arg->numEntries = 5;

	cleanup_args *cl_args = malloc(sizeof(cleanup_args));
	cl_args->empty = malloc(sizeof(topicEntry));
	sub_args *sub_arg = malloc(sizeof(sub_args));
	sub_arg->lastEntry = 0;
	sub_arg->empty = malloc(sizeof(topicEntry));

	pthread_t p1, s1, cl1;
	pthread_create(&p1, NULL, publisher, pub_arg);
	pthread_create(&s1, NULL, subscriber, sub_arg);
	pthread_create(&cl1, NULL, cleanup, cl_args);
	pthread_join(p1,NULL);
	pthread_join(s1,NULL);
	pthread_join(cl1,NULL);
	
	//dequeue(q, empty);

	//display_Q(&q);

	destroy(&q);
	free(pub_arg);
	free(e1);
	free(e2);
	free(e3);
	free(e4);
	free(e5);
	free(empty);
}
