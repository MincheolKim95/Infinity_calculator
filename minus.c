#include "calculate.h"

pDigit minus(pDigit first, pDigit second)
{
    bool flag = false;
    bool positive = true;
    pDigit result = initializeDigit();

    if(isBig(first, second))
    {
        pDigit tmp = first;
        first = second;
        second = tmp;
        positive = false;
    }

	int beforeSize = first->beforeSize > second->beforeSize ? first->beforeSize : second->beforeSize;
	int afterSize  = first->afterSize > second->afterSize ? first->afterSize : second->afterSize;
    // int beforeSize = max(first->beforeSize, second->beforeSize);
    // int afterSize = max(first->afterSize, second->afterSize);
    int carry=0;
    int a, b;
    pNum pfirst, psecond,presult;

    pfirst = first->after;
    psecond = second->after;
    presult = result->after;

    for (int i = 0; i < afterSize; i++) {
        if (afterSize - i > first->afterSize) {
            a = 0;
        }
        else {
            a = pfirst->num;
            pfirst = pfirst->next;
        }

        if (afterSize - i > second->afterSize) {
            b = 0;
        }
        else {
            b = psecond->num;
            psecond = psecond->next;
        }

        if (a - carry < b) {
            a = 10 + a - carry - b;
            carry = 1;
        }
        else {
            a = a - carry - b;
            carry = 0;
        }

        if (presult == NULL) {
            presult = malloc( sizeof( Num ) );
            presult->num = a;
            presult->next = NULL;
            result->after = presult;
        }
        else
        {
            presult->next = malloc( sizeof( Num ) );
            presult->next->num = a;
            presult->next->next = NULL;
            presult = presult->next;
        }
        result->afterSize++;
    }

    pfirst = first->before;
    psecond = second->before;
    presult = result->before;

    for (int i = 0; i < beforeSize; i++)
    {
        if (i >= first->beforeSize) {
            a = 0;
        }
        else {
            a = pfirst->num;
            pfirst = pfirst->next;
        }

        if (i >= second->beforeSize) {
            b = 0;
        }
        else {
            b = psecond->num;
            psecond = psecond->next;
        }

        if (a - carry < b) {
            a = 10 + a - carry - b;
            carry = 1;
        }
        else {
            a = a - carry - b;
            carry = 0;
        }

        if (presult == NULL)
        {
            presult = malloc( sizeof( Num ) );
            presult->num = a;
            presult->next = NULL;
            result->before = presult;
        }
        else {
            presult->next = malloc( sizeof( Num ) );
            presult->next->num = a;
            presult->next->next = NULL;
            presult = presult->next;
        }
        result->beforeSize++;
    }

    int end = result->afterSize;
    for (int i = 0; i < end; i++)
    {
        if (result->after->num == 0)
        {
            pNum temp = result->after->next;
            free( result->after );
            result->after = temp;
            result->afterSize--;
        }
        else
        {
            break;
        }
    }


//    end = result->beforeSize;
//    pNum lastNonZero = NULL;
//    pNum cur = result->before;
//    for (int i = 0; i < end; i++)
//    {
//        if (cur != NULL && cur->num != 0)
//        {
//            lastNonZero = cur;
//        }
//        cur = cur->next;
//    }
//
//    while (lastNonZero != NULL) {
//        pNum temp = lastNonZero->next;
//        lastNonZero->next = NULL;
//        if (lastNonZero->num == 0) {
//            free(lastNonZero);
//            result->beforeSize--;
//        }
//        lastNonZero = temp;
//    }

    pNum tmp = NULL;
    bool tmpFlag = false;
    int tmpBeforeSize = result->beforeSize;
    for(int i = 0; i < result->beforeSize; i++)
    {
        pushNum(&tmp, popNum(&(result->before)));
    }
    for(int i = 0; i < result->beforeSize; i++)
    {
        int tmpNum = popNum(&tmp);
        if(i == (result->beforeSize - 1))
        {
           pushNum(&(result->before), tmpNum);
        }
        else
        {
            if(tmpNum != 0)
            {
                pushNum(&(result->before), tmpNum);
                tmpFlag = true;
            }
            else
            {
                if(tmpFlag)
                {
                    pushNum(&(result->before), tmpNum);
                }
                else
                {
                    tmpBeforeSize--;
                }
            }
        }
    }
    result->beforeSize = tmpBeforeSize;
    result->positive = true;
    return result;
}
