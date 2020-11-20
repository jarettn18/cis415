#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxTickets 4
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

void destroy(struct MTQ *Q_ptr) {
	free(Q_ptr->buffer);
	free(Q_ptr);
}

void display_Q(int index) {
	for (int i = 0 ; i < maxTickets ; i++) {
		printf("Ticket %d: %s\n",i,registry[index]->buffer[i]->dish_name);
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

int main(int argc, char *argv[]) {
	struct MTQ *Breakfast = malloc(sizeof(struct MTQ));
	struct MTQ *Lunch = malloc(sizeof(struct MTQ));
	struct MTQ *Dinner = malloc(sizeof(struct MTQ));
	struct MTQ *Bar = malloc(sizeof(struct MTQ));

	struct mealTicket *bacon = malloc(sizeof(struct mealTicket));
	struct mealTicket *eggs = malloc(sizeof(struct mealTicket));
	struct mealTicket *rice = malloc(sizeof(struct mealTicket));
	struct mealTicket *extra = malloc(sizeof(struct mealTicket));

	initialize_mtq(Breakfast, "Breakfast");
	initialize_mtq(Lunch, "Lunch");
	initialize_mtq(Dinner, "Dinner");
	initialize_mtq(Bar, "Bar");

	initialize_mt(bacon, 1, "Bacon");
	initialize_mt(eggs, 2, "Eggs");
	initialize_mt(rice, 3, "Rice");
	initialize_mt(extra, 4, "extra");

	registry[0] = Breakfast;
	registry[1] = Lunch;
	registry[2] = Dinner;
	registry[3] = Bar;
	enqueue("Breakfast", bacon);
	enqueue("Breakfast", eggs);
	enqueue("Breakfast", rice);
	enqueue("Breakfast", extra);

	display_Q(0);

	for(int i = 0 ; i < MAXQUEUES ; i++) {
		destroy(registry[i]);
	}
}
		
		
