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
#include "quacker.h"
#include "command.h"
#include "string_parser.h"

/* ================ Definitions and Global Variables ==============*/
//#define MAXENTRY 4
//#define DELTA 15
//#define MAXQUEUES 4
double DELTA = 10;
int numQueues = 0;
//topicQueue registry[MAXQUEUES];

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

void init_topicQueue(topicQueue *queue, char *name, int len)
{
	strcpy(queue->name, name);
	queue->buffer = malloc(sizeof(topicEntry *) * len);
	queue->head = 0;
	queue->tail = 0;
	queue->length = 0;
	queue->count = 0;
	queue->MAXLENGTH = len;
}

void display_Q(topicQueue *queue)
{
	if (queue->length == 0) {
		printf("QUEUE: %s\n",queue->name);
		printf("\tQUEUE is empty\n");
	}
	else {
		printf("QUEUE: %s\n",queue->name);
		for (int i = 0 ; i < queue->length ; i++) {
			printf("\tTicket %d: %d\n",queue->buffer[i]->entryNum,queue->buffer[i]->pubID);
		}
		printf(" TAIL: %d HEAD: %d LENGTH: %d\n",queue->tail, queue->head, queue->length);
	}
}

int all_empty()
{
	int empty = 1;
	for (int i = 0 ; i < numQueues ; i++) {
		if (registry[i].length > 0) {
			empty = 0;
		}
	}
	return empty;
}

void destroy(topicQueue *queue)
{
	for (int i = 0 ; i < queue->MAXLENGTH ; i++) {
		free(queue->buffer[i]);
	}
	free(queue->buffer);
}

/* ====================== Pubs and Subs ======================*/

void *publisher(void *args)
{
	pthread_cond_wait(&cv, &cm);
	pthread_mutex_unlock(&cm);

	pub_args *pub_args = args;

 	char *p_buf = malloc(sizeof(char) * 50);;
	char *sp = p_buf;
        size_t p_bufsize = 50;
        int size;
        FILE *pub_fp = fopen(pub_args->file, "r");
        command_line p_cmd;
        if (pub_fp == NULL) {
                printf("TID %d: File %s is not valid\n",pub_args->TID, pub_args->file);
		free_command_line(&p_cmd);
		memset(&p_cmd, 0, 0);
                free(sp);
        }
	else{
		while (size = getline(&p_buf, &p_bufsize, pub_fp) >= 0)
		{
			int stop = strncmp(p_buf, "stop", 4);
			if (stop == 0) {
				printf("TID %d: Stop Read! Exiting\n",pub_args->TID);
				pub_pool[pub_args->TID].flag = 0;
				free(sp);
				fclose(pub_fp);
				pthread_exit(0);
				free_command_line(&p_cmd);
				memset(&p_cmd, 0, 0);
				break;
			}
			//printf("%s",p_buf);
			p_cmd = str_filler(p_buf, " ");
			if (strcmp(p_cmd.command_list[0], "put") == 0)
			{	
				fprintf(stdout,"TID %d: pushing\n",pub_args->TID);
				topicEntry *entry = malloc(sizeof(topicEntry));
				entry->pubID = pub_args->TID;
				entry->entryNum = 0;
				strcpy(entry->photoURL, p_cmd.command_list[2]);
				strcpy(entry->photoCaption, p_cmd.command_list[3]);
				int enq_succ = enqueue(atoi(p_cmd.command_list[1]) - 1, entry);
				if (enq_succ == 0) {
					printf("Queue full: Yielding\n");
					while (enq_succ == 0) {
						sched_yield();
						enq_succ = enqueue(atoi(p_cmd.command_list[1]) - 1, entry);
					}
				}
			}
			else if (strcmp(p_cmd.command_list[0], "sleep") == 0)
			{
				t_sleep(atoi(p_cmd.command_list[1]));
			}
			else {
				printf("Command %s not found\n",p_cmd.command_list[0]);
			}
		}
	}
	/*
	for (int i = 0 ; i < pub_args->numEntries ; i++) {
		fprintf(stdout,"\nTID %lu: pushing\n",pthread_self());
		int enq_success = enqueue(pub_args->pos_array[i], pub_args->entry_array[i]);
		if (enq_success == 0) {
			printf("Queue full: Yielding\n");
			while (enq_success == 0) {
				sched_yield();
				enq_success = enqueue(pub_args->pos_array[i],pub_args->entry_array[i]);
			}
		}
	}
	printf("TID %lu: Done\n",pthread_self());
	*/
}

