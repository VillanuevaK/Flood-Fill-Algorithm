// intlist.cpp
// Implements class IntList
// Kevin Villanueva | 2/8/18

#include "intlist.h"

#include <iostream>
using std::cout;


// copy constructor
IntList::IntList(const IntList& source) {// ERR if source is null
	Node* tempThis  = first;
	Node* tempOther = source.first;
    while(tempOther!=NULL)
    {
    	tempThis->info = tempOther->info;
    	tempThis->next = tempOther->next;
    	tempOther      = tempOther->next;
    	tempThis       = tempThis->next;
    }
    return;
}

// destructor deletes all nodes
IntList::~IntList() {
	Node* deleteThis = first;
    while(first!=NULL)
    {
    	first = first->next;
    	delete deleteThis;
    	deleteThis = first;
    }
    return;
}


// return sum of values in list
int IntList::sum() const {
	Node* temp = first;
	int sum = 0;
	while(temp!=NULL)
	{
		sum += temp->info;
		temp = temp->next;
	}
    return sum; 
}

// returns true if value is in the list; false if not
bool IntList::contains(int value) const {
	Node* temp = first;
	while(temp!=NULL)
	{
		if(value==temp->info)
			return true;
		temp = temp->next;
	}
    return false; 
}

// returns maximum value in list, or 0 if empty list
int IntList::max() const {
	if(first==NULL) return 0;
	Node* temp = first;
	int max = -9999;
	while(temp!=NULL)
	{
		max = (temp->info > max)? temp->info:max;
		temp = temp->next;
	}
    return max; 
}

// returns average (arithmetic mean) of all values, or
// 0 if list is empty
double IntList::average() const {
	if(first==NULL) return 0;
	Node* temp = first;
	int cnt = 0;
	while(temp!=NULL)
	{
		cnt++;
		temp = temp->next;
	}
	double avg = sum()/(static_cast<double> (cnt));
    return avg; 
}

// inserts value as new node at beginning of list
void IntList::insertFirst(int value) {
    Node* newNode = new Node;
    newNode->info = value;
    newNode->next = first;
    first = newNode;
}

//Assignment operator should copy the list from the source
//to this list, deleting/replacing any existing nodes
IntList& IntList::operator=(const IntList& source){// ERR is it actually adding all the necesaary stuff?
    Node* deleteThis = first;
    while(first!=NULL)
    {
    	first = first->next;
    	delete deleteThis;
    	deleteThis = first;
    }

    Node* tempThis  = first;
	Node* tempOther = source.first;
    while(tempOther!=NULL)
    {
    	tempThis->info = tempOther->info;
    	tempThis->next = tempOther->next;
    	tempOther      = tempOther->next;
    	tempThis       = tempThis->next;
    }
    return *this;
}



// DO NOT CHANGE ANYTHING BELOW (READ IT THOUGH)

// constructor sets up empty list
IntList::IntList() : first(0) { }


// append value at end of list
void IntList::append(int value) {
    if (first == 0) { // empty list
        first = new Node;
        first->info = value;
        first->next = 0;
    }
    else {
        Node *n = first;
        while (n->next) // not last node yet
            n = n->next;
        n->next = new Node;
        n->next->info = value;
        n->next->next = 0;
    }
}

// print values enclose in [], separated by spaces
void IntList::print() const {
    Node *n = first;
    cout << '[';
    while (n) {
        cout << n->info;
        if (n->next)
            cout << " ";
        n = n->next;
    }
    cout << ']';
}

// return count of values
int IntList::count() const {
    int result = 0;
    Node *n = first;
    while (n) {
        ++result;
        n = n->next;
    }
    return result;
}
