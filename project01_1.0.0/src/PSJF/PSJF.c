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

//void PSJF(Queue_T* P_Queue, int Process_Num);
//void GetTime(pid_t pid);

int pi;

/*
int main(){
	//printf("here01");
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
    printf("%s %d\n",P_Queue[pi].pname, pid);
}*/

void PSJF(Queue_T* P_Queue, int Process_Num)
{
  int pi= 0;
  int i;
  int active_num = 0;
  int ready_num = 0;
  int time = 0;
  int tmp = 0;
  pid_t pid1;

  for (i = 0; i<Process_Num; i++)
  {
  	  P_Queue[i].active = 1;
	  P_Queue[i].start_time = 0;
	  P_Queue[i].remaining_time = P_Queue[i].exec_time;
	  P_Queue[i].end_time = -1;

  }
  //if (active_num == 0) return;
  active_num = Process_Num;
  pi = 0;

  for(i =0; i < Process_Num; i++){
	if(P_Queue[i].remaining_time > P_Queue[tmp].remaining_time) tmp = i;//P_Queue[i].remaining_time;
  }
// printf("the longest job is%d, which need %d\n\n", tmp, P_Queue[tmp].remaining_time);
  while (1)
  {
  	  //printf("time = %d \n", time);
	 if(P_Queue[pi].active == 0){
	 /*
	  for(i = 0; i < Process_Num; i++){
	  	if(P_Queue[i].active == 1 ) pi = i;

	  }
	  */
	  pi = tmp;
	  }
	  for (i = 0; i < Process_Num; i++) {
		 if ((P_Queue[i].ready_time <= time) && (P_Queue[i].active == 1) && (P_Queue[i].remaining_time <= P_Queue[pi].exec_time)){
			pi = i;
            //printf("%s %d\n",P_Queue[pi].pname, pi);

			//	printf("now it is %d chance\n", pi);
		}
	  }

	  if (P_Queue[pi].start_time == 0 && P_Queue[pi].ready_time < time) { //start_time records if a process is Fed or not;0 is unforked
	  	  //printf("%d forked\n", pi);
		  P_Queue[pi].end_time = fork(); // end_time record the PID of each child process
		  callSystemTime(getpid(),OSPROJ_PROC_START);
		  if(P_Queue[pi].end_time < 0) printf("error");
		  P_Queue[pi].start_time = 1;
	  }

	  if (P_Queue[pi].start_time == 1 && P_Queue[pi].active == 1 && P_Queue[pi].ready_time <= time) {
		  if (P_Queue[pi].end_time == 0) {//CHILD PART
			  for (i = 0; i < P_Queue[pi].exec_time; i++) {
				 // printf("in child\n");
				  kill(getpid(), SIGSTOP); //suspend child process
				  volatile unsigned long j;
				  for(j = 0; j < 1000000UL ;j++);  // do million loops
				  //printf(".");
				  //kill(getppid(), SIGCONT);
				  //printf(". %d. ", P_Queue[pi].remaining_time);
			  }
			  //printf("%d is done\n ", pi);
			  //printf("here");
			  exit(0);//end of a child process
		  }
		  else { //PARENT PART
			 // printf("*");
			  kill(P_Queue[pi].end_time, SIGCONT);//signal child process to go on
			 // kill(getpid(), SIGSTOP );			  //suspend parent process
		  	 // printf("executed %d, remain %d\n", pi, P_Queue[pi].remaining_time);
		  }
		  P_Queue[pi].remaining_time--;
		  time++;
		 // printf("remain-1\n");
	  }
	  else {volatile unsigned long j; for(j = 0; j < 1000000UL ;j++);time++;}
	  if (P_Queue[pi].remaining_time < 1&& P_Queue[pi].active == 1){
	  	  //printf("process no. %d  ", pi);
		  //GetTime(P_Queue[pi].end_time);
        printf("%s %d\n",P_Queue[pi].pname, P_Queue[pi].end_time);
        callSystemTime(pi,OSPROJ_PROC_END);
		  //printf("`time = %d\n", time);
		//  for(i=0; i < Process_Num; i++){
		//  	if(P_Queue[i].active)printf("%d  ", i);
		//  }
		  //printf("pi = %d \n", pi);
		 // for(i=0; i < Process_Num; i++){
		 // 	printf("%d ", P_Queue[i].active);
		 // }
		  P_Queue[pi].active = 0;
	  }
	  active_num = 0;
	  for (i = 0; i<Process_Num; i++)
	  {
		  if (P_Queue[i].active == 1) active_num++;
	  }
	  if (active_num <1) return;

	 }

  }//end of function
