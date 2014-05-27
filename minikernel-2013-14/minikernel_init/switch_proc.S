.text
.global _switch_proc

_switch_proc :
	movl 4(%esp), %eax
	movl %esp,(%eax)
	movl %ebx,4(%eax) 
	movl %ecx,8(%eax)
	movl %edi,12(%eax)
	movl %esi,16(%eax)
	movl %ebp,20(%eax)
	movl %esp,24(%eax)
	mov  %es,28(%eax)
	mov  %cs,32(%eax)
	mov  %ss,36(%eax)
	mov  %ds,40(%eax)
	mov  %fs,44(%eax)
	mov  %gs,48(%eax)

	movl $reprise,52(%eax)
	
	movl 8(%esp),%eax
	movl (%eax),%esp
	movl 4(%eax),%ebx
	movl 8(%eax),%ecx
	movl 12(%eax),%edi
	movl 16(%eax),%esi
	movl 20(%eax),%ebp
	movl 24(%eax),%esp
	mov  28(%eax),%es
	mov  32(%eax),%cs
	mov  36(%eax),%ss
	mov  40(%eax),%ds
	mov  44(%eax),%fs
	mov  48(%eax),%gs
	
	movl 52(%eax),%eax 	
	sti
	jmp *(%eax)
	iret 
reprise:
	ret

