#include "list.h"

#define STANDART_SIZE 20;
#define LONG_LINE "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"
#define LINE_ERROR "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n"
#define dumpline(command, ...) fprintf(graph, command __VA_ARGS__)
#define print_html(command, ...) fprintf(graphicBuf, command __VA_ARGS__)

#define LIST_OK(List, listInfo)\
    listValidator (List, listInfo);

void listDestructor (list_t ** List,     info_t * info)
{
    free (*List);
    info->size      = 0;
    info->capacity  = 0;
    info->freePlace = 0;
    info->tail      = 0;
    info->head      = 0;
}

list_t * listConstruct (info_t *  listInfo, size_t   customCapacity)
{
    if (customCapacity == 0 || customCapacity == 1)
    {
        customCapacity = 2;
    }

    list_t * list = (list_t *) calloc (customCapacity, sizeof(list_t));
    MY_ASSERT (list == nullptr, "Unable to allocate the memory");

    listInfo->tail = listInfo->head = 1;
    listInfo->capacity              = customCapacity;
    listInfo->freePlace             = 1;
    listInfo->size                  = 0;
    fillingNewFields (list, listInfo->head, listInfo->capacity - 1);

    return list;
}

size_t listPushBack (list_t ** List, info_t * listInfo, elem_t newMemb)
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
    else
    {
        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].next = listInfo->head;
        (*List)[newPlace].prev = oldPlace;

        (*List)[oldPlace].next = newPlace;
    }

    listInfo->tail = newPlace;
    (*List)[firstMem].prev = listInfo->tail;

    return newPlace;

}

void listResize (list_t ** List, info_t * listInfo)
{
    list_t * newPlace = (list_t *) realloc (*List, listInfo->capacity * 2 * sizeof (list_t));
    MY_ASSERT (newPlace == nullptr, "Unable to allocate new memory");

    (*List) = newPlace;

    fillingNewFields(*List, listInfo->capacity, (listInfo->capacity * 2) - 1);

    listInfo->freePlace = listInfo->capacity;
    listInfo->capacity = listInfo->capacity * 2;
}

void fillingNewFields (list_t *  list, size_t start, size_t finish)
{
    for (int i = start; i < finish; i++)
    {
        list[i].prev = -1;
        list[i].data = POISON;
        list[i].next = i+1;
    }
    list[finish].next = 0;
    list[finish].prev = -1;
    list[finish].data = POISON;
}

void listDump (info_t * info, list_t ** List)
{
    #ifdef CLEAR_LOGFILE
         FILE * clear_log = fopen ("log.txt", "w");  \
         MY_ASSERT (clear_log == nullptr, "There is no access to the file");\
         fclose (clear_log);
    #endif

    FILE * log = fopen ("log.txt", "a");
    MY_ASSERT (log == nullptr, "Unable to access the file");

    fprintf (log, "\n\nNEW START\n");
    fprintf (log, LONG_LINE);
    fprintf (log, "info.size      = %zu\n", info->size);
    fprintf (log, "info.capacity  = %zu\n", info->capacity);
    fprintf (log, "info.freePlace = %zu\n", info->freePlace);
    fprintf (log, "info.head      = %zu\n", info->head);
    fprintf (log, "info.tail      = %zu\n\n", info->tail);

    fprintf (log, "index = ");

    for (int i = 0; i < info->capacity; i++)
    {
        fprintf (log, "%4d ", i);
    }
    fprintf (log, "\n");

    fprintf (log, "data  = ");
    for (int i = 0; i < info->capacity; i++)
    {
        fprintf (log, "%4.1lf ", (*List)[i].data);
    }
    fprintf (log, "\n");

    fprintf (log, "next  = ");
    for (int i = 0; i < info->capacity; i++)
    {
        fprintf (log, "%4zu ", (*List)[i].next);
    }
    fprintf (log, "\n");

    fprintf (log, "prev  = ");
    for (int i = 0; i < info->capacity; i++)
    {
        fprintf (log, "%4ld ", (*List)[i].prev);
    }
    fprintf (log, "\n");

    listGraphviz (List, info);

    fflush(log);
    fclose (log);
}

