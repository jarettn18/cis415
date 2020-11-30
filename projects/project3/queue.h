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
	pthread_mutex_t mutex;
}topicQueue;

int enqueue(topicQueue * queue, topicEntry *entry);
int dequeue(topicEntry *empty_entry);
int getEntry(int lastEntry, topicEntry *empty_entry);

#endif
