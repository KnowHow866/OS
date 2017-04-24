#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <errno.h> 
#include <sys/wait.h> 
 
int main()
{
	int pid, i;
	pid = fork();
	
	switch(pid)
	{
	case -1: /* an error occurred */
		printf("Fork error");
		break;
	case 0: /* this code is executed by the child process */
		for(i=1; i<10; i++){
			printf("Child process, iteration: %d\n", i);
			kill(pid, SIGSTOP);
		}
	default: /* this code is executed by the parent process */
		waitpid(pid,NULL,WUNTRACED);
		for(i=1; i<10; i++){
			printf("Parent process, iteration: %d\n", i);
			kill(pid, SIGCONT);
		}
	}
}