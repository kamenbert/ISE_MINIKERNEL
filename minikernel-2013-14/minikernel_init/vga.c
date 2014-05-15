
/*
 * basic print to vga screen for debuging
*/

#include <stdarg.h>
#include "kernel.h"
#include <mini_kernel.h>


/*
 * setup has initialiszed the handler and put all
 * information at 0x90000 adressese
*/
#define SCREEN_INFO (*(struct screen_info *)0x90000)

struct screen_info {
	unsigned char  orig_x;			/* 0x00 */
	unsigned char  orig_y;			/* 0x01 */
	unsigned short dontuse1;		/* 0x02 -- EXT_MEM_K sits here */
	unsigned short orig_video_page;		/* 0x04 */
	unsigned char  orig_video_mode;		/* 0x06 */
	unsigned char  orig_video_cols;		/* 0x07 */
	unsigned short unused2;			/* 0x08 */
	unsigned short orig_video_ega_bx;	/* 0x0a */
	unsigned short unused3;			/* 0x0c */
	unsigned char  orig_video_lines;	/* 0x0e */
	unsigned char  orig_video_isVGA;	/* 0x0f */
	unsigned short orig_video_points;	/* 0x10 */

	/* VESA graphic mode -- linear frame buffer */
	unsigned short lfb_width;		/* 0x12 */
	unsigned short lfb_height;		/* 0x14 */
	unsigned short lfb_depth;		/* 0x16 */
	unsigned long  lfb_base;		/* 0x18 */
	unsigned long  lfb_size;		/* 0x1c */
	unsigned short dontuse2, dontuse3;	/* 0x20 -- CL_MAGIC and CL_OFFSET here */
	unsigned short lfb_linelength;		/* 0x24 */
	unsigned char  red_size;		/* 0x26 */
	unsigned char  red_pos;			/* 0x27 */
	unsigned char  green_size;		/* 0x28 */
	unsigned char  green_pos;		/* 0x29 */
	unsigned char  blue_size;		/* 0x2a */
	unsigned char  blue_pos;		/* 0x2b */
	unsigned char  rsvd_size;		/* 0x2c */
	unsigned char  rsvd_pos;		/* 0x2d */
	unsigned short vesapm_seg;		/* 0x2e */
	unsigned short vesapm_off;		/* 0x30 */
	unsigned short pages;			/* 0x32 */
						/* 0x34 -- 0x3f reserved for future expansion */
};

/********************************************/
/**** real screan                        ****/
/****    - kernel is alive               ****/
/****    - ttyS0 infos                   ****/
/****    - ttyS1 infos                   ****/
/****    - 4 scroll lines                ****/
/****    - user screen                   ****/
static nblines, nbcols;
static char *vidmem;
static int vidport;


subscreen sc_alive, sc_ttyS0, sc_ttyS1, sc_kernel, sc_user;
subscreen sc_tty_user[4];
subscreen sc_tty_info[4];


void printborder (subscreen* psc,char c){
	int nby = psc->nblines;
	int nbx = psc->nbcols;
	int i;
	c *= 16;
	char* border = psc->vidmem - (nbcols + 1)*2;
	for(i=0;i<nbx+2;i++){
		border[1+2*i]=c;
		border[1+2*(i + nbcols * (nby + 1))]=c;
	}
	for(i=1;i<=nby;i++){
		border[1+2*(i * nbcols)]=c;
		border[1+2*(i * nbcols + nbx + 1)]=c;
	}
	return;
}

