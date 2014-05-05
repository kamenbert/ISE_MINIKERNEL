#include "kernel.h"
#include "keyboard.h"

struct KbdState{
	int read;
};

/*
 * Affectation of the handler keyboard_irq1() to
 * the keyboard interruption.
 */
void kbd_init(){
	int i;
	for(i = 0; i<KBD_KEY_NUMBER; i++)
	{
		kbdKeyPressed[i] = '0';
	}

	/* init irq1 entry 0x21 (clavier) */
	{
		extern unsigned long idt_table[];
		long addr=(long)kbd_irq1;
		unsigned short* pidt=(unsigned short*)(idt_table+(0x21<<1));
		pidt[0]= addr;
		pidt[3]= (((long)addr)>>16)&0xffff;
	}

}


void do_scancode(int scancode){

}


void kbd_output(int scancode)
{
	unsigned short masque_state = 0x80;
	unsigned short state = scancode & masque_state;
	kprintf(&sc_user, "\n Scancode : %04x ET masque_state : %04x = state : %04x ", scancode,masque_state, state );
	unsigned short value = state == 0x80 ? scancode - 256 : scancode; //scancode ^ masque_state;
	kprintf(&sc_user, "\n Scancode : %04x XOR masque_state = %04x = value :  %04x ", scancode,masque_state, value);

}
