#include "listPtr.h"

int main (void) //добавить везде const
{
    listPtr_t list = {};
    struct listElement * listCreat = ListConstruct(&list);

    listDump (&list);
    PushFront (&list, 20);
    listDump (&list);

    struct listElement * after = PushBack (&list, 65);
    listDump (&list);

    PushBack (&list, 73);
    listDump (&list);

    ListInsertBefore (&list, after, 45);
    listDump (&list);

    PushFront (&list, 101);
    listDump (&list);

    listGraphviz(&list);

    struct listElement * noValid = validator (&list);
    // if (noValid == nullptr)
    // {
    //     printf ("999\n");
    // }
    // else 
    // {
    //     printf ("noValid->data = %.2lf\n", noValid->data);
    // }

    // struct listElement * finder = listFindElemByValue(&list, 65);
    // printf ("finder->data = %.2lf\n", finder->data);

    // struct listElement * byNum = listFindElemByNumber(&list, 2);
    // printf ("byNum->data = %.2lf\n", byNum->data);


    // listDeleteElem(&list, after);
    // listDump (&list);

    // ListInsertAfter (&list, after, 31);
    // listDump (&list);

    // listDestructor (&list);
    // listDump (&list);

    //printf ("list = %p\n", &list);
}

struct listElement * ListConstruct (listPtr_t * list)
{
    list->ptrToList = (struct listElement *) calloc (1, sizeof (listElement));

    MY_ASSERT(list->ptrToList == nullptr, "Unable to allocate memory for the list");

    // printf ("\nIn CONSTRUCT before operations\n");
    // printf ("list->ptrToList[0].prev = %p\n", list->ptrToList[0].prev);
    // printf ("list->ptrToList[0].next = %p\n", list->ptrToList[0].next);

    list->ptrToList[0].data = poison;
    list->ptrToList[0].prev = list->ptrToList[0].next = &(list->ptrToList[0]); //в нулевом эл-те prev == head
                                                                            //                next == tail                                                                     
    // printf ("\nIn CONSTRUCT after operations\n");
    // printf ("list->ptrToList[0].prev = %p\n", list->ptrToList[0].prev);
    // printf ("list->ptrToList[0].next = %p\n", list->ptrToList[0].next);
    // printf ("data in tail and head: %.2lf and %.2lf\n\n", list->ptrToList[0].prev->data, list->ptrToList[0].next->data);

    return &(list->ptrToList[0]);
}

void listDump (const listPtr_t * list)
{
    MY_ASSERT (list == nullptr, "There is no access to list file");
    FILE * log = fopen ("log.txt", "a");
    MY_ASSERT(log == nullptr, "There is no access to log file");

    fprintf (log, LONG_LINE);

    fprintf (log, "list.size = %zu\n", list->size);
    
    fprintf (log, "data in fucken element is %.2lf\n\n", list->ptrToList[0].data);

    fprintf (log, "data: ");

    struct listElement * currentElem = (list->ptrToList)[0].prev;
    for (int i = 0; i < list->size; i++)
    {
        fprintf (log, "%.2lf ", currentElem->data);
        currentElem = ptrListElemNext(currentElem);
    }

    fprintf (log,"\nnext: ");

    currentElem = list->ptrToList[0].prev;
    for (int i = 0; i < list->size; i++)
    {
        fprintf (log, "%.2lf ", currentElem->next->data);
        currentElem = ptrListElemNext(currentElem);
    }

    fprintf (log, "\nprev: ");

    currentElem = list->ptrToList[0].prev;
    for (int i = 0; i < list->size; i++)
    {
        fprintf (log, "%.2lf ", currentElem->prev->data);
        currentElem = ptrListElemNext(currentElem);
    } 

    fprintf (log, "\n");
    fprintf (log, LONG_LINE);
    fprintf (log, "\n\n");


    fflush (log);
    fclose (log);
}

struct listElement * ptrListElemNext (struct listElement * currentElem)
{
    MY_ASSERT (currentElem == nullptr, "There is no access to current element");
    
    return currentElem->next; 
}

struct listElement * ptrListElemPrev (struct listElement * currentElem)
{
    MY_ASSERT (currentElem == nullptr, "There is no access to current element");

    return currentElem->prev;
}

struct listElement * PushBack (listPtr_t * list, elem_t newElemValue)
{
    MY_ASSERT (list == nullptr, "There is no access to list");
    struct listElement * newElem = (struct listElement *) calloc (1, sizeof(struct listElement));
    MY_ASSERT (newElem == nullptr, "Unable to allocate new memory");

    if (list->size == 0)
    {
        return PushFirstElem (list, newElemValue);
    }

    newElem->data = newElemValue;
    newElem->next = list->ptrToList[0].prev;
    newElem->prev = list->ptrToList[0].next;

