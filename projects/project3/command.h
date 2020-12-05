/* 
 * Command.h
 *
 * Created on: 12/4/2020
 * 	Author: Jarett Nishijo
 */

#ifndef COMMAND_H_
#define COMMAND_h_

//Pseudo Commands
int create_topic(int ID, char *name, int length);
void *query();
int add_pub(void *cmd_file);
int add_sub(void *cmd_file);
int delta(int d);
void *start();

//Pub/Sub Commands
int put(int ID, char *URL, char *caption);
int get(int ID);
void *sleep(int ms);
void *stop();

#endif
