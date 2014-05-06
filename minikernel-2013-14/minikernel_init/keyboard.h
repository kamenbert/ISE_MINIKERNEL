#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_BUFFER_SIZE 2000
#define KBD_KEY_NUMBER 128
#define KEY_UP 0x80

struct KbdState{
	// ajouter un pointeur vers le buffer du processus courant
	int nbElements;	// represente le nombre d'element present dans le buffer
	int echo; 	// Mis a vrai, permet d'afficher les caracteres ajoute
			// dans le buffer
};

struct KbdState kbdState;

char kbdBuffer[KBD_BUFFER_SIZE];

/**********************************************************************/
/**** Initialisation of the keyboard				   ****/
void kbd_init();

/**********************************************************************/
/**** Manage the keyboard interrupt				   ****/
void kbd_irq1();
void kbd_output(int scancode);

#endif
