#include "stdarg.h"

static void printf_str(const char* str, int len, char fillwith, int placeleft, int signe){
	int i;
        if(len==0){
		if(signe) write(signe);
	        write_str(str);

	}
        else {
                int slen=strlen(str) + (signe ? 1 : 0);
                if (slen >= len) {
                        if (signe) write(signe);
                                write_str(str);
                        } else if (placeleft) {
                                if (signe) write(signe);
                                write_str(str);
                                for (i=0; i<(len-slen) ; i++) {
                                        write(fillwith);
                                }
                        } else {
                                if (signe&&(fillwith!=' ')) write(signe);
                                for (i=0; i<(len-slen) ; i++)
                                        write(fillwith);
                                        if ( signe&&(fillwith==' ')) write(signe);
                                        write_str(str);
                        }
        }
}


void printf ( const char* fmt,...){
int 	len=0;
char 	fillwith=' ';
int     placeleft=0;
int     signe=0;
char	buf[100];
va_list args;  
va_start(args, fmt); /* Initialisation de la liste args */
while (*fmt) {
switch (*fmt) {
case '%' :   /* Si le caractère est "%", on regarde le caractère suivant */    
        fmt++;
        while (*fmt) {
                switch (*fmt){
		case 'c':
			write(va_arg(args,int)&0xff);
			fmt++;
			goto leave_printarg;		

                case 's' : 
			write_str(va_arg(args,char*));                        
                        fmt++;
                        goto leave_printarg;

		case 'd' :

		case 'i' : {
			char* p=buf;
			int y=1000000000;
			int x= va_arg(args, int);
			if (x<0) {
				signe = '-';
				x = -x;
			} else if (signe) signe = '+';
			for ( ; y>=10 ; y /= 10) {
				if ( (x/y)>0 ) break;
			}
			for ( ; y>=10 ; y /= 10) {
				*p++ = (x / y) + '0';
				x = x % y;
			}
			*p++ = x + '0';
			*p=0;
			if (placeleft)
			fillwith=' ';
			printf_str(buf, len, fillwith, placeleft,signe);
			fmt++;
			goto leave_printarg;
			  }

		case '0' : fmt++; fillwith='0'; continue;
		case '+' : fmt++; signe= 1; 	continue;
		case '-' : fmt++; placeleft= 1; continue;

 		default:
                        if ( (*fmt<'1') || ('9'<=*fmt) ) {
                        /* On ignore le reste des caractères non numériques */
                        continue;
                        }
                        /* Si *fmt est un chiffre entre 0 et 9 pour par exemple l'affichage de "%5d ou %12d" */
                        while (*fmt && !((*fmt<'0') || ('9'<*fmt)) ) {
                                len = len*10 + *fmt-'0';
                                fmt++;
                        }
                        continue;
            } /* Fin switch */
        } /* Fin while */
        leave_printarg:
        len=0;
        fillwith=' ';
        placeleft=0;
        signe=0;
        continue;
default : 
	write(*fmt);
	fmt++;
	continue;
}
}
va_end(args);
}


#if 0
int scanf_usr( const char* fmt, char* addr){

char*	buf=addr;// = malloc(BUF_LEN*sizeof(char));
int 	i=0;
char	c;
int 	signe=0;
while (*fmt) {
switch (*fmt) {
case '%' :	
 	fmt++;		
   	switch(*fmt){     
		case 'c' : 
			c=read(); //read(1,&c,1);
              	        if ( c> 32 && c<127) {
				*buf = c;
				i++;// Si un seul format dans scanf
			}
			goto fin;
		case 'd':	
			//while(read(1,&c,1)){
			if (c = read()){			
				if (signe ==0 &&( c =='+'||c== '-') ){
					signe = 1;
					*(buf+i) = c;
					i++;
	 				}
				if (c>'0' && c<='9'){	
					*(buf+i) = c; /*printf("Nb : %d\n", (int) c); //COONVERTIR EN INT : ATOI stdlib, ou garder en char ? */
					i++;
				}else break;
			}
			goto fin;		
		case 's':

			/* Condition de fin , taille allocation mémoire
			Probleme de dépassement de mémoire ?*/aprintf("\n\n");
			//while(read(1,&c,1)){
			while(c= read()){			
				if ( c> 32 && c<127){ /* Problème avec les accents*/ //printf("%c",c);
					*(buf+i) = c;
					i++;
				}
				else break;
			}
			goto fin;
			
		default: break;
	}// fin switch

default : fmt++;
} // fin switch
} // fin while

fin:
*(buf+i) = '\0';
buf = NULL;
return i;
}

#endif
