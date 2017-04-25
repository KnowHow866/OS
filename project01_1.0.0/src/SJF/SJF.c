#include "SJF.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
int SJF(Queue_T* P_Queue, int Process_Num)
{
  int clock = 0;
  int CurMin = 0;
  int pi = 0;
  int i=0,z=0;
  int active_num = 0;
  int ready_num = 0;
  int first_ready = 0;
  pid_t pid1;
  // find next ready process with shortest exec time pi until queue is empty
  for(z=0;z<Process_Num;z++)
  {
    active_num = 0;
    ready_num = 0;
    for (i=0;i<Process_Num;i++)
    {
       if (P_Queue[i].active==1) active_num++;
    }
    //printf("active_num = %i\n",active_num);
    if (active_num == 0) return 0;

    for (i=0; i<Process_Num; i++)
    {
       if((P_Queue[i].active==1) && (P_Queue[i].ready_time <= clock)) ready_num++;
    }
    //printf("ready_num = %i\n",ready_num);

    if (ready_num == 0) // case 1: find first ready
    {
       //find first ready process
       first_ready = 0;
       for (i=0; i<Process_Num; i++){
           if(P_Queue[i].ready_time <= CurMin && P_Queue[i].active==1){
                //printf("%i < %i\n",P_Queue[i].ready_time,CurMin);
                first_ready = i;
                CurMin = P_Queue[i].ready_time;
           }
       }
       for (i=0; i<Process_Num; i++){
            if(P_Queue[i].ready_time > CurMin)CurMin = P_Queue[i].ready_time;}

      if(P_Queue[first_ready].ready_time >= clock)
        clock = P_Queue[first_ready].ready_time;
      pi = first_ready;
      //printf("first ready is %i\n",first_ready);
      //printf("now the clock is: %i\n",clock);
    }
    else // case 2
    {
      for (i=0; i<Process_Num; i++)
      {
       if ((P_Queue[i].active == 1) && (P_Queue[i].ready_time<=clock))
       {
          pi = i;
          break;
       }
      }
      for (i = pi+1; i<Process_Num; i++)
      {
        if ((P_Queue[i].exec_time < P_Queue[pi].exec_time) &&
             (P_Queue[i].ready_time<=clock) &&
            (P_Queue[i].active == 1) )
        {pi = i;}
      }
    } // end else
    pid1 = fork();
    if(pid1 == 0) //child
    {
      //printf("%s %d\n",P_Queue[pi].pname, pid1);
      callSystemTime(getpid(),OSPROJ_PROC_START);
      // do P_Queue[pi].exec_time million loops
      Pause(P_Queue[pi].exec_time);
      exit(0);
    }
    else //parent
    {
      //set highest priority for pi
      wait(0);
      clock = clock + P_Queue[pi].exec_time;
      printf("%s %d\n",P_Queue[pi].pname, pid1);
      callSystemTime(pid1,OSPROJ_PROC_END);
      P_Queue[pi].active = 0;
      active_num--;
    }
   } // while
}
