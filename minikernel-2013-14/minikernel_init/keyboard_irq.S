/************ Keyboard irq ************/
.text
.globl kbd_irq1
kbd_irq1:
	cld
	
	/* Save the registers */
	pushl %eax

	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %esi
	pushl %edi
	pushl %ebp
	pushl %esp

	/****************************/
	/* Protection mémoire 
	/* ring0 : mode système 
	/* ring3 : mode utilisateur
	*/
	/* Update to ring0 segments */
/*	movl $addr, %eax //Mettre l'adresse du segment systeme
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs
*/
	/* Call the handler */
	movl $0, %eax
	inb $0x60,%al
	pushl %eax
	call kbd_output
	addl $4, %esp

	/* Update to ring3 segments */
/*	movl $addr, %eax // Mettre adresse segment utilisateur
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs
*/	
	/* Acknowledge interrupt */
	movl $0x20, %eax
	outb %al, $0x20

	/* Restore the registers */
	popl %esp
	popl %ebp
	popl %edi
	popl %esi
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	
	iret

