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

//char history[MAX_COMMANDS][MAX_LINE]; //the array used to store history commands.
//char display_history [MAX_COMMANDS][MAX_LINE]; 
CommandHistory* history;
//CommandHistory* displayHistory;

/*the array used for "printf" to display history nicely. Remove special characters like "\n" or "\0"*/
//int hist_len = 0;

int command_count = 0;
int background;             	/* equals 1 if a command is followed by '&' */

/**
 * Add the most recent command to the history.
 */

 
void printBuffer(char* args[])
{
	int length = MAX_LINE/2 + 1;
	int i;
	int f = 1;
	for(i = 0; i < length; i++)
	{
		if(!args[i])
		{
			f = 0;
		}
		if(f)
			fprintf(stderr, "%p %p %s, ", args+i, args[i], args[i]);
		else
			fprintf(stderr, "%p %p, ", args+i, args[i]);
	}
	fprintf(stderr, "\n");
}

void printStrings(char* args[])
{
	char** curr = args;
	if(!curr) return;
	while(*curr)
	{
		printf("%s ", *curr);
		curr++;
	}
	printf("\n");
}
 
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

void addtohistory(char inputBuffer[], char** args) 
{
	// update array"history": add the command to history, strcpy(str1,str2);
	insertMemory(inputBuffer, args, background, history);
	fprintf(stderr, "added %s to history\n", history->tail->command);
	// update array"display_history": remove characters like '\n', '\0' in order to display nicely
	/*CommandMemory* displayTail = insertMemory(inputBuffer, displayHistory);
	char* command = displayTail->command;
	int i;
	int length = sizeof(inputBuffer)/sizeof(inputBuffer[0]);
	for(i = 0; i < length; i++)
	{
		switch(*(command+i))
		{
			case '\0':
				*(command+i) = ' ';
				break;
			case '\n': 
				*(command+i) = '\0';
				break;
			default:
				break;
		}
	}*/
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
	command_count=0;
    int length,		/* # of characters in the command line */
	i,				/* loop index for accessing inputBuffer array */
	command_number;	/* index of requested command number */

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
	
	// fill in your code here Part II, if the user input is to repeat some history commands
	if (inputBuffer[0] == '!') 
	{
		/* 	
		if(length < 3)
		{
			fprintf(stderr, "command syntax error\n");
			return 0;
		} 
		*/
	}
	
	char cmd[80];
	strcpy(cmd, inputBuffer);
	char* pt = cmd;
	while(*pt != '\n') {pt++;}
	*pt = '\0';

	/**
	 * Add the command to the history
	 */
	 // fill in your code here Part II, add the following line for PartII
	//addtohistory(inputBuffer); 
	
	if(startsWith(inputBuffer, "!")) 
	{
		if(!strcmp(inputBuffer+1, "!\n"))
		{
			printf("!!\n");
			if(!history->head)
			{
				fprintf(stderr, "!!: No commands in history\n");
				return 1;
			}
			int count = 0;
			while(*(history->head->args+count))
			{
				args[count] = history->head->args[count];
				count++;
			}
			args[count] = NULL;
			*background = history->head->background;
			printf("!!: %s\n", history->head->command);
			addtohistory(history->head->command, args);
			return 1;
		} else if(isdigit(inputBuffer[1])) {
			int j = 0;
			while(isdigit(*(inputBuffer+(++j)))){}
			*(inputBuffer+j) = '\0';
			int commandIndex = atoi(inputBuffer+1)-1;
			CommandMemory* item = memoryAt(commandIndex, history);
			if(item)
			{
				int count = 0;
				while(*(item->args+count))
				{
					fprintf(stderr, "changing '%s' to '%s'\n", args[count], item->args[count]);
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
			printf("bad !\n");
		}
	}
	
	/**
	 * Parse the contents of inputBuffer
	 */
	args[0] = inputBuffer;
    for (i=0;i<length;i++)
	{ 
		/* examine every character in the inputBuffer */
		//fprintf(stderr, "%s", inputBuffer+i);
        switch (inputBuffer[i])
		{
			case '\n':  /* should be the final char examined */
				inputBuffer[i] = '\0';
				//fprintf(stderr, "(argsIndex: %d) %p contains %s\n", argsIndex-1, args[argsIndex-1], args[argsIndex-1]);
				args[argsIndex] = NULL;
				argsIndex++;
				break;
				/* no more arguments to this command */	
			case ' ':
			case '\t' : /* argument separators */
				inputBuffer[i] = '\0';
				if(++i < length)
				{
					//fprintf(stderr, "(argsIndex: %d) %p contains %s\n", argsIndex-1, args[argsIndex-1], args[argsIndex-1]);
					if(inputBuffer[i] != '\n')
					{
						args[argsIndex] = inputBuffer + i;
						argsIndex++;
						//fprintf(stderr, "%p contains %s\n", args[argsIndex-1], args[argsIndex-1]);
					} else {
						args[argsIndex] = NULL;
						argsIndex++;
						inputBuffer[i] = '\0';
					}
				}
				break;
				//fill in your code here, set up args
			case '&':
				*background = 1;
				inputBuffer[i] = '\0';
				break;
	    	default :             /* some other character */
				 //fill in your code here, 
				 break;
				/* args[i] is a pointer to a string, its value is the address of the first charater of that string
				* You want to track the location of the beginning character of each string. 
				* The location is the first character, which is not ' ', not '\t', and not '\n'
				* You also need check "&". If '&' is detected, setup background flag.
				*/  
		} /* end of switch */
	}    /* end of for */
	/**
	 * Here you finish parsing the input. 
	 * There is one more thing to assure. If we get '&', make sure you don't enter it in the args array
	 */
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
	//displayHistory = newCommandHistory(MAX_COMMANDS);
	
	//define your local variables here, at the beginning of your program. 

	int shouldrun = 1;
	
    while (1)
	{            		/* Program terminates normally inside setup */
		background = 0;
		
		shouldrun = setup(inputBuffer,args,&background);       /* get next command */
		
		// fill in your code here Part I
		/* if the user typed in "exit", the shell program will return (or terminate). 
		* Call strncmp(str1,str1,count). The function call will return 0 if str1 == str2.
		* "count" is the number of characters we use to compare.    
		*/		
		
		// fill in your code here Part II
		/* if the user typed in "history", the shell program will display the history commands. 
		* you will use "printf" to print the display_history
		* after you display all the array, this command is done. 
		* Your program should go to read again, which means calling the "setup" function.  
		*/
		
		/*

		*/
		//printStrings(args);
		char*mshe[]={"./msh","-e",NULL};
		if(startsWith(inputBuffer, "history") && (inputBuffer[7] == ' ' || inputBuffer[7] == '\n' || inputBuffer[7] == '\0' || inputBuffer[7] == '\t'))
		{
			if(!strcmp(inputBuffer, "history\0"))
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
			/* creates a duplicate process! */
			//here fill in your code
			/* pid < 0  error
			*  pid == 0, it is the child process. use the system call execvp(args[0],args);
			*  pid > 0, it is the parent. Here you need consider it is foreground or background
			*/
			
			if((child = fork()) == 0)
			{
				printf("hi i'm running your stuff\n");
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
				perror("error in fork");
				exit(-1);
			}
			if(!background)
			{
				int status;
				waitpid(child, &status, 0);
			} else {
				background = 0;
			}
			//printStrings(history->head->args);
		}
    }
	return 0;
}

