#include<io.h>

int main(int argc, char **argv)
{
	umask(0);
	pid_t pid;
	int i;
	for(i = 0; i < 1; i++){
	pid = fork();

	if(pid > 0){
	//父进程
		wait(0);
		exit(0);
		break;
	}else if(pid == 0){
	//子进程
		if(i == 0){//子进程
			exit(0);
		}if(i == 1){//孙子进程
			
		}
	}
	}
	setsid();
	chdir("/");
	close(0);
	close(1);
	close(2);

	//打开日志
	openlog(argv[1], LOG_PID, LOG_USER);
	//操作日志
	i = 0;
	syslog(LOG_ERR, "OUTPUT i = %d\n", i++);
	sleep(1);
	//关闭日志
	closelog();

	return 0;
}
