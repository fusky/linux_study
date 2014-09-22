//信号的不可靠性

#include <io.h>

int *heap;
int array[10];

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

}

int main(int argc, char** argv)
{
	signal(SIGINT, sighandler);
	heap = new int[10];
	set(10);

	return 0;
}