void vga_init()
{
int nbl;
int i;

	if (SCREEN_INFO.orig_video_mode == 7) {
		vidmem = (char *) 0xb0000;
		vidport = 0x3b4;
	} else {
		vidmem = (char *) 0xb8000;
		vidport = 0x3d4;
	}
	nblines = SCREEN_INFO.orig_video_lines;
	nbcols  = SCREEN_INFO.orig_video_cols;
	/* clear screen */
	for (i=0 ; i< (nblines*nbcols*2) ; i += 2) {
		vidmem[i]=' ';
	}

	nbl= nblines;

	/* init alive screen */
	sc_alive.vidmem= vidmem;
	sc_alive.nblines= 1;
	sc_alive.nbcols= nbcols;
	sc_alive.cline=0;
	sc_alive.ccol=0;
	nbl-=sc_alive.nblines;
	


#if 0
	/* init sc_ttyS0 screen */
	sc_ttyS0.vidmem= sc_alive.vidmem + sc_alive.nblines * nbcols * 2;
	sc_ttyS0.nblines= 1;
	sc_ttyS0.nbcols= nbcols;
	sc_ttyS0.cline=0;
	sc_ttyS0.ccol=0;
	nbl--;

	/* init sc_ttyS1 screen */
	sc_ttyS1.vidmem= sc_ttyS0.vidmem + sc_ttyS0.nblines * nbcols * 2;
	sc_ttyS1.nblines= 0;
	sc_ttyS1.nbcols= nbcols;
	sc_ttyS1.cline=0;
	sc_ttyS1.ccol=0;
	nbl--;

	/* init kernel scroll screen */
	sc_kernel.vidmem= sc_ttyS1.vidmem + sc_ttyS1.nblines * nbcols * 2;
	sc_kernel.nblines= 0;
	sc_kernel.nbcols= nbcols;
	sc_kernel.cline=0;
	sc_kernel.ccol=0;
	nbl -= sc_kernel.nblines;
#endif
	
	/* init user screen */
	sc_user.vidmem= sc_alive.vidmem + sc_alive.nblines * nbcols * 2;
	sc_user.nblines= nbl;
	sc_user.nbcols= nbcols;
	sc_user.cline=0;
	sc_user.ccol=0;
	
	int j;
	int taille_info = 1;
	int largeur_bordure = 1;

	for(j = 0;j < 4; j++){
		int decalage_info=0;
		decalage_info=+ nbcols * largeur_bordure;
		decalage_info+= largeur_bordure;
		decalage_info+= j * (nbcols / 4);

		sc_tty_info[j].vidmem= sc_user.vidmem + 2*decalage_info;
		sc_tty_info[j].nblines= taille_info;
		sc_tty_info[j].nbcols= (nbcols - 5)/4;
		sc_tty_info[j].cline=0;
		sc_tty_info[j].ccol=0;

		printborder(&sc_tty_info[j],7);

		int decalage_user=0;
		decalage_user+= nbcols * sc_tty_info[j].nblines;
		decalage_user+= nbcols * largeur_bordure;
		
		sc_tty_user[j].vidmem= sc_tty_info[j].vidmem + decalage_user * 2;
		sc_tty_user[j].nblines= sc_user.nblines - sc_tty_info[j].nblines;
		sc_tty_user[j].nblines-= 3*largeur_bordure;
		sc_tty_user[j].nbcols= sc_tty_info[j].nbcols;
		sc_tty_user[j].cline=0;
		sc_tty_user[j].ccol=0;

		printborder(&sc_tty_user[j],7);
	}
}

static void vkprintf(subscreen* psc, const char* fmt, va_list args);
static void scroll(subscreen *psc);

void kprintc(subscreen* psc, char c)
{
	int x,y,pos;

	

	x = psc->ccol;
	y = psc->cline;

	if ( c == '\n') {
		x = 0;
		if ( ++y >= psc->nblines ) {
			scroll(psc);
			y--;
		}
	} else {
		psc->vidmem [ x * 2 + nbcols * y * 2 ] = c; 
		if ( ++x >= psc->nbcols ) {
			x = 0;
			y++;
			if ( y >= psc->nblines ) {
				scroll(psc); 
				y--;
	}	}	}

	psc->ccol  = x;
	psc->cline = y;
#if 0 
	pos = (x + nbcols * y) * 2;	/* Update cursor position */
	outb_p(14, vidport);
	outb_p(0xff & (pos >> 9), vidport+1);
	outb_p(15, vidport);
	outb_p(0xff & (pos >> 1), vidport+1);
#endif
}

void kprints(subscreen* psc, const char *p)
{
	while (*p)
		kprintc(psc,*p++);
}

void kprintf(subscreen* psc, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vkprintf(psc,fmt,args);
	va_end(args);
}

void vgaprintc(char c)
{ kprintc(&sc_user,c); }

void vgaprints(const char* s)
{ kprints(&sc_user,s); }

void vgaprintf(const char* fmt,...)
{ 
	va_list args;
	va_start(args, fmt);
	vkprintf(&sc_user,fmt,args);
	va_end(args);
}

