/*
 * queue.h
 *
 * Created on: 11/29/2020
 * 	Author: Jarett Nishijo
 */

#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct
{
	int entryNum;
	struct timeval timeStamp;
	int pubID;
	char photoURL[URLSIZE];
	char photoCaption[CAPSIZE];
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
	int *pos_array;
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

int enqueue(int pos, topicEntry *entry);
int dequeue(topicQueue *queue, topicEntry *empty);
int getEntry(int lastEntry, topicEntry *empty, int pos);
void *publisher(void *args);
void *subscriber(void *args);
void *cleanup(void *args);

#endif
