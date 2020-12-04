/*
 * queue.h
 *
 * Created on: 11/29/2020
 * 	Author: Jarett Nishijo
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define URLSIZE 100
#define CAPSIZE 250
#define MAXNAME 25

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
	int TID;
	int numEntries;
}pub_args;

typedef struct
{	
	int lastEntry;
	topicEntry *empty;
}sub_args;

typedef struct
{
	topicEntry *empty;
}cleanup_args;

int enqueue(topicQueue *queue, topicEntry *entry);
int dequeue(topicQueue *queue, topicEntry *empty);
int getEntry(int lastEntry, topicEntry *empty);
void *publisher(void *args);
void *subscriber(void *args);
void *cleanup(void *args);

#endif
