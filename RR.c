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

#define POLICY_LEN 10

//Struct
struct _Queue_T
{
      char pname[POLICY_LEN];
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

//--------------------------------------
struct node
{
	Queue_T process;
	struct node* next;
	struct node* prev;
};
typedef struct node Node;

struct list
{
	Node* entry;
	Node* end;
};
typedef struct list List;

//Function
void GetTime(pid_t pid);
void FIFO(Queue_T* P_Queue, int Process_Num);
void RR(Queue_T* P_Queue, int Process_Num);
void delay(int unit);
void simuTime(char pname[], int time);
void log_residue(pid_t pid, int residue_time);
//---------------------------------------
List newList();
void addList(List* reserve_Queue, Node* p_new);
Node consumeList(List* reserve_Queue);
int isEmpty(List* reserve_Queue);

//Golbal varity
char Policy[POLICY_LEN];
int Process_Num = 0;

int main(){

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
	RR(P_Queue, Process_Num);
}

void FIFO(Queue_T* P_Queue, int Process_Num){
	int quantum = 500;
	printf("Start: ");
	GetTime(1);

	pid_t pid;
	for(int i = 0; i < Process_Num; i++){
		pid = fork();

		if(pid < 0){
	 		puts("Error pid");
		}
		else if(pid == 0){
			volatile unsigned long i;
			for(i = 0; i < 1000000UL* P_Queue[i].ready_time ;i++);
			//GetTime(getpid());
			exit(NULL);
		}
		if(pid > 0){
			wait(NULL);
			GetTime(1);
		}
	}
}

void RR(Queue_T* P_Queue, int Process_Num){
	int quantum = 500;

	Remain* residue = (Remain*)malloc(sizeof(Remain)*(10*Process_Num));

	pid_t pid, status;
	int time = 0; //對應標準時間單位參照進程
	int residue_count = 0;
	for(int i = 0; i < Process_Num; i++){
		//GetTime(1);
		pid = fork();

		if(pid < 0){
	 		puts("Error pid");
		}
		else if(pid == 0){ //Child
			//到了沒
			if(time < P_Queue[i].ready_time){
				//等待行程到達
				delay(P_Queue[i].ready_time - time); 
			}
			//執行完畢，exit
			//執行不完，加入residue待執行，stop
			if(P_Queue[i].exec_time <= 500){
				delay(P_Queue[i].exec_time);
				time += P_Queue[i].exec_time;
				//GetTime(getpid());
				simuTime(P_Queue[i].pname , time);
				exit(NULL);
			}
			else if(P_Queue[i].exec_time > 500){
				delay(quantum);
				time += 500;
				//行程t > 500 儲存進度
				log_residue(getpid(), P_Queue[i].exec_time - 500);
				
				simuTime(P_Queue[i].pname , time);
				//kill(getpid(), SIGSTOP);
				exit(NULL);
			}
			else{
				printf("Process err $d\n", i);
				exit(NULL);
			}
		}
		if(pid > 0){	//Father
			pid_t cpid;
			//這個行程到了沒
			if(time < P_Queue[i].ready_time){
				//等待行程到達
				delay(P_Queue[i].ready_time - time); 
				time = P_Queue[i].ready_time;
			}
			//能夠執行完畢，等待
			//做不完，等待500後繼續
			if(P_Queue[i].exec_time <= 500){
				cpid = wait(NULL);
				time += P_Queue[i].exec_time;

			}
			else if(P_Queue[i].exec_time > 500){
				delay(500);
				time += 500;
				//cpid = waitpid(pid, &status ,WUNTRACED);
				cpid = wait(NULL);
			}
			else{
				printf("Father Err %d\n", i);
			}

			GetTime(cpid);
		}

		printf("Process %d ok\n", i);
	}

	for(int i = 0; i < residue_count; i++){
		printf("residue %d pid: %d, remain time: %d\n",
			i, residue[i].pid, residue[i].rm_time );
		kill(residue[i].pid, SIGCONT);
	}

	puts("End");
	GetTime(1);
}

void GetTime(pid_t pid)
{
    clock_t t;
    t = clock();
    printf("[Project1] ");
    printf("pid:%d ", pid);
    printf("time: %lu\n",t);
}

void simuTime(char pname[], int time){
	printf("[Project1] ");
    printf("pid:%s ", pname);
    printf("time: %d\n",time);
}

void delay(int unit){
	volatile unsigned long i;
	for(i = 0; i < 1000000UL*unit ;i++);
}

void log_residue(pid_t pid, int residue_time){
	FILE *in;
	in = fopen("residue_Queue.txt","a");
	fprintf(in,"%d %d\n", pid, residue_time);
	close(in);
}

//---------------------------------------------
List newList(){
	puts("ready newList");
	List new;
	new.entry->next = new.end;
	new.end->prev = new.entry;
	puts("OK newList");
	return new;
}

void addList(List* reserve_Queue, Node* p_new){
	p_new->next = reserve_Queue->entry->next;
	reserve_Queue->entry->next->prev = p_new;

	reserve_Queue->entry->next = p_new;
	p_new->prev = reserve_Queue->entry;
}

Node consumeList(List* reserve_Queue){
	if(isEmpty(reserve_Queue)){
		puts("list empty err");
		Node no;
		return no;
	}
	else{
		Node* tmp= reserve_Queue->end->prev;
		reserve_Queue->end->prev = reserve_Queue->end->prev->prev;
		reserve_Queue->end->prev->prev->next = reserve_Queue->end->prev;
		return *tmp;
	}
}

int isEmpty(List* reserve_Queue){
	if(reserve_Queue->entry->next == reserve_Queue->end) return 1;
	else return 0;
}

