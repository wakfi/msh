#ifndef COMMAND_MEMORY_H
#define COMMAND_MEMORY_H
#include <stdlib.h>

typedef struct CommandMemory CommandMemory
struct CommandMemory
{
	char* command;
	CommandMemory* next;
};

CommandMemory* newCommandMemory(char* command)
{
	CommandMemory* tmp = malloc(sizeof(CommandMemory));
	tmp->command = command;
	tmp->next = NULL;
	return tmp;
}


#endif