    list->ptrToList[0].next->next = newElem;
    list->ptrToList[0].prev->prev = newElem;

    list->ptrToList[0].next = newElem;    

    list->size++;

    return newElem;
}

struct listElement * PushFront (listPtr_t * list, elem_t newElemValue)
{
    MY_ASSERT (list == nullptr, "There is no access to list");
    struct listElement * newElem = (struct listElement *) calloc (1, sizeof(struct listElement));
    MY_ASSERT (newElem == nullptr, "Unable to allocate new memory");

    if (list->size == 0)
    {
        return PushFirstElem (list, newElemValue);
    }

    newElem->data = newElemValue;
    
    newElem->next = list->ptrToList[0].prev;
    newElem->prev = list->ptrToList[0].next;

    list->ptrToList[0].prev->prev = newElem;
    list->ptrToList[0].next->next = newElem;

    list->ptrToList[0].prev = newElem;

    list->size++;

    return newElem;
}

struct listElement * PushFirstElem (listPtr_t * list, elem_t newElemValue)
{
    struct listElement * newElem = (struct listElement *) calloc (1, sizeof(struct listElement));
    MY_ASSERT (newElem == nullptr, "There is no access to list");
    list->ptrToList[0].next = newElem;
    list->ptrToList[0].prev = newElem;

    printf ("PushFirstElem works now\n");

    newElem->data = newElemValue;
    newElem->next = list->ptrToList[0].prev;
    newElem->prev = list->ptrToList[0].next;  

    list->size++;

    return newElem;
}

struct listElement * ListInsertBefore (listPtr_t * list, struct listElement * currentElem, elem_t newElemValue)
{
    MY_ASSERT (list == nullptr, "Unable to allocate new memory");
    struct listElement * newElem = (struct listElement *) calloc (1, sizeof(struct listElement));
    MY_ASSERT (newElem == nullptr, "Unable to allocate new memory");

    newElem->data = newElemValue;
    newElem->next = currentElem;
    newElem->prev = currentElem->prev;

    currentElem->prev->next = newElem;
    currentElem->prev = newElem;

    list->size++;

    return newElem;
}

struct listElement * ListInsertAfter (listPtr_t * list, struct listElement * currentElem, elem_t newElemValue)
{
    MY_ASSERT (list == nullptr, "Unable to allocate new memory");
    struct listElement * newElem = (struct listElement *) calloc (1, sizeof(struct listElement));
    MY_ASSERT (newElem == nullptr, "Unable to allocate new memory");

    newElem->data = newElemValue;
    newElem->next = currentElem->next;
    newElem->prev = currentElem;

    currentElem->next->prev = newElem;
    currentElem->next = newElem;

    list->size++;

    return newElem;
}

void listDestructor (listPtr_t * list)
{
    // printf ("list->ptrToList[0].prev = %p and the data of it is \"%.2lf\"\n", list->ptrToList[0].prev, list->ptrToList[0].prev->data);
    MY_ASSERT(list == nullptr, "There is no access to list");
    // printf ("in Destructor\n");
    struct listElement * current = list->ptrToList[0].prev;
    // printf ("\nhead = %p\n\n", &(list->ptrToList[0]));
    // printf ("Before for\n");
    for (int i = 0; i < list->size - 1; i++)
    {
        // printf ("begin in for: %d\n", i);
        // printf ("current->data before = %.2lf\n", current->data);
        current->data = poison;
        // printf ("current->data after = %.2lf\n", current->data);
        // printf ("1 operation\n");
        current->prev = nullptr;
        // printf ("2 operation\n");
        // printf ("current before = %p\n", current);
        //(*tmp)->next = nullptr; 
        current = current->next;
        current->prev->next = nullptr;
        // printf ("current after = %p\n", current);
        // printf ("current.data = %.2lf\n", current->data);
        // printf ("3 operation\n");
        // printf ("4 operation\n");
    }

    current->data = poison;
    current->prev = nullptr;
    current->next = nullptr;
             
    list->ptrToList[0].prev = nullptr;
    list->ptrToList[0].next = nullptr;
    list->size = 0;
}

void listDeleteElem (listPtr_t * list, struct listElement * currentElem)
{
    MY_ASSERT (list == nullptr, "There is no access to list");
    MY_ASSERT (currentElem == nullptr, "Unable to find this element");

    currentElem->prev->next = currentElem->next;
    currentElem->next->prev = currentElem->prev;

    currentElem->data = poison;
    currentElem->next = nullptr;
    currentElem->prev = nullptr;

    list->size--;
}

struct listElement * AccessFirstElem (listPtr_t * list)
{
    MY_ASSERT (list == nullptr, "There is no access to list");

    return list->ptrToList[0].prev;
}

struct listElement * AccessLastElem (listPtr_t * list)
{
    MY_ASSERT (list == nullptr, "There is no access to list");

