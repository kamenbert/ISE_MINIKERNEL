.text

/************	int 0x80	************/
.globl syscall_handler
syscall_handler:
	
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
	pushl %ebx
	pushl %eax
	call syscall
	addl $8, %esp

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

