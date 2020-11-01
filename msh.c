/**
 * msh.c
 * Author: Walker Gray
 * Derivative of simple-shell.c
 * CPSC 346
 * Project 3
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE		80 /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	9 /* size of history */

char history[MAX_COMMANDS][MAX_LINE]; //the array used to store history commands.
char display_history [MAX_COMMANDS][MAX_LINE]; 
/*the array used for "printf" to display history nicely. Remove special characters like "\n" or "\0"*/

int command_count = 0;

// used for some light debug/testing
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

// utilities
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

size_t bufflen(char* buffer)
{
	if(!buffer)
	{
		return -1;
	}
	size_t i = 0;
	while(1)
	{
		switch(*(buffer + i++))
		{
			case '\0':
			case '\n':
				return i;
		}
	}
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
 * Add the most recent command to the history.
 */

void addtohistory(char inputBuffer[]) 
{
	int length = (int)(bufflen(inputBuffer));
	if(startsWith(inputBuffer, "history\n"))
	{
		return;
	}
	// update array"history": add the command to history, strcpy(str1,str2);
	if(command_count == MAX_COMMANDS)
	{
		int i;
		for(i = 1; i < MAX_COMMANDS; i++)
		{
			strcpy(history[i-1], history[i]);
			strcpy(display_history[i-1], display_history[i]);
		}
		strcpy(history[i-1], "");
		strcpy(display_history[i-1], "");
		command_count--;
	}
	strcpy(history[command_count], inputBuffer);
	// update array"display_history": remove characters like '\n', '\0' in order to display nicely
	strcpy(display_history[command_count], inputBuffer);
	if (history[command_count][length] != '\0')
	{
		history[command_count][length] = '\0';
	}
	if (display_history[command_count++][length-1] == '\n')
	{
		display_history[command_count-1][length-1] = '\0';
	}
	return;
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
	while(inputBuffer[0] == '\n' && length == 1); /* swallow newline characters */


    if (length == 0)
	{
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
	if (inputBuffer[0] == '!' && length == 3) 
	{
		if(!command_count)
		{
			inputBuffer[length-1] = '\0';
			fprintf(stderr, "%s: No commands in history\n", inputBuffer);
			return 0;
		}
		if(inputBuffer[1] == '!')
		{
			command_number = command_count;
		} else if (isdigit(inputBuffer[1])) {
			command_number = atoi(inputBuffer+1);
			if(command_number > command_count)
			{
				fprintf(stderr, "!%d: No such command in history\n", command_count);
				return 0;
			}
		} else {
			perror(inputBuffer);
			return 0;
		}
		strcpy(inputBuffer, history[command_number-1]);
		int j = 0;
		while(*(inputBuffer+j++) != '\n')
		{}
		inputBuffer[j] = '\0';
		length = j;
		printf("!%d: %s\n", command_number, display_history[command_number-1]);
	}
	
	
	
	/**
	 * Add the command to the history
	 */
	// fill in your code here Part II, add the following line for PartII
	addtohistory(inputBuffer); 
	
	//trim leading whitespace
	for (i=0; i<length; i++)
	{
		if(inputBuffer[i] != ' ' && inputBuffer[i] != '\t' && inputBuffer[i] != '\n')
		{
			break;
		}
	}
	
	/**
	 * Parse the contents of inputBuffer
	 */
	
    args[0] = inputBuffer + (i++);
    for (;i<length;i++)
	{ 
        switch (inputBuffer[i])
		{
			case ' ':
			case '\t' : /* argument separators */
				inputBuffer[i] = '\0';
				if(++i < length)
				{
					switch(inputBuffer[i])
					{
						case '&':
							*background = 1;
						case '\n':
							args[argsIndex++] = NULL;
							inputBuffer[i] = '\0';
							break;
						default:
							args[argsIndex++] = inputBuffer + i;
					}
				}
				break;
			case '\n':  /* should be the final char examined */
				inputBuffer[i] = '\0';
				args[argsIndex] = NULL;
				argsIndex++;
				break;
				/* no more arguments to this command */	
			case '&':
				*background = 1;
				inputBuffer[i] = '\0';
				break;
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

	return 1;
} /* end of setup routine */


int main(void)
{
	char inputBuffer[MAX_LINE]; 	/* buffer to hold the command entered */
	int background;             	/* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	pid_t child;            		/* process id of the child process */
	
	//define your local variables here, at the beginning of your program. 

	int shouldrun = 1;
	
	//changed shouldrun to dictate whether forking and exec should occur since exiting
	//occurs in setup func
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
		
	
		char*mshe[]={"./msh","-e",NULL};
		if(args[0] && !strcmp(args[0], "history"))
		{
			int i;
			for(i = command_count-1; i >= 0; i--)
			{
				printf("%d. %s\n", i+1, display_history[i]);
			}
			if(command_count == 0)
			{
				printf("history is empty\n");
			}
		} else if(!strcmp(args[0], "exit")) {
			exit(0);
		} else if(!argscmp(args, mshe)) {
			//command to allow hot swapping shell program after recompiling
			args[1] = NULL;
			execvp(args[0], args);
			perror(args[0]);
		} else if (shouldrun) {
			if((child = fork()) == 0)
			{
				execvp(args[0], args);
				if(args[0][0] == '.' && args[0][1] == '/')
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

