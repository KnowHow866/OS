#ifndef _GLOBAL_H
#define _GLOBAL_H
#define POLICY_LEN 10
#define MAX_QUEUE_LEN 10
#define OSPROJ_PROC_START   0x09487940
#define OSPROJ_PROC_END     0x09487941
#include <sys/types.h>

// User function
void GetTime(int pid);
void Pause(int);
int callSystemTime(pid_t pid, int status);

struct Queue_T
{
      char pname[POLICY_LEN];
      int ready_time;
      int exec_time;
      int remaining_time;
      int active;
      int start_time;
      int end_time;
}; typedef struct Queue_T Queue_T;

struct Queue_T RR_Queue[MAX_QUEUE_LEN];
char Policy[POLICY_LEN];
int Process_Num;
volatile unsigned long waiter;

#endif
