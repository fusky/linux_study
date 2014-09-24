#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include"minishell.h"

//print welcome message
void welcomeMessage()
{
	printf("\nWelcome to minishell!\n");
}

//print prompt
void printPrompt()
{
	printf("choggle shell>>>");
}

char* lookupPath(char **argv, char **dir)
{
	char *result;
	char pName[MAX_PATH_LEN];
	if(*argv[0] == '/'){
		return argv[0];
	}else if(*argv[0] == '.'){
		if(*++argv[0] == '.'){
			if(getcwd(pName, sizeof(pName)) == NULL)
				perror("getcwd(): err!\n");
			else{
				*--argv[0];
				asprintf(&result, "%s%s%s", pName, "/", argv[0]);
			}
			return result;
		}
	}

	int i;
	for(i = 0;i < MAX_PATHS; i++){
		if(dir[i] != NULL){
			asprintf(&result, "%s%s%s", dir[i], "/", argv[0]);
			if(access(result, X_OK) == 0){
				return result;
			}
		}
		else continue;
	}

	fprintf(stderr, "%s: command not found\n",argv[0]);
	return NULL;
}

int parsePath(char* dirs[])
{
	int debug = 0;
	char* pathEnvVar;
	char* thePath;
	int i;

	for(i=0;i<MAX_ARGS;i++)
		dirs[i] = NULL;
	pathEnvVar = (char*)getenv("PATH");
	thePath = (char*)malloc(strlen(pathEnvVar)+1);
	strcpy(thePath,pathEnvVar);

	char* pch;
	pch = strtok(thePath, ":");
	int j = 0;
	while(pch != NULL){
		pch = strtok(NULL, ":");
		dirs[j] = pch;
		j++;
	}

	if(debug){
		printf("Directories in PATH variable\n");
		for(i=0;i<MAX_PATHS;i++)
			if(dirs[i] != '\0')
				printf("	Directory[%d]: %s\n", i, dirs[i]);
	}
}

int parseCommand(char* commandLine, struct command_t *command)
{
	int debug = 0;
	char* pch;
	pch = strtok(commandLine, " ");
	int i = 0;
	while(pch != NULL){
		command->argv[i] = pch;
		pch = strtok(NULL, " ");
		i++;
	}
	command->argc = i;
	command->argv[i++] = NULL;
	return 0;
}

int readCommand(char* buffer, char* commandInput)
{
	buf_chars = 0;

	while((*commandInput != '\n')&&(buf_chars < LINE_LEN)){
		buffer[buf_chars++] = *commandInput;
		*commandInput = getchar();
	}

	buffer[buf_chars] = '\0';
	return 0;
}

int executeFileInCommand(char* commandName, char* argv[], char* filename)
{
	int pipefd[2];

	if(pipe(pipefd)){
		perror("pipe");
		exit(127);
	}

	switch(fork()){
		case -1:
			perror("fork()");
			exit(127);
		case 0:
			close(pipefd[0]);
			dup2(pipefd[1], 1);
			close(pipefd[1]);
			FILE* pFile;
			char mystring;

			pFile = fopen(filename, "r");
			if(pFile == NULL) perror("Error opening file");
			else{
				while((mystring = fgetc(pFile)) != EOF){
					putchar(mystring);
				}
				fclose(pFile);
			}
			exit(EXIT_SUCCESS);
		default:
			close(pipefd[1]);
			dup2(pipefd[0],0);
			close(pipefd[0]);

			execve(commandName, argv, 0);
			perror(commandName);
			exit(125);
	}
	return 0;
}

int executeFileOutCommand(char* commandName, char* argv[], char* filename)
{
	int def = dup(1);

	int file = open(filename,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU|\
			S_IRGRP|S_IROTH);
	if(file < 0) return 1;

	if(dup2(file,1)<0) return 1;
	int pid;
	if(pid = fork() == 0){
		close(file);
		close(def);
		execve(commandName, argv, 0);
		return 0;
	}
	dup2(def, 1);
	close(file);
	close(def);
	wait(NULL);
	close(file);
	return 0;
}

void executePipedCommand(char* argvA[],char* argvB[], char *nameA, \
		char *nameB)
{
	int pipefd[2];

	if(pipe(pipefd)){
		perror("pipe");
		exit(127);
	}

	switch(fork()){
		case -1:
			perror("fork()");
			exit(127);
		case 0:
			close(pipefd[0]);
			dup2(pipefd[1],1);
			close(pipefd[1]);
			execve(nameA, argvA, 0);
			perror(nameA);
			exit(126);
		default:
			close(pipefd[1]);
			dup2(pipefd[0],0);
			close(pipefd[0]);
			execve(nameB, argvB, 0);
			perror(nameB);
			exit(125);
	}
}





















