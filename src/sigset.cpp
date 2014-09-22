#include<io.h>

static int flags = 0;

void out(sigset_t set)
{
	int i = 1;
	for(i = 1; i <= 31; i++){
		if(sigismember(&set, i) > 0){
			cout << getpid() << ":"\
				<< "signal=" << i << endl;
	}
	}
}

void sighandler(int sig)
{
	cout << "In sighandler:" << endl;
	sigset_t oset;
	sigemptyset(&oset);
	sigprocmask(SIG_BLOCK, NULL, &oset);
	out(oset);

	flags = 1;

}

int main(int argc, char **argv)
{
	signal(SIGINT, sighandler);

	sigset_t set, oset;
	sigemptyset(&set);
	sigemptyset(&oset);

	cout << "Signal Before:" << endl;
	sigprocmask(SIG_BLOCK, NULL, &oset);
	out(oset);

	while(!flags) pause();

	cout << "Signal after:" << endl;
	sigprocmask(SIG_BLOCK, NULL, &oset);
	out(oset);

	return 0;
}
