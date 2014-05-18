/************ Keyboard irq ************/
.text
.globl kbd_irq1
kbd_irq1:
	cld
	/* Acknowledge interrupt */
	movl $0x20, %eax
	outb %al, $0x20

	
	/* Save the registers */
	pushl %eax

	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %esi
	pushl %edi
	pushl %ebp
	pushl %esp

	/* Call the handler */
	movl $0, %eax
	inb $0x60,%al
	pushl %eax
	call kbd_output
	addl $4, %esp

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

