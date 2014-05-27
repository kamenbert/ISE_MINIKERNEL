/*
 * entry to minikernel
 * 
 * initialisation are:
 *   - vga driver
 */

#include <mini_kernel.h>
#include "../minikernel_init/kernel.h"
#include "../minikernel_init/process.h"
void mini_kernel()
{
int i;
int cnt=56;
//ttyS_config(ttyS0_base,12);	// 9600 bauds */
firstSchedule = 0;
scheduler();
while (1) kprintf(&(sc_tty_user[3]),"plip");
}

