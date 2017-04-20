#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#define POLICY_LEN 10
#define MAX_QUEUE_LEN 10

int FIFO();
int SJF();
int RR();
int PSJF();

/*
GetTime(pid_t pid)
{
    struct timespec ts_start;
   // getnstimeofday(&ts_start);
   // printk("[Project1] ");
   // printk("%d ", pid);
   // printk("%lu\n", ts_start);
    return;
}
*/

// User function
void GetTime(pid_t pid)
{
    clock_t t;
    t = clock();
    printf("[Project1] ");
    printf("pid:%d ", pid);
    printf("time: %lu\n",t);
    int i;
    for(i = 0; i < 1000000; i++)
    {
        int x = 90;
    }
}

struct Queue_T
{
      char pname[POLICY_LEN];
      int ready_time;
      int exec_time;
      int remaining_time;
      int active;
      int start_time;
      int end_time;
} P_Queue[MAX_QUEUE_LEN];

struct Queue_T RR_Queue[MAX_QUEUE_LEN];

char Policy[POLICY_LEN];
int Process_Num = 0;

int main()
{
//Input
  FILE *fp;
  fp = fopen("input.txt","r");
  if (fp == NULL)
  {
     printf("file open error\n");
     exit(1);
  }
  if (fscanf(fp,"%s %i",&Policy,&Process_Num)==EOF)
  {
     printf("data input error\n");
     exit(1);
  }
   //printf("process_num: %i\n",Process_Num);
  for (int i=0; i<Process_Num; i++)
  {
     if(fscanf(fp,"%s %i %i",&P_Queue[i].pname,&P_Queue[i].ready_time,&P_Queue[i].exec_time)==EOF)
      {
        printf("data input error\n");
        exit(1);
      }
    P_Queue[i].active = 1;
    P_Queue[i].remaining_time =  P_Queue[i].exec_time;
   }
  fclose(fp);
   if(strcmp(Policy, "FIFO")==0) FIFO();
   //if(strcmp(Policy, "RR")==0) RR();
   //if(strcmp(Policy, "SJF")==0) SJF();
   //if(strcmp(Policy, "PSJF")==0) PSJF();
   /*
   struct sched_param param;
      int maxpri;
      maxpri = sched_get_priority_max(SCHED_FIFO);//max priority can get
      if(maxpri == -1)
      {
            perror("sched_get_priority_max() failed");
            exit(1);
      }
      param.sched_priority = maxpri;
      if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1)
     {
            perror("sched_setscheduler() failed");
            exit(1);
     }
     wait();
  }*/
  return 0;
} // main

int FIFO()
{
  int clocktime = 0;
  int pi = 0;
  int i;
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
    {printf("terminated\n");return 0;}

    for (i=pi+1; i<Process_Num; i++)
    {
       if ((P_Queue[i].ready_time < P_Queue[pi].ready_time) && (P_Queue[i].active == 1) )
       {
           pi = i;
       }
    }

    if(P_Queue[pi].ready_time > clocktime)
    {
      for (i=0; i<P_Queue[pi].ready_time - clocktime; i++)
      {
       // do million loops
      }
      clocktime = P_Queue[pi].ready_time;
    }

    pid1 = fork();
    if (pid1 == 0) //child
    {
      // do P_Queue[pi].exec_time million loops
      printf("child\n");
      volatile unsigned long ii;
      for (int j=0; j<P_Queue[pi].exec_time; j++)
      {
         for( ii=0;ii<1000000UL;ii++);
      }
    }
    else // parent
    {
      /*
      //set highest priority for pi
      struct sched_param param;
      int maxpri = 0;
      maxpri = sched_get_priority_max(SCHED_FIFO);//max priority can get
      if(maxpri == -1)
      {
            perror("sched_get_priority_max() failed");
            exit(1);
      }
      param.sched_priority = maxpri;
      if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1)
     {
            perror("sched_setscheduler() failed");
            exit(1);
     }
     */
      //GetTime(pi);
      wait();
      //GetTime(pi);
      clocktime = clocktime + P_Queue[pi].exec_time;
      printf("%s %d %d\n",P_Queue[pi].pname, pid1, clocktime);
      P_Queue[pi].active = 0;
    }
   } // while
 }

