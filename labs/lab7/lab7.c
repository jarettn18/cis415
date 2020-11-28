#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define maxTickets 3
#define MAXNAME 25
#define MAXDISH 20
#define MAXQUEUES 4
#define MAXPUBS 3
#define MAXSUBS 4

struct MTQ;
struct MTQ *registry[MAXQUEUES];
pthread_cond_t cv;
pthread_mutex_t cm;

struct mealTicket {
	int ticketNum;
	char *dish_name;
};

struct MTQ {
	char name[MAXNAME];
	struct mealTicket **buffer;
	int head;
	int tail;
	int length;
	pthread_mutex_t mutex;
};

struct pargs {
	struct mealTicket **MT_array;
	char *MTQ_ID_array[MAXDISH];
	int TID;
	int numtix;
};

struct sargs {
	char *MTQ_ID;
	int TID;
	struct mealTicket *MT;
};

//init functions
void initialize_mutex(int pos)
{
	int success = pthread_mutex_init(&registry[pos]->mutex,NULL);
	if (success != 0) {
		fprintf(stderr, "Error: MUTEX NOT INITIALIZED\n");
		exit(EXIT_FAILURE);
	}
}

void initialize_mt(struct mealTicket *mt, int ticket, char *name) {
	mt->ticketNum = ticket;
	mt->dish_name = name;
}

void initialize_mtq(struct MTQ *Q_ptr, char *name) {
	strcpy(Q_ptr->name, name);
	Q_ptr->buffer = malloc(sizeof(struct mealTicket *) * maxTickets);
	Q_ptr->head = 0;
	Q_ptr->tail = 0;
	Q_ptr->length = 0;
}

//free functions
void destroy(struct MTQ *Q_ptr) {
	free(Q_ptr->buffer);
	free(Q_ptr);
}

//Helper function displays contents of a Queue pass in index of registry
void display_Q(int index) {
	printf("%s\n",registry[index]->name);
	if (registry[index]->length == 0) {
		printf("\tQueue is Empty\n");
		return;
	}
	for (int i = 0 ; i < maxTickets ; i++) {
		printf("\tTicket %d: %s\n",registry[index]->buffer[i]->ticketNum,registry[index]->buffer[i]->dish_name);
	}
}

int enqueue(char *MTQ_ID, struct mealTicket *MT) {
	struct MTQ *queue;
	int queue_i = -1;
	for (int i = 0 ; i < MAXQUEUES ; i++) {
		if (strcmp(MTQ_ID, registry[i]->name) == 0) {
			queue_i = i;
			break;
		}
	}
	if (queue_i == -1) {
		fprintf(stderr,"%s is not a valid Queue\n",MTQ_ID);
		return 0;
	}
	queue = registry[queue_i];
	//Aquire Mutex
	int aquire = pthread_mutex_lock(&queue->mutex);
	//
	if (queue->length == maxTickets) {
		fprintf(stderr, "pushing: Queue %s - Error Queue is full.\n",queue->name);
		pthread_mutex_unlock(&queue->mutex);
		return 0;
	}
	else {
		fprintf(stdout, "pushing: Queue %s - Ticket Number: %d - Dish %s\n", queue->name, MT->ticketNum, MT->dish_name);
		queue->buffer[queue->head] = MT;
		queue->head++;
		if (queue->head == maxTickets) {
			queue->head = 0;
		}
		queue->length++;
		pthread_mutex_unlock(&queue->mutex);
		return 1;
	}
}

int dequeue(char *MTQ_ID, struct mealTicket *MT) {
	struct MTQ *queue;
	int queue_i = -1;
	for (int i = 0 ; i < MAXQUEUES ; i++) {
		if (strcmp(MTQ_ID, registry[i]->name) == 0) {
			queue_i = i;
			break;
		}
	}
	if (queue_i == -1) {
		fprintf(stderr,"%s is not a valid Queue\n",MTQ_ID);
		return 0;
	}
	queue = registry[queue_i];
	//Aquire Mutex
	pthread_mutex_lock(&queue->mutex);

	if (queue->length == 0) {
		fprintf(stderr, "popping: Queue %s - Queue is empty, nothing to pop\n", queue->name);
		pthread_mutex_unlock(&queue->mutex);
		return 0;
	}
	else {
		fprintf(stdout, "popping: Queue %s - Ticket Number: %d - Dish %s\n",queue->name, queue->buffer[queue->tail]->ticketNum, queue->buffer[queue->tail]->dish_name);
		MT->ticketNum = queue->buffer[queue->tail]->ticketNum;
		MT->dish_name = queue->buffer[queue->tail]->dish_name;
		queue->buffer[queue->tail]->ticketNum = 0;
		queue->buffer[queue->tail]->dish_name = "EMPTY";
		queue->tail++;
		if (queue->tail == maxTickets) {
			queue->tail = 0;
		}
		queue->length--;
		pthread_mutex_unlock(&queue->mutex);
		return 1;
	}
}

