#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>

#define POLICY_LEN 10

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
}

struct remain{
	pid_t pid;
	int rm_time;
	int nice;
} remain_P[POLICY_LEN];

struct input process[MAX_QUEUE_LEN];

char Policy[POLICY_LEN];
int Process_Num = 0;

int main(){
	scanf("%s",&Policy);
	scanf("%d",&Process_Num);
	Queue_T *P_Queue = (*Queue_T)malloc((Queue_T)*Process_Num);
	for(int i = 0; i < Process_Num; i++){
		scanf(" %s %d %d",&P_Queue[i]->pname, &P_Queue[i]->ready_time, &P_Queue[i]->exec_time);
	}  

	for(int i = 0;i < Process_Num; i++){
		printf("%s\n",P_Queue[i]->pname);
		printf("%d %d",P_Queue[i]->ready_time,P_Queue[i]->exec_time);
	}
}

