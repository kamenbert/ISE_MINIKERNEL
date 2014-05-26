#ifndef PROCESS_H
#define PROCESS_H

#include "kernel.h"
#include "keyboard.h"

#define DEAD 0
#define SLEEPING 1
#define RUNNING 2
#define READING 3
#define STOPPED 4

struct task_struct{
	int state;
	/* STATE :
	dead :		0 
	sleeping :	1
	running : 	2
	reading :	3
	stopped :	4
	*/
	long ebp, esp, eip;
	int es, cs, ss, ds, fs, gs;
	subscreen * tty_user; // in which tty does I appear ?
	subscreen * tty_info; // in which tty does my infos appear ?
	int nb_ticks_sleep; //sleeping since how long ?
	int nb_ticks_alive; // alive since how long ?
	int nb_ticks_active; // how much has it been active ?
	int nb_ticks_stopped; // how much has it been stopped ?
	int sleep_length; //sleep time
	char buffer[KBD_BUFFER_SIZE]; // buffer for reading
	int buffer_filling; //filling of the buffer
};

//this is usefull only for initialization
struct process{
	long ebp, esp, eip;
	int es, cs, ss, ds, fs, gs;
}

//SOME GLOBAL VARIABLES
struct task_struct* focus; // current focus
struct task_struct* current; // current active process
struct task_struct* task_table[4]; //process table
struct task_struct task_struct_table[4]; //the real process table

//I added these for my personnal convenience
int focus_;
int current_;

//INIT FUNCTIONS
int init_process();
int init_task_table();
//struct task_struct* init_struct_task(int tty_user, int tty_info);
int init_struct_task(struct task_struct* tss,int tty_user, int tty_info);

void scheduler();

// TODO
// - la vraie tss de la gdt

#endif
