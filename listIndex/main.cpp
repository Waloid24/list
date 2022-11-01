#include "list.h"

#define CLEAR_LOGFILE

int main (void)
{
    info_t info = {};
    list_t * List = listConstruct (&info, 3); 
    //printf ("Ctor\n");
    //listDump (info, List);
    //printf ("info.size = %zu\n", info.size);
    // printf ("1\n");
    PushFront (&List, &info, 31);
    //listDump (info, List);

    PushBack (&List, &info, 87);
    //listDump (info, List);


    PushFront (&List, &info, 65);
    //listDump (info, List);

    // PushBack (&List, &info, 7);
    // listDump (info, List);

    ListInsertBeforeThisNum (&List, &info, 50, 2);
    listDump (&info, &List);


    ListInsertAfterThisNum (&List, &info, 71, 4);
    listDump (&info, &List);

    PushFront (&List, &info, 23);
    listDump (&info, &List);

}

void     ListDestructor          (list_t ** List,     info_t * info)
{
    free (*List);
    info->size      = 0;
    info->capacity  = 0;
    info->freePlace = 0;
    info->tail      = 0;
    info->head      = 0;
}

list_t * listConstruct           (info_t *  listInfo, size_t   customCapacity)
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
    //printf ("Before in func --listConstruct--\n");
    fillingNewFields (list, listInfo->head, listInfo->capacity - 1);

    return list;
}

size_t   PushBack                (list_t ** List,     info_t * listInfo, elem_t newMemb)
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
    //printf ("freePlace = %zu\n", listInfo->freePlace);
    

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

void     listResize              (list_t ** List,     info_t * listInfo)
{
    list_t * newPlace = (list_t *) realloc (*List, listInfo->capacity * 2 * sizeof (list_t));
    MY_ASSERT (newPlace == nullptr, "Unable to allocate new memory");
    
    (*List) = newPlace;

    fillingNewFields(*List, listInfo->capacity, (listInfo->capacity * 2) - 1);

    listInfo->freePlace = listInfo->capacity;
    listInfo->capacity = listInfo->capacity * 2;    
}

void     fillingNewFields        (list_t *  list,     size_t   start,    size_t finish) 
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

void     listDump                (info_t *  info,     list_t ** List)
{
    //printf ("4444444444444444444444\n");
    #ifdef CLEAR_LOGFILE
         FILE * clear_log = fopen ("log.txt", "w");  \
         MY_ASSERT (clear_log == nullptr, "There is no access to the file");\
         fclose (clear_log);
    #endif

    // printf ("start\n");

    FILE * log = fopen ("log.txt", "a");
    MY_ASSERT (log == nullptr, "Unable to access the file");

    fprintf (log, "\n\nNEW START\n");
    fprintf (log, LONG_LINE);
    fprintf (log, "info.size      = %zu\n", info->size);
    // printf ("in listDump: info.size = %zu\n", info.size);
    fprintf (log, "info.capacity  = %zu\n", info->capacity);
    fprintf (log, "info.freePlace = %zu\n", info->freePlace);
    fprintf (log, "info.head      = %zu\n", info->head);
    fprintf (log, "info.tail      = %zu\n\n", info->tail);

    fprintf (log, "index = ");

    // printf ("111\n");

    for (int i = 0; i < info->capacity; i++)
    {
        fprintf (log, "%4d ", i);
    }
    fprintf (log, "\n");

    // printf ("222\n");

    fprintf (log, "data  = ");
    for (int i = 0; i < info->capacity; i++)
    {
        fprintf (log, "%4.1lf ", (*List)[i].data);
    }
    fprintf (log, "\n");
    
    // printf ("333\n");   

    fprintf (log, "next  = ");
    for (int i = 0; i < info->capacity; i++)
    {
        fprintf (log, "%4zu ", (*List)[i].next);
    }
    fprintf (log, "\n");

    // printf ("444\n");

    fprintf (log, "prev  = ");
    for (int i = 0; i < info->capacity; i++)
    {
        fprintf (log, "%4ld ", (*List)[i].prev);
    }
    fprintf (log, "\n");

    //printf ("555\n");

    Graphviz (List, info);

    fflush(log);
    fclose (log);
}  
    
