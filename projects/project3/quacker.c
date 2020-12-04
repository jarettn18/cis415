/* =====================  Project 3 ================*/
/*
 * Project 3: Instaquack
 * Author: Jarett Nishijo
 * Date: 11/29/2020
 * 
 * Notes:
 * 	1.
 *
 */

/* ==================== Libraries and definitions ==============*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAXNAME 25
#define URLSIZE 100
#define CAPSIZE 200
#define NUMPROXIES 8
#include "queue.c"

/* ======================== Global Variables and Structs =================*/
pthread_cond_t cv;
pthread_mutex_t cm;

typedef struct {
	int flag;
	int TID;
	pthread_t self;
}proxyThread;

/* ======================== Init Functions ================================*/
void init_proxypool(proxyThread *proxy) 
{
	proxy->flag = 0;
	proxy->TID = pthread_self();
}

/* ========================= Helper Functions ============================*/
int get_freeThread(proxyThread *pool)
{
	for (int i =  0 ; i < NUMPROXIES / 2 ; i++) {
		if (pool[i].flag == 0) {
			return i;
		}
	}
	return -1;
}

/* ========================= Main Function ================================*/

int main(int argc, char *argv[])
{
	//Topic Store init
	for (int i = 0 ; i < MAXQUEUES ; i++) {
		char str[8];
		sprintf(str, "Test %d", i);
		init_mutex(&registry[i]);
		init_topicQueue(&registry[i], str);
	}
	//Proxy Thread Pools
	//	a. Publishers
	topicEntry *array[NUMPROXIES / 2][5];

	pub_args parg[NUMPROXIES / 2];

	for (int i = 0 ; i < NUMPROXIES / 2 ; i++) {
		for (int j = 0 ; j < 5 ; j++) {
			init_topicEntry(array[i][j],123);
		}
		parg[i].entry_array = array[i];
		//TODO POSITION ARRAYS U STUPID BITCH
		parg[i].numEntries = 4;
	}
	proxyThread pool[NUMPROXIES / 2];
	pthread_t thread[NUMPROXIES / 2];
	for (int i = 0 ; i < NUMPROXIES / 2 ; i++) {
		init_proxypool(&pool[i]);
		pthread_create(&thread[i],NULL,publisher,&parg[i]);
	}
	//	b. Subscribers
	//
	




	/*
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
	pub_arg->pos_array = malloc(sizeof(int) * 6);
	pub_arg->pos_array[0] = 0;
	pub_arg->pos_array[1] = 0;
	pub_arg->pos_array[2] = 0;
	pub_arg->pos_array[3] = 0;
	pub_arg->pos_array[4] = 0;
	pub_arg->pos_array[5] = 0;
        pub_arg->numEntries = 6;

        cleanup_args *cl_args = malloc(sizeof(cleanup_args));
        cl_args->empty = malloc(sizeof(topicEntry));
	cl_args->queue_i = 0;

        sub_args *sub_arg = malloc(sizeof(sub_args));
        sub_arg->lastEntry = 0;
	sub_arg->pos = 0;
        sub_arg->empty = malloc(sizeof(topicEntry));
        sub_arg->empty->entryNum = 0;

	pthread_t p1, p2, s1, s2, cl1;
        pthread_create(&p1, NULL, publisher, pub_arg);
        pthread_create(&s1, NULL, subscriber, sub_arg);
        pthread_create(&cl1, NULL, cleanup, cl_args);
        pthread_join(p1,NULL);
        pthread_join(s1,NULL);
        pthread_join(cl1,NULL);
	display_Q(&registry[0]);
	*/
	return 0;
}