size_t listPushFront (list_t ** List, info_t * listInfo, elem_t newMemb)
{
    listInfo->size++;
    if (listInfo->size >= listInfo->capacity)
    {
        listResize (List, listInfo);
    }

    size_t newPlace = listInfo->freePlace;
    size_t lastMem = listInfo->tail;
    size_t firstMem = listInfo->head;

    listInfo->freePlace = (*List)[listInfo->freePlace].next;

    if (listInfo->size == 1)
    {
        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].next = listInfo->head;
        (*List)[newPlace].prev = 1;
    }
    else
    {
        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].next = firstMem;
        (*List)[newPlace].prev = lastMem;

        (*List)[lastMem].next = newPlace;
    }

    listInfo->head = newPlace;
    (*List)[firstMem].prev = listInfo->head;

    return newPlace;
}

size_t listInsertBeforeThisNum (list_t ** List, info_t * listInfo, elem_t newMemb, size_t num_of_count)
{

    if (listInfo->size == 0 || listInfo->size == 1 || num_of_count == 0 || num_of_count == 1)
        return listPushFront (List, listInfo, newMemb);
    else
    {
        if (listInfo->size >= listInfo->capacity)
        {
            listResize (List, listInfo);
        }

        if (num_of_count > listInfo->size)
        {
            return listPushBack(List, listInfo, newMemb);
        }

        listInfo->size++;

        size_t indexThisNum = (*List)[listInfo->head].next;
        for (int i = 1; i < listInfo->size && i < num_of_count-1; i++)
        {
            indexThisNum = (*List)[indexThisNum].next;
        }

        size_t newPlace = listInfo->freePlace;
        size_t countBeforeNewElem = (*List)[indexThisNum].prev;
        size_t countAfterNewElem  = indexThisNum;
        listInfo->freePlace = (*List)[listInfo->freePlace].next;

        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].prev = countBeforeNewElem;
        (*List)[newPlace].next = countAfterNewElem;

        (*List)[countBeforeNewElem].next = newPlace;
        (*List)[countAfterNewElem].prev  = newPlace; //.........

        return indexThisNum;
    }
}

size_t listInsertAfterThisNum (list_t ** List,     info_t * listInfo, elem_t newMemb, size_t num_of_count)
{
    if (listInfo->size == 0 || listInfo->size == 1)
        return listPushFront (List, listInfo, newMemb);
    else
    {
        if (listInfo->size >= listInfo->capacity)
        {
            listResize (List, listInfo);
        }

        if (num_of_count >= listInfo->size)
        {
            return listPushBack(List, listInfo, newMemb);
        }

        listInfo->size++;

        size_t indexThisNum = (*List)[listInfo->head].next;
        for (int i = 1; i < listInfo->size && i < num_of_count; i++)
        {
            indexThisNum = (*List)[indexThisNum].next;
        }

        size_t newPlace = listInfo->freePlace;
        size_t countBeforeNewElem = (*List)[indexThisNum].prev;
        size_t countAfterNewElem  = indexThisNum;
        listInfo->freePlace = (*List)[listInfo->freePlace].next;

        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].prev = countBeforeNewElem;
        (*List)[newPlace].next = countAfterNewElem;

        (*List)[countBeforeNewElem].next = newPlace;
        (*List)[countAfterNewElem].prev  = newPlace; //.........

        return indexThisNum;
    }
}

size_t listAccessFirstElem (info_t * info)
{
    MY_ASSERT (info == nullptr, "There is no access to list");

    return info->head;
}

size_t listAccessLastElem (info_t * info)
{
    MY_ASSERT (info == nullptr, "There is no access to list");

    return info->tail;
}

size_t listEmpty (info_t * info)
{
    MY_ASSERT (info == nullptr, "There is no access to list");

    return info->size;
}

size_t listSize (info_t * info)
{
    MY_ASSERT (info == nullptr, "There is no access to list");

    return info->size;
}

size_t listIndexAfterThisElem (list_t * List, info_t * listInfo, size_t num_of_count)
{
    MY_ASSERT (List == nullptr, "There is no access to list");

    size_t indexThisNum = List[listInfo->head].next;
    for (int i = 1; i < listInfo->size && i < num_of_count-1; i++)
    {
        indexThisNum = List[indexThisNum].next;
    }
    printf ("this number is %lf\n", List[indexThisNum].data);

    return List[indexThisNum].next;
}

