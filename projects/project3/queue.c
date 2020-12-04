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
#define MAXQUEUES 4
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
	queue->count = 0;
}

void display_Q(topicQueue *queue)
{
	if (queue->length == 0) {
		printf("QUEUE is empty\n");
	}
	else {
		printf("QUEUE: %s\n",queue->name);
		for (int i = 0 ; i < MAXENTRY ; i++) {
			printf("\tTicket %d: %d\n",queue->buffer[i]->entryNum,queue->buffer[i]->pubID);
		}
		printf(" TAIL: %d HEAD: %d LENGTH: %d\n",queue->tail, queue->head, queue->length);
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
			printf("Queue full: Yielding\n");
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
	int i = 1;
	while (i != 10) {
		int get_entry = getEntry(sub_args->lastEntry, sub_args->empty);
		if (get_entry == 0) {
			printf("sleeping %d\n",i);
			sleep(3);
		}
		else if (get_entry == 1){
			printf("TID %lu: Read Entry %d last entry %d\n",pthread_self(), sub_args->empty->entryNum, sub_args->lastEntry);
			sub_args->lastEntry++;
			sleep(1);
		}
		else {
			printf("TID %lu: Read Entry %d last entry %d\n",pthread_self(), sub_args->empty->entryNum, sub_args->lastEntry);
			sub_args->lastEntry = get_entry;
			sleep(1);
		}
		i++;
	}
}

void *cleanup(void *args)
{
	cleanup_args *cl_args = args;
	int deq_succ = 1;
	while(deq_succ == 1) {
		sleep(3);
		deq_succ = dequeue(&q, cl_args->empty);
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
		fprintf(stdout, "\nPushing entry %d to queue %s\n",queue->count+1, queue->name);
		queue->length++;
		queue->count++;
		queue->buffer[queue->head] = entry;
		entry->entryNum = queue->count;
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
		queue->length--;
		empty->entryNum = queue->buffer[queue->tail]->entryNum;
		empty->pubID = queue->buffer[queue->tail]->pubID;
		queue->buffer[queue->tail]->pubID = 0;
		queue->buffer[queue->tail]->entryNum = -1;
		queue->tail++;
		if (queue->tail == MAXENTRY)
		{
			queue->tail = 0;
		}
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
		empty->entryNum = q.buffer[q.tail]->entryNum;
		empty->pubID = q.buffer[q.tail]->pubID;
		pthread_mutex_unlock(&q.mutex);
		return empty->entryNum;
	}
	//Case 2 last entry + 1 in queue
	for (int i = 0 ; i < q.length ; i++) {
		int current_i = q.tail + i;
		if (current_i >= MAXENTRY) {
			current_i = current_i - MAXENTRY;
		}
		if (q.buffer[current_i]->entryNum == lastEntry + 1) {
			empty->entryNum = q.buffer[current_i]->entryNum;
			empty->pubID = q.buffer[current_i]->pubID;
			pthread_mutex_unlock(&q.mutex);
			return 1;
		}
		if (q.buffer[current_i]->entryNum > lastEntry + 1) {
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
/*
int main()
{
	init_mutex(&q);
	init_topicQueue(&q, "Test1");

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
	pub_arg->numEntries = 4;

	cleanup_args *cl_args = malloc(sizeof(cleanup_args));
	cl_args->empty = malloc(sizeof(topicEntry));
	sub_args *sub_arg = malloc(sizeof(sub_args));
	sub_arg->lastEntry = 0;
	sub_arg->empty = malloc(sizeof(topicEntry));
	sub_arg->empty->entryNum = 0;

	pthread_t p1, p2, s1, s2, cl1;
	pthread_create(&p1, NULL, publisher, pub_arg);
	pthread_create(&p2, NULL, publisher, pub_arg);
	pthread_create(&s1, NULL, subscriber, sub_arg);
	pthread_create(&s2, NULL, subscriber, sub_arg);
	pthread_create(&cl1, NULL, cleanup, cl_args);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(s1,NULL);
	pthread_join(s2,NULL);
	pthread_join(cl1,NULL);
	
	//dequeue(&q, empty);
	//enqueue(&q, e5);
	display_Q(&q);

	destroy(&q);
	free(pub_arg);
	free(sub_arg->empty);
	free(cl_args->empty);
	free(sub_arg);
	free(cl_args);
	free(e1);
	free(e2);
	free(e3);
	free(e4);
	free(e5);
	free(e6);
	free(empty);
}
*/
