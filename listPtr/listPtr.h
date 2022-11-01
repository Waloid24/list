#include <stdio.h>
#include <math.h>
#include "my_assert.h"

//====================================variables used=====================================

typedef double elem_t;
const double poison NAN;
static int times_push_html = 1;

#define LONG_LINE "###################################################################\n"
#define dumpline(string, ...) fprintf (ptrGraph, string __VA_ARGS__);
#define print_html(command, ...) fprintf(graphicBuf, command __VA_ARGS__);

//=================================defining structures===================================

struct listElement
{
    listElement * prev;
    elem_t   data;
    listElement * next;
};

typedef struct 
{
    struct listElement * ptrToList;

    //size_t capacity;
    size_t size;
    // elem_t * tail;
    // elem_t * head;
} listPtr_t;

//================================main functions========================================

struct listElement * ListConstruct    (listPtr_t * list);
struct listElement * PushBack         (listPtr_t * list, elem_t newElemValue);
struct listElement * PushFront        (listPtr_t * list, elem_t newElemValue);
struct listElement * PushFirstElem    (listPtr_t * list, elem_t newElemValue);
struct listElement * ListInsertBefore (listPtr_t * list, struct listElement * currentElem, elem_t newElemValue);
struct listElement * ListInsertAfter  (listPtr_t * list, struct listElement * currentElem, elem_t newElemValue);
void                 listDestructor   (listPtr_t * list);
void                 listDeleteElem   (listPtr_t * list, struct listElement * currentElem);

struct listElement * validator        (const listPtr_t * list);
void                 listGraphviz     (const listPtr_t * list);

//===============================support function=======================================

struct listElement * ptrListElemNext      (struct listElement * currentElem);
void                 listDump             (const listPtr_t    * list);
struct listElement * AccessLastElem       (const listPtr_t    * list);
struct listElement * AccessFirstElem      (const listPtr_t    * list);
struct listElement * listFindElemByValue  (const listPtr_t    * list, elem_t value);
struct listElement * listFindElemByNumber (const listPtr_t    * list, elem_t number);

void cmdLine        (int num);
void createHTMLfile (int * num);

//======================================================================================


