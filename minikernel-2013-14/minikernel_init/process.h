#ifndef PROCESS_H
#define PROCESS_H

#include "kernel.h"

struct task_struct{
	int state; 
	long eax, ebx, ecx, edi, esi, ebp, esp, eip;
	int es, cs, ss, ds, fs, gs;
	subscreen * tty;
	int nb_ticks_sleep;
	int nb_ticks_alive;
	char buffer[1024];
};

struct task_struct * focus;
struct task_struct * current;

// TODO
// - Table des task_struct -> scheduler
// - fonction d'init, cf vga.h, kernel.h (ne pas oublier d'initialiser focus et current)
// - 

#endif
