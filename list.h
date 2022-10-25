#include <stdio.h>
#include <stdlib.h>

#define LONG_LINE "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"

typedef int elem_t;

const int STANDART_SIZE = 50;
const int poisonForFirst = -2;

typedef struct 
{
    int * prev;
    elem_t data;
    int * next;
} list_t;

typedef struct 
{
    elem_t * head;
    int size;
    elem_t * tail;
} listInfo;

list_t * ListConstruct (listInfo * info);
list_t * insertListEnd (elem_t newElem, list_t * listElem, listInfo * info);

void dumpList (list_t * listElem, listInfo info, FILE * log);
