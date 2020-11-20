#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxTickets 3
#define MAXNAME 25
#define MAXQUEUES 4

struct MTQ;
struct MTQ *registry[MAXQUEUES];

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
};

//init functions
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
	for (int i = 0 ; i < maxTickets ; i++) {
		printf("Ticket %d: %s\n",registry[index]->buffer[i]->ticketNum,registry[index]->buffer[i]->dish_name);
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

	if (queue->length == maxTickets) {
		fprintf(stderr, "pushing: Queue %s - Error Queue is full.\n",queue->name);
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

	if (queue->length == 0) {
		fprintf(stderr, "popping: Queue %s - Queue is empty, nothing to pop\n", queue->name);
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
		return 1;
	}
}

int main(int argc, char *argv[]) {
	//MALLOC QUEUES
	struct MTQ *Breakfast = malloc(sizeof(struct MTQ));
	struct MTQ *Lunch = malloc(sizeof(struct MTQ));
	struct MTQ *Dinner = malloc(sizeof(struct MTQ));
	struct MTQ *Bar = malloc(sizeof(struct MTQ));

	//YUM BREAKFAST
	struct mealTicket *bacon = malloc(sizeof(struct mealTicket));
	struct mealTicket *eggs = malloc(sizeof(struct mealTicket));
	struct mealTicket *rice = malloc(sizeof(struct mealTicket));
	struct mealTicket *extra = malloc(sizeof(struct mealTicket));
	initialize_mt(bacon, 1, "Bacon");
	initialize_mt(eggs, 2, "Eggs");
	initialize_mt(rice, 3, "Rice");
	initialize_mt(extra, 4, "extra");

	//TIME FOR LUNCH
	struct mealTicket *Sandwich = malloc(sizeof(struct mealTicket));
	struct mealTicket *Chips = malloc(sizeof(struct mealTicket));
	struct mealTicket *Cookie = malloc(sizeof(struct mealTicket));
	initialize_mt(Sandwich, 1, "Sandwich");
	initialize_mt(Chips, 2, "Chips");
	initialize_mt(Cookie, 3, "Cookie");

	//HUNGRY FOR DINNER 
	struct mealTicket *Steak = malloc(sizeof(struct mealTicket));
	struct mealTicket *Potatos = malloc(sizeof(struct mealTicket));
	struct mealTicket *Salad = malloc(sizeof(struct mealTicket));
	initialize_mt(Steak, 1, "Steak");
	initialize_mt(Potatos, 2, "Potatos");
	initialize_mt(Salad, 3, "Salad");

	//I NEED A DRINK
	struct mealTicket *Soda = malloc(sizeof(struct mealTicket));
	struct mealTicket *Beer = malloc(sizeof(struct mealTicket));
	struct mealTicket *Wine = malloc(sizeof(struct mealTicket));
	initialize_mt(Soda, 1, "Soda");
	initialize_mt(Beer, 2, "Beer");
	initialize_mt(Wine, 3, "Wine");

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

	//enqueue
	enqueue("Breakfast", bacon);
	enqueue("Breakfast", eggs);
	enqueue("Breakfast", rice);

	enqueue("Lunch", Sandwich);
	enqueue("Lunch", Chips);
	enqueue("Lunch", Cookie);

	enqueue("Dinner", Steak);
	enqueue("Dinner", Potatos);
	enqueue("Dinner", Salad);

	enqueue("Bar", Soda);
	enqueue("Bar", Beer);
	enqueue("Bar", Wine);
	enqueue("Bar", extra);

	//begin dequeue
	struct mealTicket *deq = malloc(sizeof(struct mealTicket));

	for(int i = 0 ; i < maxTickets ; i++) {
		for (int j = 0 ; j < MAXQUEUES ; j++) {
			dequeue(registry[j]->name, deq);
		}
	}
	dequeue(registry[0]->name, deq);

	/*
	display_Q(0);
	display_Q(1);
	display_Q(2);
	display_Q(3);
	*/

	for(int i = 0 ; i < MAXQUEUES ; i++) {
		destroy(registry[i]);
	}
	free(bacon);
	free(eggs);
	free(rice);
	free(extra);

	free(Sandwich);
	free(Chips);
	free(Cookie);

	free(Steak);
	free(Potatos);
	free(Salad);

	free(Soda);
	free(Beer);
	free(Wine);
	free(deq);
}
		
		
