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
#define MAXENTRY 15
#define MAXQUEUES 6

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
	int MAXLENGTH;
	pthread_mutex_t mutex;
}topicQueue;

typedef struct
{
	topicEntry *entry;
	char file[MAXNAME];
	int pos;
	int TID;
}pub_args;

typedef struct
{	
	topicEntry *empty;
	char file[MAXNAME];
	int lastEntry;
	int TID;
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
void destroy(topicQueue *queue);
int enqueue(int pos, topicEntry *entry);
int dequeue(topicQueue *queue, topicEntry *empty);
int getEntry(int lastEntry, topicEntry *empty, int pos);
void *publisher(void *args);
void *subscriber(void *args);
void *cleanup(void *args);

topicQueue registry[MAXQUEUES];
#endif
