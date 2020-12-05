/* 
 * Command.h
 *
 * Created on: 12/4/2020
 * 	Author: Jarett Nishijo
 */

#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED
//Pseudo Commands
int create_topic(int ID, char *name, int length);
void *query();
int add_pub(char *cmd_file);
int add_sub(char *cmd_file);
int delta(int d);
void *start();
//Pub/Sub Commands
int put(int ID, char *URL, char *caption);
int get(int ID);
void *t_sleep(int ms);
void *stop();
#endif
