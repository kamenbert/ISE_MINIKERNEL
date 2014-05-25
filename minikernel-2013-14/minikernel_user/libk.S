.text
.global read
.align 4

read:

push %ebp
mov %esp, %ebp
push %ebx
mov $(1), %eax
#mov 8(%ebp), %ebx
#mov 12(%ebp), %ecx
#mov 16(%ebp), %edx
int $0x80
movl %eax, %edx
#test %edx, %edx
#jge Lexit
#negl %edx
#push %edx
#call __errno_location
#		pop %edx
#movl %edx, (%eax)
#		movl $-1, %eax

#		Lexit:
pop %ebx
mov %ebp, %esp
pop %ebp
ret


.text
.global write 
.align 4

write:

push %ebp
mov %esp, %ebp
push %ebx
mov $(2), %eax
mov 8(%ebp), %ebx
#mov 12(%ebp), %ecx
#mov 16(%ebp), %edx
int $0x80
movl %eax, %edx
#test %edx, %edx
#jge Lexit
#negl %edx
#push %edx
#call __errno_location
#		pop %edx
#movl %edx, (%eax)
#		movl $-1, %eax

#		Lexit:
pop %ebx
mov %ebp, %esp
pop %ebp
ret


.text
.global sleep 
.align 4

sleep:

push %ebp
mov %esp, %ebp
push %ebx
mov $(3), %eax
mov 8(%ebp), %ebx
#mov 12(%ebp), %ecx
#mov 16(%ebp), %edx
int $0x80
movl %eax, %edx
#test %edx, %edx
#jge Lexit
#negl %edx
#push %edx
#call __errno_location
#		pop %edx
#movl %edx, (%eax)
#		movl $-1, %eax

#		Lexit:
pop %ebx
mov %ebp, %esp
pop %ebp
ret
