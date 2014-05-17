#include "syscall.h"
#include "kernel.h"
#include "null.h"

#define READ 1
#define WRITE 2
#define SLEEP 3

void syscall_init()
{
	/* init the int 0x80 (clavier) */
	extern unsigned long idt_table[];
	long addr=(long)syscall_handler;
	unsigned short* pidt=(unsigned short*)(idt_table+(0x50<<1));
	pidt[0]= addr;
	pidt[3]= (((long)addr)>>16)&0xffff;

}


void* syscall(int func, void* arg1)
{
	// TODO  rajouter la vérification sur les arguments
	// lors de la protection mémoire
	void * ret = NULL;
	switch(func)
	{	
		case READ: // TODO  appel de la fonction read
			break;
		case WRITE: // TODO appel de la fonction write      
			break;
		case SLEEP: // TODO appel de la fonction sleep
			break;
		default : 
			break;
	}

}
