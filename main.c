#include <stdio.h>
#include "calculator.h"

int main(int argc, char* argv[])
{
    List list;
    initList(&list);
    FILE* ifp = fopen(argv[1], "r");
    char* str = openFile(ifp);
    makePostfix(str, &list, NULL);
    free(str);
    pDigit digit = makeExpression(&list);
    returnValue(digit);
}
