#include "list.h"

#define CLEAR_LOGFILE

int main (void)
{
    info_t info = {};
    list_t * List = listConstruct (&info, 3);
    listPushFront (&List, &info, 31);

    listPushBack (&List, &info, 87);

    listPushFront (&List, &info, 65);

    listInsertBeforeThisNum (&List, &info, 50, 2);
    listDump (&info, &List);

    listInsertAfterThisNum (&List, &info, 71, 4);
    listDump (&info, &List);

    listPushFront (&List, &info, 23);
    listDump (&info, &List);

}
