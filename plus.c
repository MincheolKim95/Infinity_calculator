#include "calculate.h"

pNum beforeCal(pNum max, pNum min, int max_before, int min_before, bool carry, bool* result)
{
    pNum before = NULL;
    while (max_before != 0)
    {
        int tmpbefore;
        if(min_before > 0)
        {
            tmpbefore = popNum(&max) + popNum(&min);
        }
        else
        {
            tmpbefore = popNum(&max);
        }
        if (carry)
        {
            tmpbefore++;
            carry = false;
        }
        if ((tmpbefore / 10) >= 1)
        {
            carry = true;
            tmpbefore = tmpbefore - 10;
        }
        pushNum(&before, tmpbefore);
        max_before--;
        min_before--;
    }

    if(max_before == 0)
    {
        if (carry)
        {
            pushNum(&before, 1);
        }
    }
    *result = carry;
    return before;
}

pNum afterCal(pNum max, pNum min, int max_after, int min_after, bool* result)
{
    pNum after = NULL;
    bool carry = false;
    //�����ؾ� �� ��� ���ڰ� ������ �� ���� �״�� ���ÿ� �״´�.
    while (max_after != min_after)
    {
        int new = popNum(&max);
        pushNum(&after, new);
        max_after--;
    }
    //�� ���� ��� �����ؾ� �� ���� ���� ������ ���ϱ� ����
    while (max_after != 0)
    {
        int tmpafter = popNum(&max) + popNum(&min);
        if (carry)
        {
            tmpafter++;
            carry = false;
        }
        if ((tmpafter / 10) >= 1)
        {
            carry = true;
            tmpafter = tmpafter - 10;
        }
        pushNum(&after, tmpafter);

        max_after--;
        min_after--;
    }

    *result = carry;
    return after;
}

pDigit plus(pDigit first, pDigit second)
{
    pNum before = NULL;
    pNum after = NULL;
    int beforeSize = 0;
    int afterSize = 0;
    pDigit result = initializeDigit();

    result->positive = true;

//    printf("first: ");
//    returnValue(copyDigit(first));
//    printf(", second: ");
//    returnValue(copyDigit(second));
//    printf("\n");
//
//    printf("first b : %d, first a: %d", first->beforeSize, first->afterSize);
//    printf(" second b : %d, second a: %d\n", second->beforeSize, second->afterSize);

    if((first->beforeSize == 0) && (first->afterSize == 0))
    {
        return second;
    }

    if((second->beforeSize == 0) && (second->afterSize == 0))
    {
        return first;
    }

    int firstBeforesize = first->beforeSize;
    int secondBeforesize = second->beforeSize;
    int firstAftersize = first->afterSize;
    int secondAftersize = second->afterSize;

    bool carry = false;
    bool resultFlag = 0;

    int maxAftersize;
    int maxBeforesize;

    if (firstAftersize > secondAftersize)
    {
        maxAftersize = firstAftersize;
        after = afterCal(first->after, second->after, firstAftersize, secondAftersize, &carry);
        if(firstBeforesize > secondBeforesize)
        {
            maxBeforesize = firstBeforesize;
            before = beforeCal(first->before, second->before, firstBeforesize, secondBeforesize, carry, &resultFlag);
        }
        else
        {
            maxBeforesize = secondBeforesize;
            before = beforeCal(second->before, first->before, secondBeforesize, firstBeforesize, carry, &resultFlag);
        }
    }
    else
    {
        maxAftersize = secondAftersize;
        after = afterCal(second->after, first->after, secondAftersize, firstAftersize, &carry);
        if (firstBeforesize > secondBeforesize)
        {
            maxBeforesize = firstBeforesize;
            before = beforeCal(first->before, second->before, firstBeforesize, secondBeforesize, carry, &resultFlag);
        }
        else
        {
            maxBeforesize = secondBeforesize;
            before = beforeCal(second->before, first->before, secondBeforesize, firstBeforesize, carry, &resultFlag);
        }
    }

    //����� size ����
    afterSize = maxAftersize;
    if (resultFlag)
    {
        beforeSize = maxBeforesize + 1;
    }
    else
    {
        beforeSize = maxBeforesize;
    }

    pNum resultBefore = NULL;
    pNum resultAfter = NULL;

    for (int i = 0; i < afterSize; i++)
    {
        int new = popNum(&after);
        pushNum(&resultAfter, new);
    }
    for (int i = 0; i < beforeSize; i++)
    {
        int new = popNum(&before);
        pushNum(&resultBefore, new);
    }

    result->beforeSize = beforeSize;
    result->afterSize = afterSize;
    result->before = resultBefore;
    result->after = resultAfter;

    if(result->afterSize == 1)
    {
        int tmp = popNum(&(result->after));
        if(tmp == 0)
        {
            result->afterSize--;
        }
        else
        {
            pushNum(&(result->after), tmp);
        }
    }
    return result;
}