#include "process.h"
#include "kernel.h"


void go_inactive(struct task_struct* tss) {
	long eax;
	long ebx;
	long ecx;
	long edi;
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
		"movl %%eax, %0 ;"
		"movl %%ebx, %1 ;"
		"movl %%ecx, %2 ;"
		"movl %%edi, %3 ;"
		"movl %%ebp, %4 ;"
		"movl %%esp, %5 ;"
		"movl %%eip, %6 ;"
		"mov %%es, %7;"
		"mov %%cs, %8;"
		"mov %%ss, %9;"
		"mov %%ds, %10;"
		"mov %%fs, %11;"
		"mov %%gs, %12;"
		: 	"=r" (eax),
		 	"=r" (ebx),
		 	"=r" (ecx),
		 	"=r" (edi),
		 	"=r" (ebp),
		 	"=r" (esp),
		 	"=r" (eip),
			"=r" (es),
			"=r" (cs),
			"=r" (ss),
			"=r" (ds),
			"=r" (fs),
			"=r" (gs)
	);

	eax = tss->eax;
	ebx = tss->ebx;
	ecx = tss->ecx;
	edi = tss->edi;
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
	long eax = tss->eax;
	long ebx = tss->ebx;
	long ecx = tss->ecx;
	long edi = tss->edi;
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
		"movl %0, %%eax;"
		"movl %1, %%ebx;"
		"movl %2, %%ecx;"
		"movl %3, %%edi;"
		"movl %4, %%ebp;"
		"movl %5, %%esp;"
		"movl %6, %%eip;"
		"mov %7, %%es;"
		"mov %8, %%cs;"
		"mov %9, %%ss;"
		"mov %10, %%ds;"
		"mov %11, %%fs;"
		"mov %12, %%gs;"
		:: 	"r" (eax),
		 	"r" (ebx),
		 	"r" (ecx),
		 	"r" (edi),
		 	"r" (ebp),
		 	"r" (esp),
		 	"r" (eip),
			"r" (es),
			"r" (cs),
			"r" (ss),
			"r" (ds),
			"r" (fs),
			"r" (gs)
	);
			
}
