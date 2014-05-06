#include "kernel.h"
#include "keyboard.h"

/*
 * Affectation of the handler keyboard_irq1() to
 * the keyboard interruption.
 */
void kbd_init(){
	int i;
	kbdState.nbElements = 0;
	kbdState.echo = 1;


	/* init irq1 entry 0x21 (clavier) */
	{
		extern unsigned long idt_table[];
		long addr=(long)kbd_irq1;
		unsigned short* pidt=(unsigned short*)(idt_table+(0x21<<1));
		pidt[0]= addr;
		pidt[3]= (((long)addr)>>16)&0xffff;
	}

}


void do_scancode(int scancode, int down){

	if(!down){
		scancode = scancode - 128;
	}
	if(kbdState.echo)
	{
		/*
		 * kprintc(&(sc_tty_info[0]),'a');
		 * kprintc(&(sc_tty_user[0]),'a');
		 * kprintc(&(sc_tty_info[1]),'b');
		 * kprintc(&(sc_tty_user[1]),'b');
		 * kprintc(&(sc_tty_info[2]),'c');
		 * kprintc(&(sc_tty_user[2]),'c');
		 * kprintc(&(sc_tty_info[3]),'d');
		 * kprintc(&(sc_tty_user[3]),'d');
		 * */

		if(down) 
			kprintf(&(sc_tty_user[0]), "\nKey DOWN Scancode : %i", scancode );
		else
			kprintf(&(sc_tty_user[3]), "\nKey UP Scancode : %i ", scancode);
	}

}


void kbd_output(int scancode)
{

	int state = scancode & KEY_UP;
	do_scancode(scancode, state);
}
