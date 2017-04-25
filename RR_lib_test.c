#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>
#include "RR_lib.h"

#define POLICY_LEN 10

char Policy[POLICY_LEN];
int Process_Num = 0;

int main(){
	scanf("%s",&Policy);
	scanf("%d",&Process_Num);
	Queue_T* P_Queue = (Queue_T*)malloc(sizeof(Queue_T)*Process_Num);
	for(int i = 0; i < Process_Num; i++){
		scanf(" %s %d %d",
			&(P_Queue[i].pname), 
			&(P_Queue[i].ready_time),
			&(P_Queue[i].exec_time));
	}  

	//FIFO(P_Queue, Process_Num);
	RR(P_Queue, Process_Num);
}