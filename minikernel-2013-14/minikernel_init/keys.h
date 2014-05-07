#ifndef KEYS_H
#define KEYS_H

// Tableau de correspondace scancode -> touches
unsigned char keys[KBD_KEY_NUMBER]={
'\0',								// 01 : echap
'1' , '2' , '3', '4', '5', '6', '7', '8', '9', '0', '\0' ,'+',	// 02 - DH
'\0',								// 0E : backsapce
'	',							// 0F : tabulation
'a', 'z', 'e', 'r', 't', 'y', 'u', 'i' ,'o' ,'p', '^', '$',	// 10 - 1B
'\n',								// 1C : entrer
'\0',								// 1D : ctrl 
'q', 's','d' ,'f', 'g', 'h', 'j', 'k', 'l', 'm', '\0','\0', 		// 1E - 28
'\0',								// 2A : shift gauche
'*','w','x', 'c', 'v', 'b', 'n', ',', ';', ':', '!',		// 2B - 35
'\0',								// 36 : shift droit
'*',								// 37 : * du pave numerique
'\0',								// 38 : alt (gauche ou droit)
' ',								// 39 : espace
'\0',								// 3A : caps_lock
'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',		// 3B - 44 : F1 - F10
'\0',								// 45 : verr_num
'\0',								// 46 : arret_defil
'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', // 47 - 53
'\0', '\0',							// 54 - 55 : rien ?
'<',								// 56
'\0', '\0'							// 57 - 58 : F11 - F12
};

unsigned char highKeys[KBD_KEY_NUMBER]={
'\0',								// 01 : echap
'&' , 'e' , '\"', '\'', '(', '-', 'e', '_', 'c', 'a', ')' ,'=',	// 02 - DH
'\0',								// 0E : backsapce
'\t',							// 0F : tabulation
'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I' ,'O' ,'P', '\0', '\0',	// 10 - 1B
'\n',								// 1C : entrer
'\0',								// 1D : ctrl 
'Q', 'S','D' ,'F', 'G', 'H', 'J', 'K', 'L', 'M', '%','\0', 		// 1E - 28
'\0',								// 2A : shift gauche
'\0','W','X', 'C', 'V', 'B', 'N', '?', '.', '/', '\0',		// 2B - 35
'\0',								// 36 : shift droit
'*',								// 37 : * du pave numerique
'\0',								// 38 : alt (gauche ou droit)
' ',								// 39 : espace
'\0',								// 3A : caps_lock
'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',		// 3B - 44 : F1 - F10
'\0',								// 45 : verr_num
'\0',								// 46 : arret_defil
'7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', // 47 - 53
'\0', '\0',							// 54 - 55 : rien ?
'>',								// 56
'\0', '\0'							// 57 - 58 : F11 - F12
};


#endif
