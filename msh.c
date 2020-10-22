/**
 * msh.c
 * Author: Walker Gray
 * Derivative of simple-shell.c
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include "CommandHistory.h"

#define MAX_LINE		80 /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	9 /* size of history */

CommandHistory* history;
int background;             	/* equals 1 if a command is followed by '&' */
 
int argscmp(char** args1, char** args2)
{
	int i = 0;
	int result=0;
	while(*(args1+i) && *(args2+i))
	{
		if((result=strcmp(*(args1+i), *(args2+i))))
		{
			return result;
		}
		i++;
	}
	if(*(args1+i) || *(args2+i))
	{
		return 1;
	}
	return 0;
}

/**
 * Add the most recent command to the history.
 */
void addtohistory(char inputBuffer[], char** args) 
{
	insertMemory(inputBuffer, args, background, history);
	return;
}

int startsWith(const char* str, const char* prefix)
{
	if(!str || !prefix)
	{
		return !(str || prefix);
	}
	while(*prefix != '\0' && *str != '\0')
	{
		if(*str != *prefix)
		{
			return 0;
		}
		str++;
		prefix++;
	}
	return *prefix == '\0';
}

/** 
 * The setup function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */
int setup(char inputBuffer[], char *args[],int *background)
{
    int length,		/* # of characters in the command line */
	i;				/* loop index for accessing inputBuffer array */

	//define your local variables here;
	int argsIndex = 1;
	
    /* read what the user enters on the command line */
	do 
	{
		printf("msh> ");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
	}
	while(inputBuffer[0] == '\n'); /* swallow newline characters */

    if (length == 0)
	{
		printf("\n");
        exit(0);            /* ^d was entered, end of user command stream */
	}
 
    if (length < 0) 
	{
		perror("error reading the command");
		exit(-1);           /* terminate with error code of -1 */
    }
	
	/**
	 * Check if they are using history
	 */
	if(startsWith(inputBuffer, "!")) 
	{
		if(startsWith(inputBuffer+1, "!\n"))
		{
			if(!history->tail)
			{
				fprintf(stderr, "!!: No commands in history\n");
				return 1;
			}
			int count = 0;
			while(*(history->tail->args+count))
			{
				args[count] = history->tail->args[count];
				count++;
			}
			args[count] = NULL;
			*background = history->tail->background;
			printf("!!: %s\n", history->tail->command);
			addtohistory(history->tail->command, args);
			return 1;
		} else if(isdigit(inputBuffer[1])) {
			int j = 0;
			while(isdigit(*(inputBuffer+(++j)))){}
			*(inputBuffer+j) = '\0';
			int commandIndex = atoi(inputBuffer+1)-1;
			if(commandIndex >= history->size)
			{
				fprintf(stderr, "!%d: No such command in history\n", commandIndex+1);
				return 1;
			}
			CommandMemory* item = memoryAt(commandIndex, history);
			if(item)
			{
				int count = 0;
				while(*(item->args+count))
				{
					args[count] = item->args[count];
					count++;
				}
				args[count] = NULL;
				*background = item->background;
				printf("!%d: %s\n", commandIndex+1, item->command);
				addtohistory(item->command, args);
			} else {
				fprintf(stderr, "!%d: No such command in history\n", commandIndex+1);
			}
			return 1;
		} else {
			fprintf(stderr, "bad !\n");
		}
	}
	
	char cmd[80];
	strcpy(cmd, inputBuffer);
	char* pt = cmd;
	while(*pt != '\n') {pt++;}
	*pt = '\0';
	
	/**
	 * Parse the contents of inputBuffer
	 */
	// args[i] is a pointer to a string, its value is the address of the first charater of that string 
	args[0] = inputBuffer;
    for (i=0;i<length;i++)
	{ 
        switch (inputBuffer[i])
		{
			case '\n':  /* should be the final char examined */
				inputBuffer[i] = '\0';
				args[argsIndex] = NULL;
				argsIndex++;
				break;
				/* no more arguments to this command */	
			case ' ':
			case '\t' : /* argument separators */
				inputBuffer[i] = '\0';
				if(++i < length)
				{
					if(inputBuffer[i] != '\n')
					{
						args[argsIndex] = inputBuffer + i;
						argsIndex++;
					} else {
						args[argsIndex] = NULL;
						argsIndex++;
						inputBuffer[i] = '\0';
					}
				}
				break;
			case '&':
				*background = 1;
				inputBuffer[i] = '\0';
				break;
	    	default :             /* some other character */
				 break;
		} /* end of switch */
	}    /* end of for */
	if(length == MAX_LINE)
	{
		args[argsIndex] = NULL;
		argsIndex++;
		*(inputBuffer+length) = '\0';
	}
	addtohistory(cmd, args);
	return 1;
	
} /* end of setup routine */

int main(void)
{
	char inputBuffer[MAX_LINE]; 	/* buffer to hold the command entered */
	char *args[MAX_LINE/2 + 1];		/* command line (of 80) has max of 40 arguments */
	pid_t child;            		/* process id of the child process */
	history = newCommandHistory(MAX_COMMANDS);
	
	//define your local variables here, at the beginning of your program. 
	int shouldrun = 1;
	
    while (1)
	{   /* Program terminates normally inside setup */
		background = 0;
		shouldrun = setup(inputBuffer,args,&background);       /* get next command */
		char*mshe[]={"./msh","-e",NULL};
		if(args[0] && !strcmp(args[0],"history"))
		{
			if(!args[1])
			{
				printHistory(history);
			} else {
				if((child = fork()) == 0)
				{
					execvp(args[0], args);
					perror(args[0]);
				}
				if(!background)
				{
					int status;
					waitpid(child, &status, 0);
				}
			}
		} else if(!argscmp(args, mshe)) {
			deleteCommandHistory(history);
			args[1] = NULL;
			execvp(args[0], args);
			perror(args[0]);
		} else if(shouldrun) {
			if((child = fork()) == 0)
			{
				execvp(args[0], args);
				if(startsWith(args[0], "./"))
				{
					perror(args[0]);
				} else {
					fprintf(stderr, "%s: command not found\n", args[0]);
				}
				exit(-1);
			}
			if(child < 0)
			{
				perror("fork");
				exit(-1);
			}
			if(!background)
			{
				int status;
				waitpid(child, &status, 0);
			} else {
				background = 0;
			}
		}
    }
	return 0;
}
