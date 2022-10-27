#include "list.h"

//#define CLEAR_LOGFILE

int main (void)
{
    info_t info = {};
    list_t * List = listConstruct (&info, 3); //фактически, сейчас это список свободных элементов
    printf ("Ctor\n");
    listDump (info, List);
    printf ("info.size = %zu\n", info.size);
    // printf ("1\n");
    PushBack (&List, &info, 6);
    listDump (info, List);

    PushBack (&List, &info, 10);
    listDump (info, List);

    PushBack (&List, &info, 3);
    listDump (info, List);
    // printf ("4\n");
}



list_t * listConstruct (info_t * listInfo, size_t customCapacity)
{
    if (customCapacity == 0 || customCapacity == 1)
    {
        customCapacity = 2;
    }
    
    list_t * list = (list_t *) calloc (customCapacity, sizeof(list_t));
    // printf ("looool\n");
    MY_ASSERT (list == nullptr, "Unable to allocate the memory");

    listInfo->tail = listInfo->head = 1;
    listInfo->capacity              = customCapacity;
    listInfo->freePlace             = 1;
    listInfo->size                  = 0;
    printf ("Before in func --listConstruct--\n");
    fillingNewFields (list, listInfo->head, listInfo->capacity - 1);

    return list;
}


size_t PushBack (list_t ** List, info_t * listInfo, elem_t newMemb)
{
    listInfo->size++;
    if (listInfo->size >= listInfo->capacity)
    {
        listResize (List, listInfo);
    }

    size_t newPlace = listInfo->freePlace;
    size_t oldPlace = listInfo->tail;
    size_t firstMem = listInfo->head;

    listInfo->freePlace = (*List)[listInfo->freePlace].next;
    

    if (listInfo->size == 1)
    {
        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].next = listInfo->head;
        (*List)[newPlace].prev = 1; 
    }
    else /*if (listInfo->size == 2)*/ 
    {
        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].next = listInfo->head;
        (*List)[newPlace].prev = oldPlace;

        (*List)[oldPlace].next = newPlace;
    }
    // else
    // {

    // }

    listInfo->tail = newPlace;
    (*List)[firstMem].prev = listInfo->tail;

    return newPlace;
    
}

// size_t findNewFreePlace (size_t * list, info_t listInfo);
// {
//     MY_ASSERT (list == nullptr, "There is no access to the list");

//     return listInfo.freePlace;
// }

void listResize (list_t ** List, info_t * listInfo)
{
    list_t * newPlace = (list_t *) realloc (*List, listInfo->capacity * 2 * sizeof (list_t));
    MY_ASSERT (newPlace == nullptr, "Unable to allocate new memory");
    
    (*List) = newPlace;

    fillingNewFields(*List, listInfo->capacity, (listInfo->capacity * 2) - 1);

    listInfo->freePlace = listInfo->capacity;
    listInfo->capacity = listInfo->capacity * 2;    
}

void fillingNewFields (list_t * list, size_t start, size_t finish) //заполняет от start до finish включительно
{
    for (int i = start; i < finish; i++)
    {
        list[i].prev = -1;
        list[i].data = poison;
        list[i].next = i+1;
    }
    list[finish].next = 0;  
    list[finish].prev = -1;
    list[finish].data = poison;
}

void listDump (info_t info, list_t * List)
{
    //printf ("4444444444444444444444\n");
    #ifdef CLEAR_LOGFILE
         FILE * clear_log = fopen ("log.txt", "w");  \
         MY_ASSERT (clear_log == nullptr, "There is no access to the file");\
         fclose (clear_log);
    #endif

    // printf ("start\n");

    FILE * log = fopen ("log.txt", "a");

    fprintf (log, "\n\nNEW START\n");
    fprintf (log, LONG_LINE);
    fprintf (log, "info.size      = %zu\n", info.size);
    // printf ("in listDump: info.size = %zu\n", info.size);
    fprintf (log, "info.capacity  = %zu\n", info.capacity);
    fprintf (log, "info.freePlace = %zu\n", info.freePlace);
    fprintf (log, "info.head      = %zu\n", info.head);
    fprintf (log, "info.tail      = %zu\n\n", info.tail);

    fprintf (log, "index = ");

    // printf ("111\n");

    for (int i = 0; i < info.capacity; i++)
    {
        fprintf (log, "%4d ", i);
    }
    fprintf (log, "\n");

    // printf ("222\n");

    fprintf (log, "data  = ");
    for (int i = 0; i < info.capacity; i++)
    {
        fprintf (log, "%4.1lf ", List[i].data);
    }
    fprintf (log, "\n");
    
    // printf ("333\n");   

    fprintf (log, "next  = ");
    for (int i = 0; i < info.capacity; i++)
    {
        fprintf (log, "%4zu ", List[i].next);
    }
    fprintf (log, "\n");

    // printf ("444\n");

    fprintf (log, "prev  = ");
    for (int i = 0; i < info.capacity; i++)
    {
        fprintf (log, "%4zu ", List[i].prev);
    }
    fprintf (log, "\n");

    //printf ("555\n");

    fflush(log);

    fclose (log);
    // printf ("666\n");
}  
    
// size_t PushFront (list_t ** List, info_t * listInfo, elem_t newMemb)
// {
    
// } 
