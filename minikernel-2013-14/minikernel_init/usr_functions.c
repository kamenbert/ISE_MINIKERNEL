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


/* Sans sécurité, et débordement */
int scanf( const char* fmt,...){
va_list args;  
va_start(args, fmt);
char*	buf;// =NULL; 
int 	i=0;
char	c;
int 	signe=0;
int	nb=0;
while (*fmt!='\0') {//printf("WHILE 1 : (%c)\n", *fmt);
switch (*fmt) {
case '%' :	
 	fmt++;	
	while (*fmt) {	//printf("WHILE2 : (%c)\n", *fmt);
   	switch(*fmt){   
		case 'c':
			buf =  va_arg(args, char*);			
			c=' ';
			while(c=read()){
				if(c>32 && c<255) break;
			}
              	        if (c> 32 && c<127) {
				*(buf) =c;//printf("{ ___ (%c/%c)___ } \n",*buf, *(buf+1));
				i++;
			}
			fmt++; 
			goto next;

		case 'd': //printf("START D\n\n");

			//void* memcpy(void* dest, const void* src, size_t n)
			int* nAcc= va_arg(args, int*);
			int id=i; //Pour une entrée à plusieurs format, on renvoie le nombre de caractères déjà écrit avant l'entrée pour ce format s'il y a un dépassement INT_MAX
			int max=1000000000;
			
			//int max= INT_MAX;
			while(c=read()){
				if(c== 32) continue;
				else if (signe ==0 &&( c == 43||c== 45) ){ //printf("Vérif SIGNE\n");
						if(c==45) signe = -1;
						//i++;
	 			}
				else if(c>=48 && c<=59) break;
			}
			
			do{ 					
				if (c>=48 && c<=59){	
					if(nb *10 +c-48 < max) {
						nb = nb*10 + c-48;
					}
					else{ 	//nb=INT_MAX;
						nb=0;
						i= id;
						break;
					}
					i++;
				}else break;
			}while(c=read());
			if(signe == -1) *nAcc = -nb;			
			else *nAcc= nb;
			*(nAcc+1) = '\0';
			fmt++;
			nb=0;
			c=' ';
			signe=0;
			goto next;
		case 's':	/* Espace alloué : en cas de dépassement mémoire : impossible de sizeof
				Cas de nombre de paramètres différents du nombre de format indiqué: impossible de savoir à l'avance le nombre de paramètres */
			//Pas de protection
			buf =  va_arg(args, char*); 
			int j=0;
			c=' ';
			while(c=read()){ 
				if(c>32 && c<127) break;
			}
              	       do{	if(c!='\0'&& c> 32 && c<127 ) {
					buf[j]= c;//printf("{(%c) }",c);
					i++;
					j++;
				}
				else break;
			} while(c=read());
			buf[j]='\0';
			fmt++;
			goto next;
		default: fmt++; goto next;
	}// fin switch
	}// fin while
default :  
	if (*fmt){//printf("\nDEF :%c\n",*fmt); 
		fmt++;
		continue;}
	else goto end;
} // fin switch
next: continue; 
//fmt++;
} // fin while
end :
va_end(args);
return i;
}                               

	


