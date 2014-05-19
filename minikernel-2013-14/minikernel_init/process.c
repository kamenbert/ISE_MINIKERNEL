#include "process.h"
#include "kernel.h"


//create a process and return a pointer to it
int init_struct_task(struct task_struct* tss,int tty_user, int tty_info) { 

	if(tss == NULL) {
		kprintf(&(sc_tty_info[tty_info]),"ERROR NULL POINTER");
		return -1;
	}
	tss->state = RUNNING;
	tss->eax = tss->ebx = tss->ecx = tss->edi = tss->esi = tss->ebp = tss->esp = tss->eip = 0;
	tss->es = tss->cs = tss->ss = tss->ds = tss->fs = tss->gs = 0;
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
	// kprintf(&(sc_tty_user[0]),"Initialisation des process...\n");
	int i;
	for(i=0;i<4;i++){
		task_table[i]= &(task_struct_table[i]);
		if( init_struct_task(task_table[i],i,i) < 0 ) {
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
	printborder(focus->tty_info,3); //blue border
	printborder(focus->tty_user,3);
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
		current = task_table[current_];
	}
}

