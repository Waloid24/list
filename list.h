#include <stdio.h>
#include <stdlib.h>
#include "my_assert.h"
#include <math.h> //for NAN;

#define STANDART_SIZE 20;
#define LONG_LINE "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"

typedef double elem_t;
const double poison = NAN;

typedef struct {
    size_t prev;
    elem_t data;
    size_t next;
} list_t;

typedef struct {
    size_t size;
    size_t capacity;
    size_t freePlace; //логический номер свободной ячейки

    size_t head; // логический номер первого элемента
    size_t tail; // логический номер последнего элемента

} info_t;

list_t * listConstruct (info_t * listInfo, size_t customCapacity);

void listResize (list_t ** List, info_t * listInfo);
void fillingNewFields (list_t * list, size_t start, size_t finish);
void listDump (info_t info, list_t * List);
void ListDestructor (list_t ** List, info_t * listInfo);
void ListDeleteThisElem (list_t * List, info_t * listInfo, size_t num_of_count);

size_t PushBack (list_t ** List, info_t * listInfo, elem_t newMemb);
size_t PushFront (list_t ** List, info_t * listInfo, elem_t newMemb);
size_t ListInsertBeforeThisNum (list_t ** List, info_t * listInfo, elem_t newMemb, size_t num_of_count);
size_t ListInsertAfterThisNum (list_t ** List, info_t * listInfo, elem_t newMemb, size_t num_of_count);

size_t AccessFirstElem (info_t * info);
size_t AccessLastElem (info_t * info);
size_t ListEmpty (info_t * info);
size_t ListSize (info_t * info);
size_t IndexAfterThisElem (list_t * List, info_t * info, size_t num_of_count);
size_t IndexBeforeThisElem (list_t * List, info_t * listInfo, size_t num_of_count);

elem_t FindElemLogNum (list_t * List, info_t * listInfo, size_t num_of_count);
size_t FindElemByValue (list_t * List, info_t * listInfo, elem_t num_of_count);

list_t * ListSort (list_t ** List, info_t * listInfo);