size_t   PushFront               (list_t ** List,     info_t * listInfo, elem_t newMemb)
{
    listInfo->size++;
    if (listInfo->size >= listInfo->capacity)
    {
        listResize (List, listInfo);
    }

    size_t newPlace = listInfo->freePlace;
    size_t lastMem = listInfo->tail; //lastMem
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

size_t   ListInsertBeforeThisNum (list_t ** List,     info_t * listInfo, elem_t newMemb, size_t num_of_count)
{
    
    if (listInfo->size == 0 || listInfo->size == 1 || num_of_count == 0 || num_of_count == 1)
        return PushFront (List, listInfo, newMemb);
    else 
    {
        if (listInfo->size >= listInfo->capacity)
        {
            listResize (List, listInfo);
        }

        if (num_of_count > listInfo->size)
        {
            return PushBack(List, listInfo, newMemb);
        }

        listInfo->size++;

        size_t indexThisNum = (*List)[listInfo->head].next;
        for (int i = 1; i < listInfo->size && i < num_of_count-1; i++)
        {
            indexThisNum = (*List)[indexThisNum].next;
        }
        //printf ("this number is %lf\n", (*List)[indexThisNum].data);
        
        size_t newPlace = listInfo->freePlace;
        size_t countBeforeNewElem = (*List)[indexThisNum].prev;
        size_t countAfterNewElem  = indexThisNum;
        listInfo->freePlace = (*List)[listInfo->freePlace].next;

        //printf ("count before this elem is %lf\n", (*List)[countBeforeNewElem].data);
        //printf ("count after new elem is %lf\n", (*List)[countAfterNewElem].data);

        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].prev = countBeforeNewElem;
        (*List)[newPlace].next = countAfterNewElem;

        (*List)[countBeforeNewElem].next = newPlace;
        (*List)[countAfterNewElem].prev  = newPlace; //.........

        return indexThisNum;
    }
}

size_t   ListInsertAfterThisNum  (list_t ** List,     info_t * listInfo, elem_t newMemb, size_t num_of_count)
{
    if (listInfo->size == 0 || listInfo->size == 1)
        return PushFront (List, listInfo, newMemb);
    else 
    {
        if (listInfo->size >= listInfo->capacity)
        {
            listResize (List, listInfo);
        }

        //printf ("num_of_count = %zu\n", num_of_count);
        //printf ("listInfo->size = %zu\n", listInfo->size);

        if (num_of_count >= listInfo->size)
        {
            //printf ("YYEESS\n");
            return PushBack(List, listInfo, newMemb);
        }

        listInfo->size++;

        size_t indexThisNum = (*List)[listInfo->head].next;
        for (int i = 1; i < listInfo->size && i < num_of_count; i++)
        {
            indexThisNum = (*List)[indexThisNum].next;
        }
        //printf ("this number is %lf\n", (*List)[indexThisNum].data);
        
        size_t newPlace = listInfo->freePlace;
        size_t countBeforeNewElem = (*List)[indexThisNum].prev;
        size_t countAfterNewElem  = indexThisNum;
        listInfo->freePlace = (*List)[listInfo->freePlace].next;

        //printf ("count before this elem is %lf\n", (*List)[countBeforeNewElem].data);
        //printf ("count after new elem is %lf\n",   (*List)[countAfterNewElem].data );

        (*List)[newPlace].data = newMemb;
        (*List)[newPlace].prev = countBeforeNewElem;
        (*List)[newPlace].next = countAfterNewElem;

        (*List)[countBeforeNewElem].next = newPlace;
        (*List)[countAfterNewElem].prev  = newPlace; //.........

        return indexThisNum;
    }
}

size_t   AccessFirstElem         (info_t *  info)
{
    MY_ASSERT (info == nullptr, "There is no access to list");

    return info->head;
}