size_t listIndexBeforeThisElem (list_t * List, info_t * listInfo, size_t num_of_count)
{
    MY_ASSERT (List == nullptr, "There is no access to list");

    size_t indexThisNum = List[listInfo->head].next;
    for (int i = 1; i < listInfo->size && i < num_of_count-1; i++)
    {
        indexThisNum = List[indexThisNum].next;
    }
    return List[indexThisNum].prev;
}

void listDeleteThisElem (list_t * List, info_t * listInfo, size_t num_of_count)
{
    MY_ASSERT (List == nullptr, "There is no access to list");
    MY_ASSERT (listInfo == nullptr, "There is no access to information about list");

    if (num_of_count >= listInfo->size)
    {
        num_of_count = listInfo->size;
    }

    size_t indexThisNum = List[listInfo->head].next;

    for (int i = 1; i < listInfo->size && i < num_of_count-1; i++)
    {
        indexThisNum = List[indexThisNum].next;
    }

    if (List[indexThisNum].prev == List[listInfo->head].prev) //There is the head of the list
    {
        listInfo->head = List[listInfo->head].next;
    }
    if (List[indexThisNum].prev == List[listInfo->tail].prev) //There is the tail of the list
    {
        listInfo->tail = List[listInfo->tail].prev;
    }

    List[indexThisNum].data = NAN;
    List[indexThisNum].prev = -1;
    List[indexThisNum].next = listInfo->freePlace;

    listInfo->size--;

    listInfo->freePlace = indexThisNum;
}

elem_t listFindElemLogNum (list_t * List, info_t * listInfo, size_t num_of_count)
{
    MY_ASSERT (List == nullptr, "There is no access to list");
    MY_ASSERT (listInfo == nullptr, "There is no access to information about list");

    size_t indexThisNum = List[listInfo->head].next;

    for (int i = 1; i < listInfo->size && i < num_of_count-1; i++)
    {
        indexThisNum = List[indexThisNum].next;
    }

    return List[indexThisNum].data;
}

size_t listFindElemByValue (list_t * List, info_t * listInfo, elem_t num_of_count)
{
    MY_ASSERT (List == nullptr, "There is no access to list");
    MY_ASSERT (listInfo == nullptr, "There is no access to information about list");

    size_t indexThisNum = List[listInfo->head].next;

    printf ("\n");

    for (int i = 1; i < listInfo->size; i++)
    {
        if (List[indexThisNum].data == num_of_count)
            return indexThisNum;
        indexThisNum = List[indexThisNum].next;

        printf ("List[%zu].data = %lf\n", indexThisNum, List[indexThisNum].data);
    }

    printf ("This number doesn't find\n");
    printf ("\n");

    return -1;
}

list_t * listSort (list_t ** List, info_t * listInfo)
{
    MY_ASSERT (*List == nullptr, "There is no access to list");
    list_t * newList = (list_t *) calloc (listInfo->capacity, sizeof(list_t));
    MY_ASSERT (newList == nullptr, "Unable to allocate memory");

    newList[0] = (*List)[0];

    size_t start = listInfo->head;
    for (int i = 1; i <= listInfo->size; i++)
    {
        newList[i] = (*List)[start];
        start = (*List)[start].next;
        newList[i].next = i + 1;
        newList[i].prev = i - 1;
    }

    for (int i = (listInfo->size)+1; i < listInfo->capacity; i++)
    {
        newList[i] = (*List)[i];
        newList[i].prev = -1;
    }

    newList[1].prev = listInfo->size;
    newList[listInfo->size].next = 1;

    listInfo->head = 1;
    listInfo->tail = listInfo->size;

    free (*List);

    return newList;
}

