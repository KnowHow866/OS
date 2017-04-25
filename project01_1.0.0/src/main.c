#include "SJF/SJF.h"
#include "FIFO/FIFO.h"
#include "RR/RR.h"
#include "PSJF/PSJF.h"
#include "GLOBAL.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>

int main()
{
  scanf("%s",&Policy);
  scanf("%d",&Process_Num);
  Queue_T* P_Queue = (Queue_T*)malloc(sizeof(Queue_T)*Process_Num);
  for(int i = 0; i < Process_Num; i++){
		scanf(" %s %d %d",
			&(P_Queue[i].pname),
			&(P_Queue[i].ready_time),
			&(P_Queue[i].exec_time));
	}

  for (int i=0; i<Process_Num; i++)
  {
    scanf("%s %i %i",&P_Queue[i].pname,&P_Queue[i].ready_time,&P_Queue[i].exec_time);
    P_Queue[i].active = 1;
    P_Queue[i].remaining_time =  P_Queue[i].exec_time;
   }

   if(strcmp(Policy, "FIFO")==0) FIFO(P_Queue, Process_Num);
   if(strcmp(Policy, "RR")==0) RR(P_Queue, Process_Num);
   if(strcmp(Policy, "SJF")==0) SJF(P_Queue, Process_Num);
   if(strcmp(Policy, "PSJF")==0) PSJF(P_Queue, Process_Num);

  return 0;
} // main
