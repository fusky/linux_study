//解决信号不可靠问题，和sigset.cpp比较
//设置信号屏蔽
#include <io.h>

int *heap;
int array[10];
static int flags = 0;

void set(int n)
{
	int i = 0;
	int stack[10];
	for(i = 0; i < 10; i++){
		stack[i] = n;
		heap[i] = n;
		array[i] = n;
		sleep(1);
	}
	cout << "heap: ";
	for(i = 0; i < 10; i++){
		cout << heap[i] << ",";
	}
	cout << endl;

	cout << "stack: ";
	for(i = 0; i < 10; i++){
		cout << stack[i] << ",";
	}
	cout << endl;

	cout << "data: ";
	for(i = 0; i < 10; i++){
		cout << array[i] << ",";
	}
	cout << endl;
}

void sighandler(int sig)
{
	cout << "in sighandler: " << endl;
	set(20);
	cout << "signal finished!" << endl;
	flags = 1;
}

int main(int argc, char** argv)
{
	signal(SIGINT, sighandler);
	heap = new int[10];
	//set运行时，屏蔽SIGINT信号
	sigset_t oset;
	sigemptyset(&oset);
	sigaddset(&oset, SIGINT);
	sigprocmask(SIG_BLOCK, &oset, NULL);

	set(10);

	//sigdelset(&set, SIGINT);
	sigprocmask(SIG_UNBLOCK, &oset, NULL);
	
	while(!flags) pause();
	
	delete[] heap;
	return 0;
}
