void listGraphviz (list_t ** List, info_t * listInfo)
{
    FILE * graph = fopen ("ListlistGraphviz.dot", "w");
    MY_ASSERT (graph == nullptr, "There is no access to file with graph");

    *List = listSort(List, listInfo);

    dumpline("digraph test{\n");
    dumpline("size =\"8.5\";\n");
    dumpline("rankdir = LR;\n");

    dumpline("node [shape=record];\n");

    dumpline("  table_0 [style = filled, fillcolor=\"#fde0dd\", label=\"0|%.2lf|<n0>n: %zu|<p0>p: %zu\"];\n",, (*List)[0].data, (*List)[0].next, (*List)[0].prev);
    size_t current = listInfo->head;
    for (int i = 1; i <= listInfo->size; i++)
    {
        dumpline("  table_%zu [style = filled, fillcolor=\"#f0f0f0\", label=\"%d|%.2lf|<n%zu>n: %zu|<p%zu>p: %zu\"];\n",, (*List)[current].next, i, (*List)[current].data, (*List)[current].next, (*List)[current].next, (*List)[current].prev, (*List)[current].prev);
        current = (*List)[current].next;
    }

    dumpline("infoList [style = filled, fillcolor=\"#fee8c8\", label=\"capacity = %zu|size = %zu|head = %zu|tail = %zu\"];\n",, listInfo->capacity, listInfo->size, listInfo->head, listInfo->tail);

    current = listInfo->head;
    dumpline("  infoList->table_0 [style=invis];\n");
    dumpline("  table_0:n0->table_%zu:n%zu [style=invis];\n",, (*List)[current].next, (*List)[current].next);
    size_t nextBox = 0;
    for (int i = 1; i < listInfo->size; i++)
    {
        nextBox = (*List)[current].next;
        dumpline("  table_%zu:n%zu->table_%zu:n%zu;\n",, (*List)[current].next, (*List)[current].next, (*List)[nextBox].next, (*List)[nextBox].next);
        current = (*List)[current].next;
    }

    dumpline("\n");

    current = listInfo->tail;
    nextBox = 0;
    for (int i = 1; i < listInfo->size; i++)
    {
        nextBox = (*List)[current].prev;
        dumpline("  table_%zu:p%zu->table_%zu:p%zu;\n",, (*List)[current].next, (*List)[current].prev, (*List)[nextBox].next, (*List)[nextBox].prev);
        current = (*List)[current].prev;
    }

    dumpline("}\n\n");

    fflush (graph);
    fclose (graph);

    listCmdLine(NUMBER_OF_GRAPHIC_DUMPS);

    listCreateHTMLfile (&NUMBER_OF_GRAPHIC_DUMPS);

}

void listValidator (list_t * List, info_t * listInfo)
{
    size_t current = listInfo->head;
    size_t nextMem = List[current].next;
    size_t prevMem = List[current].prev;

    for (int i = 0; i < listInfo->size; i++)
    {
        if (List[List[nextMem].prev].data == List[current].data && List[List[prevMem].next].data == List[current].data)
        {
            current = List[current].next;
            nextMem = List[current].next;
            prevMem = List[current].prev;
        }
        else
        {
            FILE * log = fopen ("log.txt", "a");
            fprintf (log, LINE_ERROR);
            fprintf (log, "The element in logical number %zu has an incorrect value\n", current);
            fprintf (log, "The current number is %.2lf\n", List[current].data);
            fprintf (log, "The next element is %.2lf\n", List[List[prevMem].next].data);
            fprintf (log, "The previous element is %.2lf\n", List[List[nextMem].prev].data);
            fprintf (log, LINE_ERROR);

            listDump (listInfo, &List);
        }
    }
}

void listCmdLine (int num)
{
    char buf[100] = {};
    sprintf(buf, "dot -T png -o graph%d.png ListlistGraphviz.dot", num);\
    system(buf);
}

void listCreateHTMLfile (int * num)
{
    FILE * graphicBuf = fopen("HTMLBuf.html", "a");
    MY_ASSERT (graphicBuf == nullptr, "Unable to access to file");

    char buf[60] = {};
    sprintf (buf, "graph%d.png", *num);

    print_html("<pre>\n");
    print_html("<img src=\"%s\" alt=\"dump №%d\"/>\n",, buf, *num);
    print_html("</pre>\n");

    (*num)++;

    fflush (graphicBuf);
    fclose (graphicBuf);
}