    return list->ptrToList[0].next;
}

size_t listSize (listPtr_t * list)
{
    MY_ASSERT (list == nullptr, "There is no access to list");

    return list->size;
}

struct listElement * listFindElemByValue (listPtr_t * list, elem_t value)
{
    MY_ASSERT (list == nullptr, "There is no access to list");
    
    struct listElement * current = list->ptrToList[0].prev;

    for (int i = 0; i < list->size && current->data != value; i++)
    {
        current = current->next;
    }
    printf ("current->data = %.2lf\n", current->data);
    return current;
}

struct listElement * listFindElemByNumber (listPtr_t * list, elem_t number)
{
    MY_ASSERT (list == nullptr, "There is no access to list");

    struct listElement * current = list->ptrToList[0].prev;

    for (int i = 0; i < number && i < list->size; i++)
    {
        current = current->next;
    }
    
    return current;
}

struct listElement * validator (const listPtr_t * list)
{
    MY_ASSERT (list == nullptr, "There is no access to list");

    FILE * log = fopen ("log.txt", "a");
    MY_ASSERT (log == nullptr, "There is no access to list");

    struct listElement * current = list->ptrToList[0].prev;

    for (int i = 0; i < list->size; i++)
    {
        if (current->next->prev->data == current->data && current->prev->next->data == current->data)
        {
            // printf ("in if in valid\n");
            current = current->next;
            // printf ("after\n");
        }
        else 
        {
            // printf ("lol\n");
            fprintf (log, "The error occurred around an element with a value of \"%.2lf\"\n", current->data);
            // printf ("current->next->prev->data = %.2lf, current->prev->next->data = %.2lf, current->data = %.2lf\n", current->next->prev->data, current->prev->next->data, current->data);
            printf ("A small error has occurred\n");
            listDump (list);
            return current;
        }
    }

    fflush (log);
    fclose (log);
    return nullptr;
}

void listGraphviz (const listPtr_t * list)
{
    FILE * ptrGraph = fopen ("GraphForPtrList.dot", "w");
    MY_ASSERT (ptrGraph == nullptr, "There is no access to dump file for graphviz");

    dumpline("digraph G {\n");
    dumpline("  graph [dpi = 100];\n\n");
    dumpline("  ranksep = 1.5;\n\n");
    dumpline("  splines = ortho;\n\n");
    
    dumpline("  {rank = min;\n");
    dumpline("          above_node[shape = rectangle, style = \"rounded, invis\", fixedsize = true, height = 1, width = 2, fontsize = 30, label = \"Hi!\", width = 3];");
    dumpline("  }\n");
    dumpline("  {rank = same;\n");
    
    dumpline("  node0[shape = record, style = \"rounded, filled\", color = \"#8c6bb1\", label=\"size = %zu|<h0>head = %.2lf|<t0>tail = %.2lf\"];\n",, list->size, list->ptrToList[0].prev->data, list->ptrToList[0].next->data);

    dumpline("  edge[minlen = 3, penwidth = 3];\n");
    dumpline("  node[shape = rectangle, style = \"rounded, filled\",\n");
    dumpline("              fixedsize = true, height = 1, width = 2,\n");
    dumpline("              penwidth = 4, color =\"#fa9fb5\", fontsize = 30];\n\n");

    struct listElement * current = list->ptrToList[0].prev;
    for (int i = 1; i < list->size + 1; i++)
    {
        dumpline("  node%d[fillcolor = \"#ffffcc\", label = \"%.2lf\"];\n",, i, current->data);
        current = current->next;
    }
    dumpline("  }\n");
    dumpline("  {rank = max;\n");
    dumpline("          below_node[style = invis, label = \"Bye!\", width = 3];\n");
    dumpline("  }\n\n");

    dumpline("  above_node -> node0 [style = invis];\n");
    dumpline("  below_node -> node0 [style = invis];\n");
    dumpline("  node0:h0 -> node1 [color = blue];\n");
    dumpline("  node0:t0 -> node%zu [color = blue];\n",, list->size);

    for (int i = 1; i < list->size; i++)
    {
        dumpline("  node%d -> node%d [color = black];\n",, i, i+1);
        dumpline("  node%d -> node%d [color = black];\n",, i+1, i);
    }
    dumpline("  node1 -> node%zu [color = black];\n",, list->size);
    dumpline("  node%zu -> node1 [color = black];\n",, list->size);

    dumpline("}\n\n");

    fflush (ptrGraph);
    fclose (ptrGraph);

    cmdLine (times_push_html);

    createHTMLfile (&times_push_html);
}

void cmdLine (int num)
{
    char buf[100] = {};
    sprintf(buf, "dot -T png -o graph%d.png GraphForPtrList.dot", num);\
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