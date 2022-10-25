#include "list.h"
//#define CLEAR_LOGFILE

#define DUMP_LIST(listElem, info, log)\
        fprintf (log, LONG_LINE);\
        fprintf (log, #listElem "->data = %d\n", listElem->data);\
        fprintf (log, #listElem "->next = %p\n", listElem->next);\
        fprintf (log, #listElem "->prev = %p\n", listElem->prev);\

int main (void)
{
    #if defined CLEAR_LOGFILE
        FILE * clearLog = fopen ("logList.txt", "a");\
        fclose(clearLog); 
    #else 
        FILE * log = fopen ("logList.txt", "a");\
        fprintf (log, "\nNEW START\n");
    #endif

    
    listInfo info = {};

    list_t * listElem = ListConstruct (&info);

    printf ("listElem = %p\n", listElem);

    insertListEnd (5, listElem, &info);
    list_t * listElemTwo = insertListEnd (8, listElem, &info);
    printf ("listElem->data = %d\n", listElem->data);


    DUMP_LIST(listElem, info, log);
    DUMP_LIST(listElemTwo, info, log);

    // void ListInsertEnd (&head, &tail);
    // void ListInsertBegin ();

}

list_t * ListConstruct (listInfo * info)
{
    list_t * elemList = (list_t *) calloc (1, sizeof(list_t));
    info -> head = &(elemList -> data);
    info -> tail = &(elemList -> data);
    info -> size += 1;

    // printf ("elemList = %p\n", elemList);
    // printf ("size = %d\n", info->size);
    return elemList;
}

list_t * insertListEnd (elem_t newElem, list_t * listElem, listInfo * info)
{
    info->size += 1;

    if (listElem->data == 0)
    {
        listElem->data = newElem;
        return listElem;
    }
    else
    {
        list_t * newElem = (list_t *) calloc (1, sizeof(list_t));
        listElem->next = &(newElem->data);
        listElem->prev = &(newElem->data);
        newElem->next  = &(listElem->data);
        newElem->prev  = &(listElem->data);

        info->tail = &(newElem->data);
        return newElem;
    }
}


