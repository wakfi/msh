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
		free(head);
		head = tmp;
	}
	free(history);
}

CommandHistory* insert_memory(char* command, CommandHistory* history)
{
	CommandMemory* memory = newCommandMemory(command);
	if(history->tail)
	{
		history->tail->next = memory;
		history->tail = history->tail->next;
	} else {
		history->head = memory;
		history->tail = memory;
	}
	if(history->size == history->limit)
	{
		CommandMemory* tmp = history->head;
		history->head = history->head->next;
		free(tmp);
	} else {
		history->size++;
	}
}

CommandMemory* memory_at(int index, CommandHistory* history)
{
	int i;
	CommandMemory* ptr = history->head;
	for(i = 0; i < index; i++)
	{
		ptr = ptr->next;
	}
	return ptr;
}

#endif