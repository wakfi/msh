#ifndef COMMAND_HISTORY_H
#define COMMAND_HISTORY_H
#include <stdlib.h>
#include "CommandMemory.h"

typedef struct CommandHistory CommandHistory;
struct CommandHistory
{
	CommandMemory* head;
	CommandMemory* tail;
	int size;
	int limit;
};

CommandHistory* newCommandHistory(int limit)
{
	CommandHistory* tmp = malloc(sizeof(CommandHistory));
	tmp->head = NULL;
	tmp->tail = NULL;
	tmp->size = 0;
	tmp->limit = limit;
	return tmp;
}

void deleteCommandHistory(CommandHistory* history)
{
	CommandMemory* head = history->head;
	CommandMemory* tmp = head;
	while(head)
	{
		tmp = head->next;
		deleteCommandMemory(head);
		head = tmp;
	}
	free(history);
}

CommandMemory* insertMemory(char* command, char** args, int background, CommandHistory* history)
{
	fprintf(stderr, "new memory from %s\n", args[0]);
	CommandMemory* memory = newCommandMemory(command, args, background);
	if(history->tail)
	{
		//fprintf(stderr, "a\n");
		history->tail->next = memory;
		history->tail = history->tail->next;
	} else {
		//fprintf(stderr, "b\n");
		history->head = memory;
		history->tail = memory;
	}
	if(history->size == history->limit)
	{
		//fprintf(stderr, "c\n");
		CommandMemory* tmp = history->head;
		history->head = history->head->next;
		free(tmp);
	} else {
		//fprintf(stderr, "d\n");
		history->size++;
	}
	fprintf(stderr, "head is now %s\n", history->head->args[0]);
	fprintf(stderr, "tail is now %s\n", history->tail->args[0]);
	return memory;
}

CommandMemory* memoryAt(int index, CommandHistory* history)
{
	int i;
	CommandMemory* ptr = history->head;
	for(i = 0; i < index; i++)
	{
		ptr = ptr->next;
	}
	return ptr;
}


void printHistory(CommandHistory* history)
{
	CommandMemory* curr = history->head;
	char** output[history->size] ;
	int i=0;
	//for(i = 0; i < history->size; i++)
	while(curr)
	{
		output[++i] = curr->args;
		curr = curr->next;
	}
	for(i = history->size - 1; i > 0; i--)
	{
		printf("%d. %s\n", i, *output[i]);
	}
}

#endif