/* 
 * quacker.h
 *
 * Date Created: 12/5/2020
 * 	Author: Jarett Nishijo
 */

#ifndef QUACKER_H_INCLUDED
#define QUACKER_H_INCLUDED

/* ====================== Structs ======================*/
typedef struct {
        int flag;
        int TID;
        pthread_t self;
}proxyThread;

/* ====================== Functions =====================*/
void init_proxypool(proxyThread *proxy, int ID);
int get_freeThread(proxyThread *pool);
void display_pool(proxyThread *pool);

/* ======================= Global Variables ===============*/
#define NUMPROXIES 16

double DELTA;
int numQueues;
//Conditional Variables
pthread_cond_t cv;
pthread_mutex_t cm;
//Proxy Thread Pool Inits
//      Publishers
proxyThread pub_pool[NUMPROXIES / 2];
pthread_t pub_thread[NUMPROXIES / 2];
//      Subscribers
proxyThread sub_pool[NUMPROXIES / 2];
pthread_t sub_thread[NUMPROXIES / 2];
pthread_t cl;

#endif
