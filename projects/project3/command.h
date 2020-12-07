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
int run_pub(char *cmd_file, int ID);
int run_sub(char *cmd_file, int ID);
int delta(int d);
void *start();
//Pub/Sub Commands
void *t_sleep(int ms);
#endif