int SJF()
{
  int clock = 0;
  int pi= 0;
  int i;
  int active_num = 0;
  int ready_num = 0;
  int first_ready = 0;
  pid_t pid1;
  // find next ready process with shortest exec time pi until queue is empty
  while (1)
  {
    active_num = 0;
    ready_num = 0;
    for (i=0; i<Process_Num; i++)
    {
       if (P_Queue[i].active == 1) active_num++;
    }
    if (active_num == 0) return 0;
    for (i=0; i<Process_Num; i++)
    {
       if((P_Queue[i].active ==1) && (P_Queue[i].ready_time <= clock)) ready_num++;
    }
    if (ready_num == 0) // case 1: find first ready
    {
       //find first ready process
       first_ready = 0;
       for (i=0; i<Process_Num; i++)
       {
         if((P_Queue[i].active==1) &&
            (P_Queue[i].ready_time < P_Queue[first_ready].ready_time))
         {
             first_ready = i;
         }
       }

      if(P_Queue[first_ready].ready_time>clock)
      {
        for (i=0; i<P_Queue[first_ready].ready_time - clock; i++)
        {
       // do million loops
        }
        clock = P_Queue[first_ready].ready_time;
        pi = first_ready;
      }
    }
    else // case 2
    {
      for (i=0; i<Process_Num; i++)
      {
       if ((P_Queue[i].active == 1)&& (P_Queue[i].ready_time<=clock))
       {
          pi = i;
          break;
       }
      }
      for (i=pi+1; i<Process_Num; i++)
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
      printf("child\n");
      // do P_Queue[pi].exec_time million loops
    }
    else // parent
    {
      //set highest priority for pi
      GetTime(pi);
      wait();
      GetTime(pi);
      clock = clock+P_Queue[pi].exec_time;
      printf("%s %d %d\n",P_Queue[pi].pname, pid1, clock);
      P_Queue[pi].active = 0;
    }
   } // while
}
/*
void RR()
{
  int clock = 0;
  int pi= 0;
  int i;
  pid_t pid1;
  int time_quantum = 500;
  int RR_Queue_Num;

  // find next ready process with remaining time pi until queue is empty

  RR_Queue_Num =0;
  while (1)
  {
    // move ready process to RR queue
    for (i=0; i<Process_Num; i++)
    {
       if ((P_Queue[i].active == 1) && (P_Queue[i].ready_time <= clock))
       {
          P_Queue[i].active = 0;
          RR_Queue[RR_Queue_Num].panme= P_Queue[i].pname;
          RR_Queue[RR_Queue_Num].ready_time = P_Queue[i].ready_time;
          RR_Queue[RR_Queue_Num].exec_time = P_Queue[i].exec_time;
          RR_Queue[RR_Queue_Num].remaining_time = P_Queue[i].remaining_time;
          RR_Queue[RR_Queue_Num].active = 1;
          RR_Queue_Num++
       }
    }

    pi = 0;
    for (i=0; i<Process_Num; i++)
    {
       if ((P_Queue[i].active == 1) && (P_Queue[i].ready_time <= clock))

    }
     if (i == Process_Num) // queue is empity
    {
      return;
    }
    for (i=pi+1; i<Process_Num; i++)
    {
       if ((P_Queue[i].ready_time < P_Queue[pi].ready_time) &&
           (P_Queue[i].active == 1) )
       {
           pi = i;
       }
    }

    if(P_Queue[pi].ready_time>clock)
    {
      for (i=0; i<P_Queue[pi].ready_time - clock; i++)
      {

    // do million loops
      }
      clock = P_Queue[pi].ready_time;
    }

    //pid1 = fork();
    pid1 = 1;
    if (pid1 == 0) //child
    {

      // do ((abs(P_Queue[pi].remaining_time - time_quantum)) % time_quantum)million loops
    }
    else // parent
    {
      //set highest priority for pi

      GetTime(pid1);
      //wait ();
      GetTime(pid1);
      if (P_Queue[pi].remaining_time <= time_quantum)
      {
         clock = clock+P_Queue[pi].remaining_time;
         P_Queue[pi].remaining_time = 0;
         P_Queue[pi].active = 0;
      }
      else
      {
         clock = clock+time_quantum;
         P_Queue[pi].remaining_time = P_Queue[pi].remaining_time - time_quantum;
      }
      printf("%s %d %d\n",P_Queue[pi].pname, pid1, clock);
    }
   } // while

 }

/*
void PSJF()
{
  int clock = 0;
  int pi= 0;
  int i;
  int active_num = 0;
  int ready_num = 0;
  int first_ready = 0;
  pid_t pid1;


  // find next ready process with shortest exec time pi until queue is empty

  while (1)
  {

    for (i=0; i<Process_Num; i++)
    {
       if (P_Queue[i].active == 1) active_num++;
    }
    if (active_num == 0) return;


    for (i=0; i<Process_Num; i++)
    {
       if (P_Queue[i].ready_time <= clock) ready_num++;
    }
    if (ready_num == 0) // case 1: find first ready
    {
       //find first ready process
       first_ready = 0;
       for (i=0; i<Process_Num; i++)
       {
         if((P_Queue[i].active==1) &&
            (P_Queue[i].ready_time < P_Queue[first_ready].ready_time))
         {
             first_ready = i;
         }
       }

      for (i=0; i<P_Queue[first_ready].ready_time - clock; i++)
      {

       // do million loops
       }
       clock = P_Queue[first_ready].ready_time;
       pi = first_ready;
    }
    else // case 2
    {

      for (i=0; i<Process_Num; i++)
      {
       if ((P_Queue[i].active == 1)&& (P_Queue[i].ready_time<=clock))
       {
          pi = i;
       }
      }
      for (i=pi; i<Process_Num; i++)
      {
        if ((P_Queue[i].ready_time < P_Queue[pi].ready_time) &&
            (P_Queue[i].remaining_time < P_Queue[pi].remaining_time) &&
            (P_Queue[i].active == 1) )
        {
           pi == i;
        }
      }
    }
*/
