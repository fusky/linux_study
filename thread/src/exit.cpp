#include<io.h>

void exit_thread(void* arg)
{
	cout << (char*)arg << endl;
}

void* th_fun(void* arg)
{

	pthread_cleanup_push(exit_thread, (void*)"First exit!");//注册退出函数
	pthread_cleanup_push(exit_thread, (void*)"Secend exit!");
	//线程操作
	//....
	cout << "Thread Finished!" << endl;
	//找最近的匹配push和pop
	pthread_cleanup_pop(1);//决定退出函数是否执行
	pthread_cleanup_pop(1);
}

int main(int argc, char** argv)
{
	pthread_t tid;
	pthread_create(&tid, NULL, th_fun, NULL);

	pthread_join(tid,NULL);
	return 0;
}
