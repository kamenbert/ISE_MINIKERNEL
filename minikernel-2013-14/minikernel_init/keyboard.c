#include "kernel.h"
#include "keyboard.h"
#include "keys.h"
#include "process.h"

#define ALT	56
#define TAB	15
#define SHIFT_L	42
#define BACKSPACE 14
#define SHIFT_R 54
#define CAPSLOCK 58
#define ENTER	28
#define ONE	2
#define TWO	3
#define THREE	4
#define FOUR	5
/**
 * Status of the keyboard
 **/
struct Kbd_State
{
	int echo; 	// Mis a vrai, permet d'afficher les caracteres ajoute
	// dans le buffer
	int maj;	// Vrai si maj
	int alt;	// Vrai si alt est enfoncé
};

struct Kbd_State kbd_state;



/*
 * Affectation of the handler keyboard_irq1() to
 * the keyboard interruption.
 */
void kbd_init()
{
	int i;
	kbd_state.echo = 1;
	kbd_state.maj = 0;
	kbd_state.alt = 0;


	/* init irq1 entry 0x21 (clavier) */
	{
		extern unsigned long idt_table[];
		long addr=(long)kbd_irq1;
		unsigned short* pidt=(unsigned short*)(idt_table+(0x21<<1));
		pidt[0]= addr;
		pidt[3]= (((long)addr)>>16)&0xffff;
	}

}

/**
 * Change the focused process state to stopped if he is running
 * and to running if he is stopped
 **/ 
void kbd_changeProcessState()
{
	if(focus->state == STOPPED)
		focus->state = RUNNING;
	else
	{
		if(focus->state == RUNNING)
			focus->state = STOPPED;
	}
}

/**
 * Move the focus to the "next" process
 **/
void kbd_changeFocus(int next)
{
	printborder(focus->tty_info,7);
	printborder(focus->tty_user,7);
	
	if(next == -1) {
		next = (focus_+1) %4;
		focus_ = next;
	}
	focus = task_table[next];
	
	int green = 2;
	printborder(focus->tty_info,green);
	printborder(focus->tty_user,green);
	return;
}

/**
 * Return true if the current buffer is empty,
 * false otherwise
 **/
int empty()
{
	return current->buffer_filling == 0;
}

/**
 * Return true if the focused buffer is full,
 * false otherwise
 **/
int full()
{
	return focus->buffer_filling == KBD_BUFFER_SIZE;
}

/**
 * Put the character c in the focused buffer
 **/
void kbd_pushBuffer(char c)
{
	// If the buffer is full we do nothing
	if(full())
		return;

	// Otherwise, we add the character at the end
	// of the buffer
	focus->buffer[focus->buffer_filling] = c;
	focus->buffer_filling++;
}

/**
 * Pop the first char out of the current buffer
 **/
char kbd_popBuffer()
{
	// If the buffer is full we do nothing
	if(empty())
		return;
	// Otherwise, we pop out the character at the head
	// of the buffer
	char c = current->buffer[0];
	int i;
	for(i=0; i<current->buffer_filling-1; i++)
		current->buffer[i]=current->buffer[i+1];
			
	current->buffer_filling--;
}

/**
 * Return the key value of the scancode
 **/
char kbd_translateScancode(int scancode)
{
	return !kbd_state.maj ? keys[scancode -1] : highKeys[scancode-1];
}

/**
 * Do the appropriate operation for the input scancode
 */
void kbd_doScancode(int scancode, int up)
{
	// Action lors du levé d'une touche
	if(up)
	{
		scancode = scancode - 128;

		switch(scancode){
			case ALT : 
				kbd_state.alt = 0; // désactivation de ALT
				break;
			case SHIFT_L :
			case SHIFT_R :
				kbd_state.maj = !kbd_state.maj; // désactivation des majuscules
				break;
			default : 
				break; // sinon rien
		}
	}
	else{
		switch(scancode){
			case ALT : 
				kbd_state.alt = 1; // Activation de ALT
				break;
			case SHIFT_L :
			case SHIFT_R :
			case CAPSLOCK :
				kbd_state.maj = !kbd_state.maj; // activation des majuscules
				break;
			case TAB :
				kbd_changeFocus(-1);
				break;
			case BACKSPACE :
				kbackspace(focus->tty_user);
			default : // traitement plus complet

				if(kbd_state.alt && scancode == ENTER)
				{
					kbd_changeProcessState();
					break;
				}
				if(kbd_state.alt &&  ONE <= scancode && scancode <= FOUR)
				{
					// ONE = 2 && tab begin with 0
					kbd_changeFocus(scancode-2); 
					break;
				}

				char c = kbd_translateScancode(scancode);

				// Si c différent de '\0'
				if(c) kbd_pushBuffer(c);

				if(kbd_state.echo && c)
				{
					kprintf(focus->tty_user, "%c", c);
				}
				break;
		}
	}
}

/**
 * Called by the routine, launch the scancode processing
 **/
void kbd_output(int scancode)
{

	int state = scancode & KEY_UP;
	kbd_doScancode(scancode, state);
}
