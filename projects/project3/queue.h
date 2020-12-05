/*
 * queue.h
 *
 * Created on: 11/29/2020
 * 	Author: Jarett Nishijo
 */

#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#define URLSIZE 100
#define CAPSIZE 250
#define MAXNAME 25
#define MAXENTRY 10
#define MAXQUEUES 4
int DELTA;
int numQueues;
pthread_cond_t cv;
pthread_mutex_t cm;

typedef struct
{
	int entryNum;
	struct timeval timeStamp;
	int pubID;
	char photoURL[URLSIZE];
	char *photoCaption[CAPSIZE];
}topicEntry;

typedef struct 
{
	char name[MAXNAME];
	topicEntry **buffer;
	int head;
	int tail;
	int length;
	int count;
	pthread_mutex_t mutex;
}topicQueue;

typedef struct
{
	topicEntry **entry_array;
	int pos_array[MAXENTRY];
	int TID;
	int numEntries;
}pub_args;

typedef struct
{	
	int lastEntry;
	topicEntry *empty;
	int pos;
}sub_args;

typedef struct
{
	topicEntry *empty;
	int queue_i;
}cleanup_args;

void init_mutex(topicQueue *queue);
void init_topicEntry(topicEntry *entry, int pubID);
void init_topicQueue(topicQueue *queue, char *name, int len);
void display_Q(topicQueue *queue);
int enqueue(int pos, topicEntry *entry);
int dequeue(topicQueue *queue, topicEntry *empty);
int getEntry(int lastEntry, topicEntry *empty, int pos);
void *publisher(void *args);
void *subscriber(void *args);
void *cleanup(void *args);

topicQueue registry[MAXQUEUES];
#endif
