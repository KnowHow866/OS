#ifndef RR_LIB_H
#define RR_LIB_H


struct _Queue_T
{
      char pname[10];
      int ready_time;
      int exec_time;
};
typedef struct _Queue_T Queue_T;

struct remain{
	pid_t pid;
	int rm_time;
	int nice;
};
typedef struct remain Remain;

void GetTime(pid_t pid);
void FIFO(Queue_T* P_Queue, int Process_Num);
void RR(Queue_T* P_Queue, int Process_Num);
void delay(int unit);
void simuTime(char pname[], int time);
void log_residue(pid_t pid, int residue_time);

#endif
