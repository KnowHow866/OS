int main(){
	scanf("%s",&Policy);
	scanf("%d",&Process_Num);
	//:Queue_T *P_Queue = (*Queue_T)malloc((Queue_T)*Process_Num);
	for(int i = 0; i < Process_Num; i++){
		scanf(" %s %d %d",&P_Queue[i].pname, &P_Queue[i].ready_time, &P_Queue[i].exec_time);
	}
}