#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "minishell.h"

struct command_t command;

// internal command clears terminal screen
void clearScreen()
{
	printf("\033[2J\033[1;1H");
}

//not implemented yet
void self()
{
	printf("self...\n");
}

//internal command to change diractory
void changeDir()
{
	if(command.argv[1] == NULL){
		chdir(getenv("HOME"));
	}else{
		if(chdir(command.argv[1]) == -1){
			printf("%s: no such directory\n", command.argv[1]);
		}
	}
}

//this function check is for built in commands
//and processes if there any
int checkInternalCommand()
{
	if(strcmp("cd", command.argv[0]) == 0){
		changeDir();
		return 1;
	}
	if(strcmp("clear", command.argv[0]) == 0){
		clearScreen();
		return 1;
	}
	if(strcmp("self",command.argv[0]) == 0){
		clearScreen();
		return 1;
	}
	return 0;
}

//excutecommand() executes regular command, commands which dosn't\
	have rediractions
//example:ls -il, cat filename
int excuteCommand()
{
	int child_pid;
	int status;
	pid_t thisChPID;

	child_pid = fork();
	if(child_pid < 0){
		fprintf(stderr,"Fork fails:\n");
		return 1;
	}else if(child_pid == 0){
		//child
		execve(command.name, command.argv, 0);
		printf("Child process failed\n");
		return 1;
	}else if(child_pid > 0){
		//parent
		do{
			thisChPID = waitpid(child_pid, &status, WUNTRACED | WCONTINUED);
			if(thisChPID == -1){
				perror("waitpid");
				exit(EXIT_FAILURE);
			}

			if(WIFEXITED(status)){
				printf("exited, status = %d\n",WEXITSTATUS(status));
				return 0;
			}else if(WIFSIGNALED(status)){
				printf("killed by signal %d\n", WTERMSIG(status));
			}else if(WIFSTOPPED(status)){
				printf("stopped by signal %d\n", WSTOPSIG(status));
			}else if(WIFCONTINUED(status)){
				printf("continued\n");
			}
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 0;
}

//this function process commands that have pipeline in it "|"
//this function is called from processCommand() function
//function accepts indes location of | and processes accordingly

int processPipedCommand(int i)
{
	char *argvA[5];
	char *argvB[5];
	char *nameA, *nameB;

	int ii;
	for(ii = 0; ii < i; ii++){
		argvA[ii] = command.argv[ii];
	}
	argvA[ii] == NULL;
	nameA = lookupPath(argvA, pathv);

	int j, jj = 0;
	for(j=i+1; j<command.argc; j++){
		argvB[jj] = command.argv[j];
		jj++;
	}
	argvB[jj] = NULL;

	int pid, status;
	fflush(stdout);

	switch((pid = fork())){
		case -1:
			perror("fork");
		case 0:
			//child
			executePipedCommand(argvA, argvB, nameA, nameB);
			break;
		default:
			pid = wait(&status);
			return 0;
	}
	return 1;
}

int processFileOutCommand(int i)
{
	char *argv[5];
	char *commandName;
	int j;
	for(j=0; j<i; j++){
		argv[j] = command.argv[j];
	}
	argv[j] = NULL;
	commandName = lookupPath(argv, pathv);
	return executeFileOutCommand(commandName, argv, command.argv[i+1]);
}

int processFileInCommand(int i)
{
	char *argv[5];
	char *commandName;
	int j;
	for(j=0;j<i;j++){
		argv[j] = command.argv[j];
	}
	argv[j] = NULL;
	commandName = lookupPath(argv, pathv);

	int pid, status;
	fflush(stdout);

	switch((pid = fork())){
		case -1:
			perror("fork");
			break;
		case 0:
			executeFileInCommand(commandName, argv, command.argv[i+1]);
			break;
		default:
			pid = wait(&status);
			return 0;
	}
	return 0;
}

int processCommand()
{
	int i;
	int infile=0, outfile=0, pipeLine=0;
	char *outFileName;
	char *inFileName;

	for(i=0;i<command.argc;i++){
		if(strcmp(command.argv[i],">") == 0){
			return processFileOutCommand(i);
		}else if(strcmp(command.argv[i],"<") == 0){
			return processFileInCommand(i);
		}else if(strcmp(command.argv[i], "|") == 0){
			return processPipedCommand(i);
		}
	}

	return excuteCommand();
}

int main(int argc, char **argv)
{
	int i;
	int debug = 0;

	parsePath(pathv);
	welcomeMessage();

	//main loop
	while(1){
			printPrompt();
			commandInput = getchar();
			if(commandInput == '\n'){
				continue;
			}else{
				readCommand(commandLine, &commandInput);
				if((strcmp(commandLine, "exit") == 0) || \
						(strcmp(commandLine, "quit") == 0))
					break;

				parseCommand(commandLine, &command);
				if(checkInternalCommand() == 0){
					command.name = lookupPath(command.argv, pathv);
					
					if(command.name == NULL){
						printf("Stub: error\n");
						continue;
					}

					processCommand();
				}
			}
	}

	printf("\n");
	return 0;
}






























