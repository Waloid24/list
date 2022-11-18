#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "my_assert.h"
#include <math.h>

typedef double elem_t;
const double POISON = NAN;

static int NUMBER_OF_GRAPHIC_DUMPS = 1;

typedef struct {
    size_t prev;
    elem_t data;
    size_t next;
} list_t;

typedef struct {
    size_t size;
    size_t capacity;
    size_t freePlace;

    size_t head;
    size_t tail;

} info_t;

list_t * listConstruct              (info_t * listInfo, size_t customCapacity);

void     listResize                 (list_t ** List, info_t * listInfo);
void     fillingNewFields           (list_t *  list, size_t   start,    size_t finish);
void     listDump                   (info_t *   info, list_t ** List);
void     listDestructor             (list_t ** List, info_t * listInfo);
void     listDeleteThisElem         (list_t *  List, info_t * listInfo, size_t num_of_count);

size_t   listPushBack               (list_t ** List, info_t * listInfo, elem_t newMemb);
size_t   listPushFront              (list_t ** List, info_t * listInfo, elem_t newMemb);
size_t   listInsertBeforeThisNum    (list_t ** List, info_t * listInfo, elem_t newMemb, size_t num_of_count);
size_t   listInsertAfterThisNum     (list_t ** List, info_t * listInfo, elem_t newMemb, size_t num_of_count);

size_t   listAccessFirstElem        (info_t *  info);
size_t   listAccessLastElem         (info_t *  info);
size_t   listEmpty                  (info_t *  info);
size_t   listSize                   (info_t *  info);
size_t   listIndexAfterThisElem     (list_t *  List, info_t * info,     size_t num_of_count);
size_t   listIndexBeforeThisElem    (list_t *  List, info_t * listInfo, size_t num_of_count);

elem_t   listFindElemLogNum         (list_t *  List, info_t * listInfo, size_t num_of_count);
size_t   listFindElemByValue        (list_t *  List, info_t * listInfo, elem_t num_of_count);

list_t * listSort                   (list_t ** List, info_t  *  listInfo);

void     listGraphviz               (list_t ** List, info_t * listInfo);
void     listValidator              (list_t *  List, info_t * listInfo);
void     listCreateHTMLfile         (int *     num);
void     listCmdLine                (int       num);

#endif
