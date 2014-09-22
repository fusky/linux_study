#include<io.h>

void sighandler(int sig)
{
	cout << "SIGALRM occurred!" << endl;

}

int main(int argc, char **argv)
{	
	signal(SIGALRM, sighandler);
	alarm(5);

	int i = 0;
	while(i<30){
		cout << getpid() << ": i = " << i++ << endl;
		sleep(1);
	}

	return 0;
}
