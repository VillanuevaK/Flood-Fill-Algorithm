// intlist.h
// Linked list header file 

#ifndef INTLIST_H
#define INTLIST_H

class IntList {

public:

    // ctor and 3 methods already done in intlist.cpp:
    IntList();              // constructor
    void append(int value); // append value at end of list
    void print() const;     // print values separate by ' '
    int count() const;      // return count of values

    // dtor, copy ctor and 6 other METHODS YOU MUST 
    // IMPLEMENT IN intlist.cpp:
    ~IntList();                      // destructor
    IntList(const IntList& source); //copy constructor (deep copy)
    int sum() const;                 // sum of all values
    bool contains(int value) const;  // true if value in list
    int max() const;                 // maximum value
    double average() const;          // average of all values
    void insertFirst(int value);     // insert new first value

    IntList& operator=(const IntList& source); //overloaded 

private:

    // definition of Node structure (DO NOT CHANGE):
    struct Node {
        int info;
        Node *next;
    };

    Node *first; // pointer to first node
};

#endif
