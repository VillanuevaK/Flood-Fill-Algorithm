#include "nodeStack.h"
// #include <iostream>

void pop1(nodeStackPtr& head, int currLoc[])//copies the location info to call by ref array, then deletes top of stack
{
	currLoc[0] = head->row;
	currLoc[1] = head->col;

	nodeStackPtr toDelete = head;
	head = head->link;
	delete toDelete;
}
void pop1(nodeStackPtr& head)//deletes top of stack
{
	nodeStackPtr toDelete = head;
	head = head->link;
	delete toDelete;
}
bool isLLempty(nodeStackPtr& head)
{
	return !head;
}
void emptyStack(nodeStackPtr& head)
{
	while(!isLLempty(head))
	{
		pop1(head);
	}
}
void push1(nodeStackPtr& head, int loc[])
{
	if(isLLempty(head))
	{
		head = new nodeStack;
		head->link = nullptr;
		head->row = loc[0];
		head->col = loc[1];
	}
	else
	{
		nodeStackPtr temp_ptr;
		temp_ptr = new nodeStack;

		temp_ptr->row = loc[0];
		temp_ptr->col = loc[1];

		temp_ptr->link = head;
		head = temp_ptr;
	}
}