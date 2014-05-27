
#include "kernel.h"
#include "process.h"
unsigned char  inb  (unsigned short port) { unsigned char  _v;  __asm__ __volatile__ ("in" "b" " %"  "w"  "1,%"   ""   "0"  : "=a" (_v) : "Nd" (port)   ); return _v; }
/*
   unsigned char  inb_p (unsigned short port) { unsigned char  _v;  __asm__ __volatile__ ("in" "b" " %"  "w"  "1,%"   ""   "0"  "\noutb %%al,$0x80"   : "=a" (_v) : "Nd" (port)   ); return _v; }
 */

unsigned short  inw  (unsigned short port) { unsigned short  _v;  __asm__ __volatile__ ("in" "w" " %"  "w"  "1,%"   ""   "0"  : "=a" (_v) : "Nd" (port)   ); return _v; }
/*
   unsigned short  inw_p (unsigned short port) { unsigned short  _v;  __asm__ __volatile__ ("in" "w" " %"  "w"  "1,%"   ""   "0"  "\noutb %%al,$0x80"   : "=a" (_v) : "Nd" (port)   ); return _v; }
 */
void outb  (unsigned   char   value, unsigned short port) {  __asm__ __volatile__ ("out" "b" " %"   "b"   "0,%"  "w"  "1"  : : "a" (value), "Nd" (port)); }
/*
   void outb_p (unsigned   char   value, unsigned short port) {  __asm__ __volatile__ ("out" "b" " %"   "b"   "0,%"  "w"  "1"  "\noutb %%al,$0x80"   : : "a" (value), "Nd" (port));}
 */
void outw  (unsigned   short   value, unsigned short port) {  __asm__ __volatile__ ("out" "w" " %"   "w"   "0,%"  "w"  "1"  : : "a" (value), "Nd" (port)); }
/*
   void outw_p (unsigned   short   value, unsigned short port) {  __asm__ __volatile__ ("out" "w" " %"   "w"   "0,%"  "w"  "1"  "\noutb %%al,$0x80"   : : "a" (value), "Nd" (port));}
 */


void main_init()
{
		char rtc_A, rtc_B, rtc_C;
		int i;

		/* init irq0 entry 0x20 (timer) */
		/** Initialise l'interruption 0 (celle du timer) 
		 * pour qu'elle execute la minikernel_irq0 (present
		 * dans head.S. minikernel_irq0 sauvegarde l'état et
		 * appelle la fonction do_minikernel_irq0 ci-dessous.
		 * */ 
		{
				extern unsigned long idt_table[];
				long addr=(long)minikernel_irq0;
				unsigned short* pidt=(unsigned short*)(idt_table+(0x20<<1));
				pidt[0]= addr;
				pidt[3]= (((long)addr)>>16)&0xffff;
		}

}

void do_minikernel_irq0()
{
		static int count=-1;
		static int time=0;
		count++;
		if ((count%10)==0) {
				unsigned char irq7_0=inb(0x21);
				unsigned char irq15_8=inb(0xa1);
				kprintf(&sc_alive,
								"\nmini_kernel is alive since %010d secondes, IRQ [15:0]=%02x%02x",
								time,irq7_0,irq15_8
					   );
				count=0;
				time++;
		}
		if(firstSchedule==0)
		{
				kprintf(&(sc_tty_user[3]),"plop");
				__asm__ __volatile__ ("cli\n\t");
				scheduler();
				//__asm__ __volatile__ ("sti\n\t");
		}
}
