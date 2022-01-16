#pragma once

#include "../Node/node.h"

struct LinkedList
{
	struct Node *head;
	
	int length;

	/* Public member functions */
	// function to insert a new node onto the list.
	void (*insert)(struct LinkedList *linked_list, int index, void *data, unsigned long size);
	// function to remove a node.
	void (*remove)(struct LinkedList *linked_list, int index);
	// access a node.
	void* (*retrieve)(struct LinkedList *linked_list, int index);
};

struct LinkedList linked_list_constructor(void);
void linked_list_destructor(struct LinkedList *linked_list);