size_t   AccessLastElem          (info_t *  info)
{
    MY_ASSERT (info == nullptr, "There is no access to list");

    return info->tail;
}

size_t      ListEmpty               (info_t *  info)
{
    MY_ASSERT (info == nullptr, "There is no access to list");

    return info->size;
}

size_t      ListSize                (info_t *  info)
{
    MY_ASSERT (info == nullptr, "There is no access to list");

    return info->size;
}

size_t   IndexAfterThisElem      (list_t * List,      info_t * listInfo, size_t num_of_count)
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

size_t   IndexBeforeThisElem     (list_t * List,      info_t * listInfo, size_t num_of_count)
{
    MY_ASSERT (List == nullptr, "There is no access to list");

    size_t indexThisNum = List[listInfo->head].next;
    //printf ("indexThisNum before = %zu\n", indexThisNum);
    for (int i = 1; i < listInfo->size && i < num_of_count-1; i++)
    {
        indexThisNum = List[indexThisNum].next;
    }
    //printf ("this number is %lf\n", List[indexThisNum].data);
    //printf ("indexThisNum after = %zu\n", indexThisNum);

    return List[indexThisNum].prev;
}

void     ListDeleteThisElem      (list_t * List,      info_t * listInfo, size_t num_of_count)
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
    //printf ("this number is %lf\n", List[indexThisNum].data);
    
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

elem_t   FindElemLogNum          (list_t * List,      info_t * listInfo, size_t num_of_count)
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

size_t   FindElemByValue         (list_t * List,      info_t * listInfo, elem_t num_of_count)
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

list_t * ListSort                (list_t ** List, info_t * listInfo)
{
    MY_ASSERT (*List == nullptr, "There is no access to list");
    list_t * newList = (list_t *) calloc (listInfo->capacity, sizeof(list_t));
    MY_ASSERT (newList == nullptr, "Unable to allocate memory");

    newList[0] = (*List)[0];

    size_t start = listInfo->head;
    for (int i = 1; i <= listInfo->size; i++)
    {
        newList[i] = (*List)[start];
        // printf ("newList[%d].data = %lf\n", i, newList[i].data);
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

void     Graphviz                (list_t ** List, info_t * listInfo)
{
    FILE * graph = fopen ("ListGraphviz.dot", "w");
    MY_ASSERT (graph == nullptr, "There is no access to file with graph");

    printf ("lol\n");

    *List = ListSort(List, listInfo);

    printf ("after listSort");

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

    //dumpline("node [fillcolor=\"#fee8c8\"]");
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

    cmdLine(times_push_html);

    createHTMLfile (&times_push_html);
    
}

void     validator               (list_t * List, info_t * listInfo)
{
    size_t current = listInfo->head;
    size_t nextMem = List[current].next;
    size_t prevMem = List[current].prev;

    for (int i = 0; i < listInfo->size; i++)
    {
        //printf ("in validator %d\n", i);
        if (List[List[nextMem].prev].data == List[current].data && List[List[prevMem].next].data == List[current].data)
        {
            // printf (LONG_LINE);
            // printf ("List[List[%zu].prev].data = %.2lf\n", nextMem, List[List[nextMem].prev].data);
            // printf ("List[List[%zu].next].data = %.2lf\n", prevMem, List[List[prevMem].next].data);
            // printf ("List[%zu].data = %.2lf\n", current, List[current].data);

            current = List[current].next;
            nextMem = List[current].next;
            prevMem = List[current].prev;
            // printf ("current = %zu\n", current);
            // printf ("nextMem = %zu\n", nextMem);
            // printf ("prevMem = %zu\n", prevMem);
            // printf (LONG_LINE);
            // printf ("\n");
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

void cmdLine (int num)
{
    char buf[100] = {};
    sprintf(buf, "dot -T png -o graph%d.png ListGraphviz.dot", num);\
    system(buf);
}

void createHTMLfile (int * num)
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