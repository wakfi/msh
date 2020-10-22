#ifndef COMMAND_MEMORY_H
#define COMMAND_MEMORY_H
#include <stdlib.h>

typedef struct CommandMemory CommandMemory;
struct CommandMemory
{
	char* command;
	char** args;
	int background;
	CommandMemory* next;
};

void __printBuffer(char* args[])
{
	int length = 41;
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

void __printStrings(char* args[])
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

int __arrsize(char* arr)
{
	int i = 0;
	if(!arr) return i;
	while(*(arr+i))
	{
		i++;
	}
	return ++i;
}

CommandMemory* newCommandMemory(char* command, char** args, int background)
{
	CommandMemory* tmp = malloc(sizeof(CommandMemory));
	tmp->command = malloc(__arrsize(command));
	strcpy(tmp->command, command);
	tmp->args = malloc(__arrsize((char*)args)*8);
	int count = 0;
	while(*(args+count))
	{
		tmp->args[count] = malloc(sizeof(args[count]));
		//fprintf(stderr, "copying from %p to %p\n", args[count], tmp->args[count]);
		strcpy(tmp->args[count], args[count]);
		//fprintf(stderr, "copied %s as %s\n", args[count], tmp->args[count]);
		count++;
	}
	//__printBuffer(args);
	//__printBuffer(tmp->args);
	//fprintf(stderr, "copied %s as %s\n", args[count], tmp->args[count]);
	//__printStrings(tmp->args);
	//__printStrings(args);
	//fprintf(stderr, "command is %s\n", tmp->command);
	tmp->background = background;
	tmp->next = NULL;
	return tmp;
}

void deleteCommandMemory(CommandMemory* memory)
{
	int count = 0;
	while(*(memory->args+count))
	{
		free(memory->args[count]);
		count++;
	}
	free(memory->args);
	free(memory);
}


#endif