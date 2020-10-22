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
		strcpy(tmp->args[count], args[count]);
		count++;
	}
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
	free(memory->command);
	free(memory);
}


#endif