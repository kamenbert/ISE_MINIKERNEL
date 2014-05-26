#include "process.h"
#include "kernel.h"


void go_inactive(struct task_struct* tss) {
	long ebp;
	long esp;
	long eip;
	int es;
	int cs;
	int ss;
	int ds;
	int fs;
	int gs;

	__asm__ __volatile__ (
		"movl %%ebp, %0 ;"
		"movl %%esp, %1 ;"
		"movl %%eip, %2 ;"
		"mov %%es, %3;"
		"mov %%cs, %4;"
		"mov %%ss, %5;"
		"mov %%ds, %6;"
		"mov %%fs, %7;"
		"mov %%gs, %8;"
		: 
		 	"=m" (ebp),
		 	"=m" (esp),
		 	"=m" (eip),
			"=m" (es),
			"=m" (cs),
			"=m" (ss),
			"=m" (ds),
			"=m" (fs),
			"=m" (gs)
	);

	ebp = tss->ebp;
	esp = tss->esp;
	eip = tss->eip;
	es = tss->es;
	cs = tss->cs;
	ss = tss->ss;
	ds = tss->ds;
	fs = tss->fs;
	gs = tss->gs;
			
}

void go_active(struct task_struct* tss) {
	long ebp = tss->ebp;
	long esp = tss->esp;
	long eip = tss->eip;
	int es = tss->es;
	int cs = tss->cs;
	int ss = tss->ss;
	int ds = tss->ds;
	int fs = tss->fs;
	int gs = tss->gs;

	__asm__ __volatile__ (
		"movl %0, %%ebp;"
		"movl %1, %%esp;" 
		"movl %2, %%eip;"
		"mov %3, %%es;"
		"mov %4, %%cs;"
		"mov %5, %%ss;"
		"mov %6, %%ds;"
		"mov %7, %%fs;"
		"mov %8, %%gs;"
		::
		 	"m" (ebp),
		 	"m" (esp),
		 	"m" (eip),
			"m" (es),
			"m" (cs),
			"m" (ss),
			"m" (ds),
			"m" (fs),
			"m" (gs)
	);
			
}
