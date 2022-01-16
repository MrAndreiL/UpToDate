#pragma once

#include "../../config.h"

struct Node
{
	void *data;

	struct Node *next;

	struct Node *previous;
};

struct Node node_constructor(void *data, unsigned long size);

void node_destructor(struct Node *node);