static void scroll(subscreen *psc)
{
	int i;

	if ( psc->nblines>1 ){
		for(i=0;i<psc->nblines-1;i++){
			memcpy ( psc->vidmem + i*nbcols*2,
			 	psc->vidmem + (i+1)*nbcols*2,
				psc->nbcols * 2
			);
	}}
	int last = (psc->nblines - 1) * nbcols * 2;
	for ( i = last;
			i < last + psc->nbcols * 2 ;i += 2 )
		psc->vidmem[i] = ' ';
}

static void vkprintf_str(subscreen* psc, const char* str, int len, char fillwith, int placeleft, int signe)
{
int i;
	if (len==0) {
		if (signe) kprintc(psc,signe);
		kprints(psc,str);
	} else {
		int slen=strlen(str) + (signe ? 1 : 0);
		if (slen >= len) {
			if (signe) kprintc(psc,signe);
			kprints(psc,str);
		} else if (placeleft) {
			if (signe) kprintc(psc,signe);
			kprints(psc,str);
			for (i=0; i<(len-slen) ; i++) {
				kprintc(psc,fillwith);
			}
		} else {
			if (signe&&(fillwith!=' ')) kprintc(psc,signe);
			for (i=0; i<(len-slen) ; i++)
				kprintc(psc,fillwith);
			if ( signe&&(fillwith==' ')) kprintc(psc,signe);
			kprints(psc,str);
	}	}
}

static void vkprintf(subscreen* psc, const char* fmt, va_list args)
{
int len=0;
char fillwith=' ';
int	 placeleft=0;
int	 signe=0;
char buf[100];

	while (*fmt) {
		switch (*fmt) {
	      case '%' :
		  fmt++;
		  while (*fmt) {
	  		switch (*fmt) {
			  case '%' : fmt++; kprintc(psc,'%'); goto leave_printarg;
			  case 's' : 
				vkprintf_str(psc,va_arg(args, char*), len, fillwith, placeleft,0);
				fmt++;
				goto leave_printarg;
			  case 'c' :
				 kprintc(psc,va_arg(args, int)&0xff);
				fmt++;
				goto leave_printarg;
			  case 'x' :
			  case 'X' : {
				char* p=buf;
				unsigned int x= va_arg(args, int);
				int i;
				for (i=0 ; i<7 ; i++, x <<= 4) {
					if ( (x&0xf0000000)!=0 )
						break;
				}
				for ( ; i<8 ; i++, x <<= 4) {
					char c= (x>>28)&0xf;
					if ( c>=10 )
						*p++= c-10+'a';
					else
						*p++= c+'0';
				}
				*p=0;
				vkprintf_str(psc,buf, len, fillwith, placeleft,0);
				fmt++;
				goto leave_printarg;
				}
			  case 'd' :
			  case 'i' : {
				char* p=buf;
				int y=1000000000;
				int x= va_arg(args, int);
				if (x<0) {
					signe = '-';
					x = -x;
				} else if (signe)
					signe = '+';
				
				for (    ; y>=10 ; y /= 10) {
					if ( (x/y)>0 )
						break;
				}
				for (    ; y>=10 ; y /= 10) {
					*p++ = (x / y) + '0';
					x = x % y;
				}
				*p++ = x + '0';
				*p=0;
				if (placeleft)
					fillwith=' ';
				vkprintf_str(psc, buf, len, fillwith, placeleft, signe);
				fmt++;
				goto leave_printarg;
				}
			  case '0' : fmt++; fillwith='0'; continue;
			  case '+' : fmt++; signe= 1; continue;
			  case '-' : fmt++; placeleft= 1; continue;
			  default:
				if ( (*fmt<'1') || ('9'<=*fmt) ) {
					/* ignore char */
					continue;
				}
				/* get number */
				while (*fmt && !((*fmt<'0') || ('9'<*fmt)) ) {
					len = len*10 + *fmt-'0';
					fmt++;
				}
				continue;
			}	}
leave_printarg:
			len=0;
			fillwith=' ';
			placeleft=0;
			signe=0;
			continue;
	    default:
			kprintc(psc,*fmt++);
			continue;
}	}	}

void vga_backspace(subscreen* psc){
	int x, y;

	x = psc->ccol;
	y = psc->cline;

	if (x <= 0){
		if (y <= 0){
			return ;
		}else{
			x = psc->nbcols;
			y --;
		}
	}else{
		x--;
	}
	psc->vidmem[x + (nbcols * y)] = ' ';
	return ;
}
