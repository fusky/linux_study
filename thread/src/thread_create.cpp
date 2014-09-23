#include <io.h>
//创建线程，在线程中打印一句话，同时传参

void* thread_fun(void* arg)
{
	int a;
	a=(int)arg;
	cout << "The number is:" << a << endl;
	cout << "thread exit!" << endl;
	sleep(5);
	//return (void*)10;
	//线程主动要求退出
	pthread_exit((void*)100);
	cout << "这句话不输出" << endl;
}

int main(int argc, char **argv)
{
	//主控线程(进程)退出，不做操作
	//pthread_exit((void*)0);
	
	//main_tid = pthread_self();

	pthread_t tid;
	int i = 10;
	//传参的时候如果大于地址宽度，传地址参数
	int err = pthread_create(&tid, NULL,\
			thread_fun,(void*)i);
	if(err<0){
		cout << "create :" << strerror(err) << endl;
		return -1;
	}
	//被动退出
	pthread_cancel(tid);

#if 0
	//sleep(1);
	//等待处理开辟的线程，阻塞等待
	int *status;
	err = pthread_join(tid, (void**)&status);
	if(err<0){
		cout << "join: " << strerror(err) << endl;
		exit(1);
	}
	cout << "Return: " << (int)status << endl;
#endif

	return 0;
}




















