#include "listPtr.h"

int main (void)
{
    listPtr_t list = {};

    FILE * logForList = listSystemOfLogs ("log.txt");
    FILE * HTMLgraphDump = listCreateHTMLfileForGraphviz ("HTMLBuf.html");

    listConstruct(&list);

    listDump (&list, logForList);

    listPushFront (&list, 20);

    listDump (&list, logForList);

    struct listElement_t * after = listPushBack (&list, 65);

    listDump (&list, logForList);

    listPushBack (&list, 73);

    listDump (&list, logForList);

    listInsertBefore (&list, after, 45);
    listDump (&list, logForList);

    listPushFront (&list, 101);
    listDump (&list, logForList);

    listGraphviz(&list, "GraphForPtrList.dot", HTMLgraphDump);

    struct listElement_t * noValid = listValidator (&list, logForList);

    // listDeleteElem(&list, after);
    // listDump (&list, logForList);

    // listInsertAfter (&list, after, 31);
    // listDump (&list, logForList);

    // listDestructor (&list);
    // listDump (&list, logForList);
}
