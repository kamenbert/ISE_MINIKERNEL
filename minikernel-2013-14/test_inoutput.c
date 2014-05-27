#include <libk.h>

int main(){
	write('B');	
	write('o');	
	write('n');	
	write('j');	
	write('o');	
	write('u');	
	write('r');	
	write('\n');	
	char input;
	while(1){
		input = read();
		write(input);
		write('\n');
	}
}
