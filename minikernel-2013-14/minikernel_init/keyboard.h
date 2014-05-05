#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KBD_BUFFER_SIZE 2000
#define KBD_KEY_NUMBER 127

char kbdBuffer[KBD_BUFFER_SIZE];
char kbdKeyPressed[127];

/**********************************************************************/
/**** Initialisation of the keyboard				   ****/
void kbd_init();

/**********************************************************************/
/**** Manage the keyboard interrupt				   ****/
void kbd_irq1();
void kbd_output(int scancode);

#endif