void *subscriber(void *args)
{
	//pthread_cond_wait(&cv, &cm);
	//pthread_mutex_unlock(&cm);
	sub_args *sub_args = args;
	int get_entry = 1;
	while (get_entry != 0) {
		get_entry = getEntry(sub_args->lastEntry, sub_args->empty, sub_args->pos);
		if (get_entry == 0) {
			printf("Subscriber: Waiting for more entries\n");
			sleep(3);
		}
		else if (get_entry == 1){
			printf("TID %lu: Read Entry %d\n",pthread_self(), sub_args->empty->entryNum);
			sub_args->lastEntry++;
			sleep(1);
		}
		else {
			printf("TID %lu: Read Entry %d\n",pthread_self(), sub_args->empty->entryNum);
			sub_args->lastEntry = get_entry;
			sleep(1);
		}
	}
}

void *cleanup(void *args)
{
	//cleanup_args *cl_args = args;
	struct timeval current_time;
	int empty = 0;
        while(empty != 1) {
	int old_threads = 1;
	while (old_threads == 1) {
		old_threads = 0;
		for (int i = 0 ; i < numQueues ; i++) {
			double total_elapsed = 0;
			if (registry[i].length > 0 ) {
				gettimeofday(&current_time,NULL);
				double elapsed_sec = (current_time.tv_sec - registry[i].buffer[registry[i].tail]->timeStamp.tv_sec) * 1000;
				double elapsed_usec = (current_time.tv_usec - registry[i].buffer[registry[i].tail]->timeStamp.tv_usec) / 1000;
				total_elapsed = elapsed_sec + elapsed_usec;
			}
			if ((total_elapsed/1000) > DELTA) {
				old_threads = 1;
				printf("Queue %s: Oldest entry %d is %.2f sec\n",registry[i].name,registry[i].buffer[registry[i].tail]->entryNum,total_elapsed/1000);
				dequeue(&registry[i],NULL); //cl_args->empty);
			}
		}
	}
	empty = all_empty();
	}
	printf("EXITING CLEAN UP THREAD\n");
	pthread_exit(0);
}

/* ====================== Main Functions ===================== */

int enqueue(int pos, topicEntry *entry)
{
	int aquire = pthread_mutex_lock(&registry[pos].mutex);
	
	if (registry[pos].length == registry[pos].MAXLENGTH) 
	{
		//fprintf(stderr, "Error: Queue %s is full\n",queue->name);
		pthread_mutex_unlock(&registry[pos].mutex);
		return 0;
	}
	else {
		fprintf(stdout, "Pushing entry %d to queue %s\n",registry[pos].count+1, registry[pos].name);
		registry[pos].length++;
		registry[pos].count++;
		registry[pos].buffer[registry[pos].head] = entry;
		entry->entryNum = registry[pos].count;
		gettimeofday(&entry->timeStamp, NULL);
		registry[pos].head++;
		if (registry[pos].head == registry[pos].MAXLENGTH) {
			registry[pos].head = 0;
		}
		pthread_mutex_unlock(&registry[pos].mutex);
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
		/*
		empty->entryNum = queue->buffer[queue->tail]->entryNum;
		empty->pubID = queue->buffer[queue->tail]->pubID;
		*/
		queue->buffer[queue->tail]->pubID = 0;
		queue->buffer[queue->tail]->entryNum = -1;
		queue->tail++;
		if (queue->tail == queue->MAXLENGTH)
		{
			queue->tail = 0;
		}
		pthread_mutex_unlock(&queue->mutex);
		return 1;
	}
}

int getEntry(int lastEntry, topicEntry *empty, int pos)
{
	int aquire = pthread_mutex_lock(&registry[pos].mutex);
	int lt = 1;
	//Case 1 empty queue
	if (registry[pos].length == 0) {
		pthread_mutex_unlock(&registry[pos].mutex);
		return 0;
	}
	//Case 3b first entry has number >= last entry + 1
	if (registry[pos].buffer[registry[pos].tail]->entryNum >= (lastEntry + 1)) 
	{
		empty->entryNum = registry[pos].buffer[registry[pos].tail]->entryNum;
		empty->pubID = registry[pos].buffer[registry[pos].tail]->pubID;
		pthread_mutex_unlock(&registry[pos].mutex);
		return empty->entryNum;
	}
	//Case 2 last entry + 1 in queue
	for (int i = 0 ; i < registry[pos].length ; i++) {
		int current_i = registry[pos].tail + i;
		if (current_i >= registry[pos].MAXLENGTH) {
			current_i = current_i - registry[pos].MAXLENGTH;
		}
		if (registry[pos].buffer[current_i]->entryNum == lastEntry + 1) {
			empty->entryNum = registry[pos].buffer[current_i]->entryNum;
			empty->pubID = registry[pos].buffer[current_i]->pubID;
			pthread_mutex_unlock(&registry[pos].mutex);
			return 1;
		}
		if (registry[pos].buffer[current_i]->entryNum > lastEntry + 1) {
			lt = 0;
		}
	}
	//Case 3 last entry + 1 is not in queue
	//	a. all entries are <= last entry + 1
	if (lt == 1) {
		pthread_mutex_unlock(&registry[pos].mutex);
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
