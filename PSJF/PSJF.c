
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
#include "PSJF.h"

#define POLICY_LEN 10

char Policy[POLICY_LEN];
int Process_Num = 0;

void PSJF(Queue_T* P_Queue, int Process_Num);
void GetTime(pid_t pid);

int main(){
	printf("here01");
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
	PSJF(P_Queue, Process_Num);
	//printf("here01\n");
}

void GetTime(pid_t pid)
{
    clock_t t;
    t = clock();
    printf("[Project1] ");
    printf("pid:%d ", pid);
    printf("time: %lu\n",t);
}


void PSJF(Queue_T* P_Queue, int Process_Num)
{
  int clock = 0;
  int pi= 0;
  int i;
  int active_num = 0;
  int ready_num = 0;

  pid_t pid1;

  for (i = 0; i<Process_Num; i++)
  {
  	  P_Queue[i].active = 1;
	  if (P_Queue[i].active == 1) active_num++;
	  P_Queue[i].start_time = 0;
	  P_Queue[i].remaining_time = P_Queue[i].exec_time;

  }
  //if (active_num == 0) return;

  while (1)
  {
  	  printf("here02\n");
	  for (i = 0; i < Process_Num; i++) {
			if ((P_Queue[i].active == 1) && (P_Queue[i].remaining_time < P_Queue[pi].remaining_time)){
				pi = i;
			}
	  }
	  
	  if (P_Queue[pi].start_time == 0) { //start_time records if a process is forked or not;0 is unforked
		  P_Queue[pi].end_time = fork(); // end_time record the PID of each child process
		  if(P_Queue[pi].end_time < 0) printf("error");
		  P_Queue[pi].start_time = 1;    
	  }
	  
	  if (P_Queue[pi].remaining_time > 0) {
		  if (P_Queue[pi].end_time == 0) {//CHILD PART
			  for (i = 0; i < P_Queue[pi].exec_time; i++) {
				  kill(getpid(), SIGSTOP ); //suspend child process
				  volatile unsigned long j;
				  for(j = 0; j < 1000000UL ;j++);  // do million loops
				  kill(getppid(), SIGCONT);
				  printf(". %d. ", P_Queue[pi].remaining_time);
			  }
			  exit(0);//end of a child process
		  }
		  else { //PARENT PART
			  kill(P_Queue[pi].end_time, SIGCONT);//signal child process to go on
			  kill(getpid(), SIGSTOP );			  //suspend parent process
		  	  printf("executed %d, remain %d\n", pi, P_Queue[pi].remaining_time);
		  }
		  P_Queue[pi].remaining_time--;
	  }
	  if (P_Queue[pi].remaining_time < 1){
	  	  GetTime(P_Queue[pi].end_time);
		  P_Queue[pi].active = 0;
	  }

	  for (i = 0; i<Process_Num; i++)
	  {
		  if (P_Queue[i].active == 1) active_num++;
	  }
	  if (active_num == 0) return;
	  
	  }
  }//end of function
