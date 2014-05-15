#include "process.h"
#include <stdlib.h>

//create a process and return a pointer to it
struct task_struct* init_struct_task(int tty_user, int tty_info) { 

	struct task_struct* tss = malloc(sizeof(struct task_struct));
	if(tss == NULL) {
		return NULL;
	}
	tss->state = RUNNING;
	tss->eax = tss->ebx = tss->ecx = tss->edi = tss->esi = tss->ebp = tss->esp = tss->eip = 0;
	int tss->es = tss->cs = tss->ss = tss->ds = tss->fs = tss->gs = 0;
	tss->nb_ticks_sleep = 0;
	tss->nb_ticks_alive = 0;
	tss->sleep_length = 0; 
	tss->tty_user = &sc_tty_user[tty_user];
	tss->tty_info = &sc_tty_info[tty_info];
	tss->buffer_filling = 0;

	return tss;
}

//initiate the task_table, return -1 if failed, 0 if ok.
int init_task_table() {
	task_table[0] = init_struct_task(0, 0); 
	if(task_table[0] == NULL) {
		return -1;
	}
	task_table[1] = init_struct_task(1, 1); 
	if(task_table[1] == NULL) {
		return -1;
	}
	task_table[2] = init_struct_task(2, 2); 
	if(task_table[2] == NULL) {
		return -1;
	}
	task_table[3] = init_struct_task(3, 3); 
	if(task_table[3] == NULL) {
		return -1;
	}
	return 0;
}

//initiate all variables from process.h, return -1 if failed, 0 if ok. 
int init_process() {
	int ret = init_task_table();
	if(ret==-1) {
		return -1;
	}
	focus = task_table[0]; // current focus
	current = task_table[0]; // current active process
	focus_ = 0;
	current_ = 0;
	return 0;
}

/*
int scheduler() {
	int state = DEAD;
	int f = focus_
	while() {
		f = (f+1)%4;
		if(task_table[f]->state == DEAD) { 
				continue;
		}
		if(task_table[f]->state == SLEEPING) { 
			task_table[f]->nb_ticks_sleeping++;
			if(task_table[f]->nb_ticks_sleeping 
*/				