void *publisher(void *args)
{
 /* ODO: The publisher will receive the following in the struct args:
        *        1. An array of mealTickets to push to the queue.
        *        2. For each meal ticket, an MTQ_ID. (Note: a publisher can push to multiple queues)
        *        3. The thread ID
        * The publisher will do the pthread_cond_wait procedure, and wait for a cond signal to begin its work
        * The publisher will then print its type and thread ID on startup. Then it will push one meal ticket at a time to
        * its appropriate queue before sleeping for 1 second. It will do this until there are no more meal tickets to push.
        */
	struct pargs *arg = (struct pargs *)args;
	int TID = pthread_self();
	int numtix = arg->numtix;

	printf("TYPE: PUBLISHER || THREAD %u: WAITING FOR SIGNAL\n",TID);
	pthread_cond_wait(&cv, &cm);
	pthread_mutex_unlock(&cm);
	printf("TYPE: PUBLISHER || THREAD ID: %u\n", TID);
	for (int i = 0 ; i < numtix ; i++) {
		enqueue(arg->MTQ_ID_array[i],arg->MT_array[i]);
		sleep(1);
	}
}

void *subscriber(void *args)
{
/* ODO:The subscriber will take the following:
        *       1. The MTQ_ID it will pull from.
        *       2. The thread ID
        *       3. An empty meal ticket.
        * The subscriber will do the pthread_cond_wait procedure, and wait for a cond signal to begin its work
        * The subscriber will print its type and thread ID on startup. Then it will pull a ticket from its queue
        * and print it. If the queue is empty then it will print an empty message along with its
        * thread ID and wait for 1 second. If the thread is not empty then it will pop a ticket and 
        * print it along with the thread id.
        */
	struct sargs *arg = (struct sargs *)args;
	int TID = pthread_self();
	printf("TYPE: SUBSCRIBER || THREAD %u: WAITING FOR SIGNAL\n",TID);
	pthread_cond_wait(&cv, &cm);
	pthread_mutex_unlock(&cm);
	printf("TYPE: SUBSCRIBER || THREAD ID: %u\n", TID);
	int success = dequeue(arg->MTQ_ID, arg->MT);
	if (success == 0) {
		fprintf(stdout, "THREAD ID %u: Queue is empty\n",TID);
		sleep(1);
	}
	else {
		fprintf(stdout, "THREAD ID %u: popped Meal Ticket %s\n",TID, arg->MT->dish_name);
	}
}

