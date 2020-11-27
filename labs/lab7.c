/*=============================================================================
 * Program Name: lab7
 * Author: Jared Hall
 * Edited by Grayson Guan
 * Date: 11/17/2020
 * Description:
 *     A simple program that implements a thread-safe queue of meal tickets
 *
 *===========================================================================*/

//========================== Preprocessor Directives ==========================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
//=============================================================================

//================================= Constants =================================
#define MAXNAME 15
#define MAXQUEUES 4
#define MAXTICKETS 3
#define MAXDISH 20
#define MAXPUBs 3
#define MAXSUBS 4
//=============================================================================

//============================ Structs and Macros =============================
typedef struct mealTicket{
	int ticketNum;
	char *dish;
} mealTicket;

//TODO: Declare a mutex in the struct. (e.g. add pthread_mutex_t ...)
typedef struct MTQ {
	char name[MAXNAME];
	struct mealTicket *buffer;
	int head;
	int tail;
	int max_length;
	int length;
	int ticket;
} MTQ;

//TODO: Add a mutex init to this function (e.g registry[pos].mutex = PTHREAD_MUTEX_INITIALIZER)
//TODO: Add a conditional mutex, use pthread_cond_wait and pthread_cond_broadcast to make all threads hold until they are all ready to start together.

//A good example link is posted here
//https://www.geeksforgeeks.org/condition-wait-signal-multi-threading/

//It will be really helpeful for you to add a print entry function for debug and demonstration purpose.
void init(int pos, char *MTQ_ID) {
    //....................
    //....................
    //....................
    //....................
}

void freeMTQ(int pos, char *MTQ_ID) {
	//....................
}
	

MTQ registry[MAXQUEUES]; //INFO: Changed to be an array of structs
//=============================================================================

//================================= Functions =================================
int enqueue(char *MTQ_ID, mealTicket *MT) {
	//Step-1: Find registry

	//STEP-2: 
		//TODO: Aquire the lock if it's available. Otherwise, wait until it is.
		//HINT: Use conditional variables and sched_yield.
	//SETP-3: enqueue
        //....................
        //....................
	//TODO: Release the lock.
}

int dequeue(char *MTQ_ID, int ticketNum, mealTicket *MT) {
	//Step-1: Find registry

	//Step-2:
		//TODO: Aquire the lock if it's available. Otherwise, wait until it is.
		//HINT: Use conditional variables and sched_yield.
	//Setp-3: dequeue
	    //....................
	    //....................
	//Step-4: 
		//TODO: Release the lock.
	}
	return ret;
}

void *publisher(void *args) {
	/* TODO: The publisher will receive the following in the struct args:
	*        1. An array of mealTickets to push to the queue.
	*        2. For each meal ticket, an MTQ_ID. (Note: a publisher can push to multiple queues)
	*        3. The thread ID
	* The publisher will do the pthread_cond_wait procedure, and wait for a cond signal to begin its work
	* The publisher will then print its type and thread ID on startup. Then it will push one meal ticket at a time to
	* its appropriate queue before sleeping for 1 second. It will do this until there are no more meal tickets to push.
	*/
}

void *subscriber(void *args) {
	/* TODO:The subscriber will take the following:
	*       1. The MTQ_ID it will pull from.
	*       2. The thread ID
	*       3. An empty meal ticket.
	* The subscriber will do the pthread_cond_wait procedure, and wait for a cond signal to begin its work
	* The subscriber will print its type and thread ID on startup. Then it will pull a ticket from its queue
	* and print it. If the queue is empty then it will print an empty message along with its
	* thread ID and wait for 1 second. If the thread is not empty then it will pop a ticket and 
	* print it along with the thread id.
	*/
}
//=============================================================================

//=============================== Program Main ================================
int main(int argc, char argv[]) {
	//Variables Declarations
	char *qNames[] = {"Breakfast", "Lunch", "Dinner", "Bar"};
	char *bFood[] = {"Eggs", "Bacon", "Steak"};
	char *lFood[] = {"Burger", "Fries", "Coke"};
	char *dFood[] = {"Steak", "Greens", "Pie"};
	char *brFood[] = {"Whiskey", "Sake", "Wine"};
	int i, j, t = 1;
	int test[4];
	char dsh[] = "Empty";
	mealTicket bfast[3] = {[0].dish = bFood[0], [1].dish = bFood[1], [2].dish = bFood[2]};
	mealTicket lnch[3] = {[0].dish = lFood[0], [1].dish = lFood[1], [2].dish = lFood[2]};
	mealTicket dnr[3] = {[0].dish = dFood[0], [1].dish = dFood[1], [2].dish = dFood[2]};
	mealTicket br[3] = {[0].dish = brFood[0], [1].dish = brFood[1], [2].dish = brFood[2]};
	mealTicket ticket = {.ticketNum=0, .dish=dsh};
	
	//STEP-1: Initialize the registry
	
	//STEP-2: Create the publisher thread-pool
	
	//STEP-3: Create the subscriber thread-pool

	//STEP-4: Join the thread-pools
	
	//STEP-5: Free the registry
	
	return EXIT_SUCCESS;
}
//=============================================================================