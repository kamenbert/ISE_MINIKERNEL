#include "syscall.h"
#include "kernel.h"
#include "sys_functions.h"

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
		case READ: 
			ret =  (void *) sys_read();
			break;
		case WRITE:
			sys_write((char) arg1);
			break;
		case SLEEP: 
			sys_sleep((unsigned int) arg1);
			break;
		default : 
			break;
	}
	return ret;
}
