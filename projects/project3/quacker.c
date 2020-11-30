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
#define MAXQUEUES 4

/* ======================== Global Variables and Structs =================*/
struct topicQueue;
struct topicQueue *registry[MAXQUEUES];
pthread_cond_t cv;
pthread_mutex_t cm;

typedef struct {
	int entryNum;
	struct timeval timeStamp;
	int pubID;
	char photoURL[URLSIZE]; //URL to photo
	char photoCaption[CAPSIZE]; //photo caption
} topicEntry;

typedef struct {
	char name[MAXNAME];
	topicEntry **buffer;
	int head;
	int tail;
	int length;
	pthread_mutex_t mutex;
} topicQueue;

int main(int argc, char *argv[])
{
	return 0;
}
