/**
 * wsh.c
 * Author: Walker Gray
 * Derivative of simple-shell.c
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>


#define MAX_LINE		80 /* 80 chars per line, per command, should be enough. */
#define MAX_COMMANDS	9 /* size of history */

char history[MAX_COMMANDS][MAX_LINE]; //the array used to store history commands.
char display_history [MAX_COMMANDS][MAX_LINE]; 
/*the array used for "printf" to display history nicely. Remove special characters like "\n" or "\0"*/

int command_count = 0;

/**
 * Add the most recent command to the history.
 */

void addtohistory(char inputBuffer[]) 
{

	
	// update array"history": add the command to history, strcpy(str1,str2);
	
	// update array"display_history": remove characters like '\n', '\0' in order to display nicely
 

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

	//define your local varialbes here;
	
	
    /* read what the user enters on the command line */
	do 
	{
		printf("wsh> ");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
	}
	while (inputBuffer[0] == '\n'); /* swallow newline characters */


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
	/*if (inputBuffer[0] == '!') {

	}*/
	
	

	/**
	 * Add the command to the history
	 */
	 // fill in your code here Part II, add the following line for PartII
	// addtohistory(inputBuffer); 
	
	/**
	 * Parse the contents of inputBuffer
	 */
	
    for (i=0;i<length;i++)
	{ 
		/* examine every character in the inputBuffer */
		
        switch (inputBuffer[i])
		{
			case ' ':
			case '\t' : /* argument separators */
				//fill in your code here, set up args				

				
			case '\n':  /* should be the final char examined */
				//fill in your code here, set up the last item args[x] ==NULL;
				/* no more arguments to this command */	

				
	    		default :             /* some other character */
				 //fill in your code here, 
				/* args[i] is a pointer to a string, its value is the address of the first charater of that string
				* You want to track the location of the beginning character of each string. 
				* The location is the first character, which is not '\t', not '\t', and not '\n'
				* You also need check "&". If '&' is detected, setup background flag.
				*/  
		} /* end of switch */
	}    /* end of for */
	
	/**
	 * Here you finish parsing the input. 
	 * There is one more thing to assure. If we get '&', make sure you don't enter it in the args array
	 */
	


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
	

		
    while (shouldrun)
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
		
	

		if (shouldrun)
		{
			/* creates a duplicate process! */
			//here fill in your code
			/* pid<0  error
			*  pid == 0, it is the child process. use the system call execvp(args[0],args);
			*  pid > 0, it is the parent. Here you need consider it is foreground or background
			*/
		}
    }
	
	return 0;
}

