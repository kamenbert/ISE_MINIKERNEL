#include "process.h"
#include "kernel.h"


//create a process and return a pointer to it
int init_struct_task(struct task_struct* tss,int tty_user, int tty_info, int* exists_entry, struct process p) { 


	if(tss == NULL) {
		kprintf(&(sc_tty_info[tty_info]),"ERROR NULL POINTER");
		return -1;
	}

	if(*exists_entry != 0) {
		tss->state = RUNNING;
	} else {
		tss->state = DEAD;
	}

	tss->ebx = p.ebx; 
	tss->ecx = p.ecx; 
	tss->edi = p.edi; 
	tss->esi = p.esi; 
	tss->ebp = p.ebp; 
	tss->esp = p.esp;
	tss->eip = p.eip;
	tss->es = p.es;
	tss->cs = p.cs;
	tss->ss = p.ss;
	tss->ds = p.ds;
	tss->fs = p.fs;
	tss->gs = p.gs;
	tss->nb_ticks_sleep = 0;
	tss->nb_ticks_alive = 0;
	tss->nb_ticks_active = 0;
	tss->nb_ticks_stopped = 0;
	tss->sleep_length = 0; 
	tss->tty_user = &(sc_tty_user[tty_user]);
	tss->tty_info = &(sc_tty_info[tty_info]);
	tss->buffer_filling = 0;

	return 0;
}

//initiate the task_table, return -1 if failed, 0 if ok.
int init_task_table() {

	/* entries of the proc table */
	int* table_entry[4];
	table_entry[0] = (int*) 0xB000;
	table_entry[1] = (int*) 0xB008;
	table_entry[2] = (int*) 0xB010;
	table_entry[3] = (int*) 0xB018;
	int* exists_entry[4];
	exists_entry[0] = (int*) 0xB004;
	exists_entry[1] = (int*) 0xB00c;
	exists_entry[2] = (int*) 0xB014;
	exists_entry[3] = (int*) 0xB01c;


	//entrées des tss en dur : pas très beau mais bon... :(
	struct process p[4];
	//processus 0
	p[0].ebx = 0; //je ne sais pas trop pour celui-là 
	p[0].ecx = 0;
	p[0].edi = 0;
	p[0].esi = 0;
	p[0].ebp = 0;
	p[0].eip = *table_entry[0];	
	p[0].esp = 0x1FFF; 	
	p[0].es = p[0].ds;
	p[0].cs = 0x4B; 	
	p[0].ss = 0x8B;	
	p[0].ds = 0x6B;	
	p[0].fs = p[0].ds;	
	p[0].gs = p[0].ds;
	//processus 1
	p[1].ebx = 0; //je ne sais pas trop pour celui-là 
	p[0].ecx = 0;
	p[0].edi = 0;
	p[0].esi = 0;
	p[0].ebp = 0;
	p[1].eip = *table_entry[1];	
	p[1].esp = 0x1FFF; 	
	p[1].es = p[1].ds;
	p[1].cs = 0x53; 	
	p[1].ss = 0x93;	
	p[1].ds = 0x73;	
	p[1].fs = p[1].ds;	
	p[1].gs = p[1].ds;
	//processus 2
	p[2].ebx = 0; //je ne sais pas trop pour celui-là 
	p[0].ecx = 0;
	p[0].edi = 0;
	p[0].esi = 0;
	p[0].ebp = 0;
	p[2].eip = *table_entry[2];	
	p[2].esp = 0x1FFF; 	
	p[2].es = p[2].ds;
	p[2].cs = 0x5B; 	
	p[2].ss = 0x7B;	
	p[2].ds = 0x9B;	
	p[2].fs = p[2].ds;	
	p[2].gs = p[2].ds;
	//processus 3
	p[3].ebx = 0; //je ne sais pas trop pour celui-là 
	p[0].ecx = 0;
	p[0].edi = 0;
	p[0].esi = 0;
	p[0].ebp = 0;
	p[3].eip = *table_entry[3];	
	p[3].esp = 0x1FFF; 	
	p[3].es = p[3].ds;
	p[3].cs = 0x63; 	
	p[3].ss = 0x83;	
	p[3].ds = 0xA3;	
	p[3].fs = p[3].ds;	
	p[3].gs = p[3].ds;


	// kprintf(&(sc_tty_user[0]),"Initialisation des process...\n");
	int i;
	for(i=0;i<4;i++){
		task_table[i]= &(task_struct_table[i]);
		if( init_struct_task(task_table[i],i,i,exists_entry[i],p[0]) < 0 ) {
			return -1;
		}
		//kprintf(task_table[i]->tty_info,"\n%d - created",i+1); //ne sert plus a rien
	}
	return 0;
}

//initiate all variables from process.h, return -1 if failed, 0 if ok. 
int init_process() {
	int ret = init_task_table();
	if(ret==-1) {
		return -1;
	}
	focus = task_table[0]; // current focus
	printborder(focus->tty_info,2);//bordur vert 
	printborder(focus->tty_user,2);
	current = task_table[0]; // current active process
	focus_ = 0;
	current_ = 0;
	return 0;
}

//TODO
//Displays infos updated.
void maj_info_process(struct task_struct* tss){
	return;
}


void scheduler() {
	int found_next = 0; 
	int i;
	struct task_struct* previous;
	for(i = 0; i < 4; i++) {
		switch(task_table[i]->state) {
			case DEAD :

				kprintf(task_table[i]->tty_info, "\n DEAD");		
				break;

			case SLEEPING :
				task_table[i]->nb_ticks_sleep++;
				task_table[i]->nb_ticks_alive++;
				task_table[i]->sleep_length--;
				if(task_table[i]->sleep_length < 1 && found_next == 0) {
					found_next = 1;
					current_ = i;
					task_table[i]->state = RUNNING;
					task_table[i]->nb_ticks_active++;
				} else {
					if(task_table[i]->nb_ticks_alive%10==0) {
						kprintf(task_table[i]->tty_info, "\n SLEEPING:%d",task_table[i]->sleep_length);			
					}
				}	
				break;

			case RUNNING :
				task_table[i]->nb_ticks_active++;
				task_table[i]->nb_ticks_alive++;
				if(found_next == 0) {
					found_next = 1;
					current_ = i;
				}
				if(task_table[i]->nb_ticks_alive%10==0) {
					kprintf(task_table[i]->tty_info, "\n RUNNING:%d",(task_table[i]->nb_ticks_active)/10);			
				}
				break;


			case STOPPED : 
				task_table[i]->nb_ticks_stopped++;
				task_table[i]->nb_ticks_alive++;
				if(task_table[i]->nb_ticks_alive%10==0) {
					kprintf(task_table[i]->tty_info, "\n STOPPED:%d",(task_table[i]->nb_ticks_stopped)/10);			
				}
				break;

			default :
				break;
		}
	}
	previous = current;
	current = task_table[current_];
	_switch_proc(previous, current);
}