int main(int argc, char *argv[]) {
	//Variable Declaration
	char *bFood[] = {"Eggs", "Bacon", "Steak"};
	char *lFood[] = {"Burger", "Fries", "Coke"};
	char *dFood[] = {"Steak", "Greens", "Pie"};
	char *brFood[] = {"Whiskey", "Sake", "Wine"};
	int i, j, t = 1;
	struct mealTicket bfast[3] = {[0].dish_name = bFood[0], [1].dish_name = bFood[1], [2].dish_name = bFood[2]};
        struct mealTicket lnch[3] = {[0].dish_name = lFood[0], [1].dish_name = lFood[1], [2].dish_name = lFood[2]};
        struct mealTicket dnr[3] = {[0].dish_name = dFood[0], [1].dish_name = dFood[1], [2].dish_name = dFood[2]};
        struct mealTicket br[3] = {[0].dish_name = brFood[0], [1].dish_name = brFood[1], [2].dish_name = brFood[2]};

	//INITIALIZE REGISTRY
	//MALLOC QUEUES
	struct MTQ *Breakfast = malloc(sizeof(struct MTQ));
	struct MTQ *Lunch = malloc(sizeof(struct MTQ));
	struct MTQ *Dinner = malloc(sizeof(struct MTQ));
	struct MTQ *Bar = malloc(sizeof(struct MTQ));

	//initialize queues
	initialize_mtq(Breakfast, "Breakfast");
	initialize_mtq(Lunch, "Lunch");
	initialize_mtq(Dinner, "Dinner");
	initialize_mtq(Bar, "Bar");
	
	//Fill registry
	registry[0] = Breakfast;
	registry[1] = Lunch;
	registry[2] = Dinner;
	registry[3] = Bar;

	//initalize mutexes
	initialize_mutex(0);
	initialize_mutex(1);
	initialize_mutex(2);
	initialize_mutex(3);
	
        //STEP-2: Create the publisher thread-pool
	// BREAKFAST Pub Args
	struct pargs *parg = malloc(sizeof(struct pargs));
	parg->MT_array = malloc(sizeof(struct mealTicket *) * MAXDISH);
	parg->MT_array[0] = &bfast[0];
	parg->MT_array[1] = &bfast[1];
	parg->MT_array[2] = &bfast[2];

	parg->MTQ_ID_array[0] = "Breakfast";
	parg->MTQ_ID_array[1] = "Breakfast";
	parg->MTQ_ID_array[2] = "Breakfast";

	parg->numtix = 3;
	
	//Lunch Pub Args
	struct pargs *pargl = malloc(sizeof(struct pargs));
	pargl->MT_array = malloc(sizeof(struct mealTicket *) * MAXDISH);
	pargl->MT_array[0] = &lnch[0];
	pargl->MT_array[1] = &lnch[1];
	pargl->MT_array[2] = &lnch[2];

	pargl->MTQ_ID_array[0] = "Lunch";
	pargl->MTQ_ID_array[1] = "Lunch";
	pargl->MTQ_ID_array[2] = "Lunch";

	pargl->numtix = 3;
	//Dinner Pub Args
	struct pargs *pargd = malloc(sizeof(struct pargs));
	pargd->MT_array = malloc(sizeof(struct mealTicket *) * MAXDISH);
	pargd->MT_array[0] = &dnr[0];
	pargd->MT_array[1] = &dnr[1];
	pargd->MT_array[2] = &dnr[2];

	pargd->MTQ_ID_array[0] = "Dinner";
	pargd->MTQ_ID_array[1] = "Dinner";
	pargd->MTQ_ID_array[2] = "Dinner";

	pargd->numtix = 3;
	//Bar Pub Args
	struct pargs *pargb = malloc(sizeof(struct pargs));
	pargb->MT_array = malloc(sizeof(struct mealTicket *) * MAXDISH);
	pargb->MT_array[0] = &br[0];
	pargb->MT_array[1] = &br[1];
	pargb->MT_array[2] = &br[2];

	pargb->MTQ_ID_array[0] = "Bar";
	pargb->MTQ_ID_array[1] = "Bar";
	pargb->MTQ_ID_array[2] = "Bar";

	pargb->numtix = 3;

	// Pthreead Create
	pthread_cond_init(&cv, NULL);
	pthread_mutex_init(&cm, NULL);

	pthread_t p1, p2, p3, p4;
	pthread_create(&p1, NULL, publisher, parg);
	pthread_create(&p2, NULL, publisher, pargl);
	pthread_create(&p3, NULL, publisher, pargd);
	pthread_create(&p4, NULL, publisher, pargb);

	sleep(1);
	printf("Broadcasting\n");
	pthread_cond_broadcast(&cv);

        //STEP-3: Create the subscriber thread-pool
	struct sargs *sarg = malloc(sizeof(struct sargs));
	struct sargs *sargl = malloc(sizeof(struct sargs));
	struct sargs *sargd = malloc(sizeof(struct sargs));
	struct sargs *sargb = malloc(sizeof(struct sargs));
	sarg->MTQ_ID = "Breakfast";
	sargl->MTQ_ID = "Lunch";
	sargd->MTQ_ID = "Dinner";
	sargb->MTQ_ID = "Bar";
	sarg->MT = malloc(sizeof(struct mealTicket));;
	sargl->MT = malloc(sizeof(struct mealTicket));;
	sargd->MT = malloc(sizeof(struct mealTicket));;
	sargb->MT = malloc(sizeof(struct mealTicket));;

	pthread_t sp1, sp2, sp3, sp4;
	pthread_create(&sp1, NULL, subscriber, sarg);
	pthread_create(&sp2, NULL, subscriber, sargl);
	pthread_create(&sp3, NULL, subscriber, sargd);
	pthread_create(&sp4, NULL, subscriber, sargb);

	sleep(1);
	pthread_cond_broadcast(&cv);

        //STEP-4: Join the thread-pools
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(p3, NULL);
	pthread_join(p4, NULL);
	pthread_join(sp1, NULL);
	pthread_join(sp2, NULL);
	pthread_join(sp3, NULL);
	pthread_join(sp4, NULL);
        //STEP-5: Free the registry
	for(int i = 0 ; i < MAXQUEUES ; i++) {
		destroy(registry[i]);
	}
}
		
		
