#ifndef SYSCALL_H
#define SYSCALL_H

void syscall_init();
void syscall_handler();
void* syscall(int func, void * arg);

#endif
