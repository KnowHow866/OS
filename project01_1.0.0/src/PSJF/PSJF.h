#ifndef PSJF_H
#define PSJF_H

#define _POSIX_SOURCE
#include "../GLOBAL.h"
//void PSJF(Queue_T* P_Queue, int Process_Num);
/*
struct Queue_T
{
      char pname[10];
      int ready_time;
      int exec_time;
      int remaining_time;
      int active;
      int start_time;
      int end_time;
} P_Queue[10];

typedef struct Queue_T Queue_T;
*/
Queue_T P_Queue[10];
void PSJF(Queue_T* P_Queue, int Process_Num);

#endif
