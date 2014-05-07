#include "kernel.h"
#include "keyboard.h"
#include "keys.h"

#define ALT	56
#define SHIFT_L	42
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
struct KbdState
{
	// ajouter un pointeur vers le buffer du processus courant
	int nbElements;	// represente le nombre d'element present dans le buffer
	int echo; 	// Mis a vrai, permet d'afficher les caracteres ajoute
	// dans le buffer
	int maj;	// Vrai si maj
	int alt;	// Vrai si alt est enfoncé
};

struct KbdState kbdState;



/*
 * Affectation of the handler keyboard_irq1() to
 * the keyboard interruption.
 */
void kbd_init()
{
	int i;
	kbdState.nbElements = 0;
	kbdState.echo = 1;
	kbdState.maj = 0;
	kbdState.alt = 0;


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
 * Change the current process state to stopped if he is running
 * and to running if he is stopped
 **/ 
void kbd_changeProcessState(){
}

/**
 * Move the focus to the "next" process
 **/
void kbd_changeFocus(int next){

}

/**
 * Put the character c in the current buffer
 **/
void kbd_pushBuffer(char c){

}

/**
 * Return the key value of the scancode
 **/
char kbd_translateScancode(int scancode){
	return !kbdState.maj ? keys[scancode -1] : highKeys[scancode-1];
}

/**
 * Do the appropriate operation for the input scancode
 */
void do_scancode(int scancode, int up)
{
	// Action lors du levé d'une touche
	if(up)
	{
		scancode = scancode - 128;

		switch(scancode){
			case ALT : 
				kbdState.alt = 0; // désactivation de ALT
				break;
			case SHIFT_L :
			case SHIFT_R :
				kbdState.maj = 0; // désactivation des majuscules
				break;
			default : 
				break; // sinon rien
		}
	}
	else{
		switch(scancode){
			case ALT : 
				kbdState.alt = 1; // Activation de ALT
				break;
			case SHIFT_L :
			case SHIFT_R :
			case CAPSLOCK :
				kbdState.maj = 1; // activation des majuscules
				break;
			default : // traitement plus complet

				if(kbdState.alt && scancode == ENTER)
				{
					kbd_changeProcessState();
					break;
				}
				if(kbdState.alt &&  ONE <= scancode && scancode <= FOUR)
				{
					kbd_changeFocus(scancode-1);
					break;
				}

				char c = kbd_translateScancode(scancode);

				// Si c différent de '\0'
				if(c) kbd_pushBuffer(c);

				if(kbdState.echo)
				{
					kprintf(&(sc_tty_user[0]), "\n%c", c);
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
	do_scancode(scancode, state);
}
