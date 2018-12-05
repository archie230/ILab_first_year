#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define POISON -100500

//#define List_Fast

const int can = 230;
const int OK = 1;
const int ERR_NULLptr = -11;
const int ERR_can1    = -22;
const int ERR_can2    = -33;
const int ERR_chcksum = -44;
const int ERR_index   = -3;
const int ERR_headtail= -2;
//----------------------------------------------------------------------------------------------------------------------

typedef struct Node{

#ifndef List_Fast
    int can1;
#endif

    int data;
    Node* next;
    Node* prev;

#ifndef List_Fast
    unsigned long int checksum;
    int can2;
#endif

}Node;

typedef struct List{
    int nodes_number;
    Node* head;
    Node* tail;
}List;

//----------------------------------------------------------------------------------------------------------------------
List* List_createlist();

Node* Node_createnode(int);

void  List_deletelist(List*);

void  List_insertafter(Node*, int, List*);

void  List_swipe_currptr_right(Node**, List*);

void  List_swipe_currptr_left(Node**, List*);

void  Node_deletenode(Node*, List*);

int   Node_index(List*, Node*);

void  List_printlist(List*);

void  Node_FillingCHECKSM(Node*);

Node* List_search(List*, int);

int   Node_ok(Node*);

int   List_ok(List*);
//______________________________________________________________________________________________________________________

#include "list.cpp"