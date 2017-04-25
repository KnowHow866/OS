#include "RR.h"

//#include "../main.h"

/*void GetTime(pid_t pid)
{
    clock_t t;
    t = clock();
    printf("[Project1] ");
    printf("pid:%d ", pid);
    printf("time: %lu\n",t);
}*/

void simuTime(char pname[], int time){
	printf("[Project1] ");
    printf("pid:%s ", pname);
    printf("time: %d\n",time);
}

void delay(int unit){
	volatile unsigned long i;
	for(i = 0; i < 1000000UL*unit ;i++);
}

void log_residue(int pid, int residue_time){
	FILE *in;
	if(residue_time == 0){
		in = fopen("residue_Queue.txt","w");
		fprintf(in, "");
		fclose(in);
	}
	else{
		in = fopen("residue_Queue.txt","a");
		fprintf(in,"%d %d\n", pid, residue_time);
		fclose(in);
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
			if(P_Queue[i].exec_time <= 500){
				delay(P_Queue[i].exec_time);
				time += P_Queue[i].exec_time;
				//GetTime(getpid());
				//simuTime(P_Queue[i].pname , time);
				exit(0);
			}
			//執行不完，加入residue待執行，stop
			else if(P_Queue[i].exec_time > 500){
				while(P_Queue[i].exec_time > 500){
					//delay(quantum);
					time += 500;
					int me_pid = getpid();
					int the_residue = P_Queue[i].exec_time -500;
					//行程t > 500 儲存進度
					log_residue(me_pid, the_residue);
					P_Queue[i].exec_time = the_residue;

					delay(quantum);
					//simuTime(P_Queue[i].pname , time);
					kill(getpid(), SIGSTOP);
					//exit(NULL);
				}
				if(P_Queue[i].exec_time <= 500){
					delay(P_Queue[i].exec_time);
					time += P_Queue[i].exec_time;
					//GetTime(getpid());
					//simuTime(P_Queue[i].pname , time);
					exit(0);
				}
			}
			else{
				printf("Process err $d\n", i);
				exit(0);
			}
		}
		if(pid > 0){	//Father
			pid_t cpid; //回傳子行程pid
			//這個行程到了沒
			if(time < P_Queue[i].ready_time){
				//等待行程到達
				delay(P_Queue[i].ready_time - time); 
				time = P_Queue[i].ready_time;
			}
			//能夠執行完畢，等待
			if(P_Queue[i].exec_time <= 500){
				delay(P_Queue[i].exec_time);
				cpid = wait(NULL);
				time += P_Queue[i].exec_time;
			}
			//做不完，等待500後繼續
			else if(P_Queue[i].exec_time > 500){
				time += 500;
				delay(quantum);
				cpid = waitpid(pid, NULL ,WUNTRACED);
				//cpid = wait(NULL);
			}
			else{
				printf("Father Err %d\n", i);
			}
			//輸出系統時間
			//simuTime(P_Queue[i].pname, time);
			GetTime(cpid);
		}

	}

	 //當residue_Queue檔案還有pid-time值組，RR輪詢
	//FILE *read;
	//read = fopen("residue_Queue.txt","r");
	//puts("file is open");

	while(1){
		 //當residue_Queue檔案還有pid-time值組，RR輪詢
		FILE *read;	
		read = fopen("residue_Queue.txt","r");
		int count = 0;
		Remain* RR_Queue = (Remain*)malloc(sizeof(Remain)*Process_Num);
		//puts("RR_Queue builded");

		while( fscanf(read," %d %d",
						&(RR_Queue[count].pid),
						&(RR_Queue[count].rm_time)) != 2) count++;
		//puts("read data ready");
		log_residue(0,0); //清空資料
		if(count == 0) break;

		for(int i = 0; i < count; i++){
			pid_t cpid; //回傳子行程pid
			kill(RR_Queue[i].pid, SIGCONT);

			if(RR_Queue[i].rm_time <= 500){
				time += RR_Queue[i].rm_time;
				delay(RR_Queue[i].rm_time);
				wait(NULL);
			}
			else if(RR_Queue[i].rm_time > 500){
				time += 500;
				//printf("wait for pid %d\n",RR_Queue[i].pid);
				delay(500);
				waitpid(pid, NULL ,WUNTRACED);
			}
			else{
				puts("wait Err\n");
				wait(NULL);
			}

			GetTime(RR_Queue[i].pid);
		}

		free(RR_Queue);
		fclose(read);
		//break;
	}

	//fclose(read);
}

/*
void FIFO(Queue_T* P_Queue, int Process_Num){
	int quantum = 500;
	//printf("Start: ");
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
			exit(0);
		}
		if(pid > 0){
			wait(NULL);
			GetTime(1);
		}
	}
}
*/
