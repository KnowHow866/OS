#include "FIFO.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
int FIFO(Queue_T* P_Queue, int Process_Num)
{
  int clocktime = 0;
  int pi = 0;
  int i = 0,j = 0;
  pid_t pid1;
  // find next ready process pi until queue is empty
  while(1)
  {
    for (i=0; i<Process_Num; i++)
    {
       if (P_Queue[i].active == 1)
       {
          pi = i;
          break;
       }
    }
    if (i == Process_Num) // queue is empty
    {return 0;}

    for (i = pi+1; i<Process_Num; i++)
    {
       if ((P_Queue[i].ready_time < P_Queue[pi].ready_time) && (P_Queue[i].active == 1) )
       {
           pi = i;
       }
    }

    if(P_Queue[pi].ready_time > clocktime)
    {
      Pause(P_Queue[pi].ready_time - clocktime);
      clocktime = P_Queue[pi].ready_time;
    }

    pid1 = fork();
    if (pid1 == 0) //child
    {
      //printf("%s %d\n",P_Queue[pi].pname, pid1);
      callSystemTime(getpid(),OSPROJ_PROC_START);
      // do P_Queue[pi].exec_time million loops
      volatile unsigned long ii;
      Pause(P_Queue[pi].exec_time);
      exit(0);
    }
    else // parent
    {
      wait(0);
      printf("%s %d\n",P_Queue[pi].pname, pid1);
      callSystemTime(pid1,OSPROJ_PROC_END);
      P_Queue[pi].active = 0;
    }
   } // while
 }
