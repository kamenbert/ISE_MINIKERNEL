#include "process.h"
#include "keyboard.h"
#include "kernel.h"

void sys_sleep(unsigned int sec)
{
	current->state = SLEEPING;
	current->sleep_length = sec;
	scheduler();
}


void sys_write(char c)
{
	kprintf(current->tty_user,"%c", c);
}

char sys_read()
{
	char c;
	// On attend de recupérer quelque chose du buffer
	// -> read bloquant
	while((c = kbd_popBuffer()) == (char) NULL);
	return c;
}
