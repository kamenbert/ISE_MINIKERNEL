#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_BUFFER_SIZE 2000
#define KBD_KEY_NUMBER 88
#define KEY_UP 0x80


/**********************************************************************/
/**** Initialisation of the keyboard				   ****/
void kbd_init();

/**********************************************************************/
/**** Environement changes					   ****/
void kbd_changeProcessState();
void kbd_changeFocus(int next);



/**********************************************************************/
/**** Manipulation of the buffer				   ****/
void kbd_pushBuffer(char c);
char kbd_popBuffer();

/**********************************************************************/
/**** Manage the keyboard interrupt				   ****/
void kbd_irq1();
void kbd_output(int scancode);

#endif
