#include "calculate.h"

pDigit multiply(pDigit first, pDigit second)
{
    // 만약 첫 수의 소수점 전, 후 자리가 두 번째 수의 소수점 전, 후의 자리수 보다 짧다면 바꿔 준다.
    if((first->beforeSize + first->afterSize) < (second->beforeSize + second->afterSize))
    {
        pDigit tmp = first;
        first = second;
        second = tmp;
    }

    // 저장할 변수를 선언해준다.
    pDigit sum = initializeDigit();
    // 더 짧은 수 기준으로 뒤에자리 숫자 부터 자리 수 이동을 해준 후에 곱하여 모든 수를 더해준다.
    for(int i = 0; i < second->afterSize; i++)
    {
        sum = plus(mul(copyDigit(first), popNum(&(second->after)), -(second->afterSize - i)), copyDigit(sum));
//        returnValue(copyDigit(sum));
//        printf("\n");
    }
    for(int i = 0; i < second->beforeSize; i++)
    {
//        printf("dd : %d\n", i);
        sum = plus(mul(copyDigit(first), popNum(&(second->before)), i), copyDigit(sum));
//        returnValue(copyDigit(sum));
//        printf("\n");
//        printf("\n");
//        sum = plus(mul(copyDigit(first), popNum(&(second->before)), i), sum);
    }
    return sum;
}

pDigit mul(pDigit digit, int x, int under)
{
    // 결과 값을 받을 back, front, result를 초기화 해준다.
    pNum back = NULL;
    pNum front = NULL;
    pDigit result = initializeDigit();
    int new;

    // 자리수를 올림해 주어야 할때는
    if(under > 0)
    {
        pNum tmpNum = NULL;
        // 만약 소수점 뒤가 있다면 pop을 하여 앞자리에 push를 해준다.
        if(digit->afterSize != 0)
        {
            for(int i = 0; i < digit->afterSize; i++)
            {
                pushNum(&tmpNum, popNum(&(digit->after)));
            }

            for(int i = 0; i < digit->afterSize; i++)
            {
                if(i < under)
                {
                    pushNum(&(digit->before), popNum(&tmpNum));
                }
                else
                {
                    pushNum(&(digit->after), popNum(&tmpNum));
                }
            }
        }
            // 소수점 뒤가 없다면 0을 푸쉬해준다.
        else
        {
            for(int i = 0; i < under; i++)
            {
                pushNum(&(digit->before), 0);
            }
        }
        digit->beforeSize += under;
        digit->afterSize -= under;
    }

        // 만약 자리수를 내림해 주어야 할때는
    else if (under < 0)
    {
        pNum tmpNum = NULL;
        // 뒤 자리수가 존재한다면
        if(digit->afterSize != 0)
        {
            // 뒤에 자리수를 전부 꺼낸 다음
            for(int i = 0; i < digit->afterSize; i++)
            {
                pushNum(&tmpNum, popNum(&(digit->after)));
            }
            // 앞에 자리수에서 pop을 하여 임시 저장소에 push를 한다.
            for(int i = 0; i < abs(under); i++)
            {
                if(digit->beforeSize > i)
                {
                    pushNum(&tmpNum, popNum(&(digit->before)));
                }
                else
                {
                    pushNum(&tmpNum, 0);
                }
            }
            // 그리고 방금 꺼낸 수를 다시 넣어 준다.
            for(int i = 0; i < (digit->afterSize + abs(under)); i++)
            {
                pushNum(&(digit->after), popNum(&tmpNum));
            }
        }
            // 뒤 자리수가 존재 하지 않다면
        else
        {
            // 앞 수를 꺼내서 뒤 자리에 보내준다.
            for(int i = 0; i < abs(under); i++)
            {
                int tmp = popNum(&(digit->before));
                pushNum(&tmpNum, tmp);
            }
            for(int i = 0; i < abs(under); i++)
            {
                pushNum(&(digit->after), popNum(&tmpNum));
            }
        }
        digit->beforeSize -= abs(under);
        digit->afterSize += abs(under);
    }

    // 만약 각 자리수가 음수면 0으로 변경해준다.
    if(digit->beforeSize < 0)
    {
        digit->beforeSize = 0;
    }
    if(digit->afterSize < 0)
    {
        digit->afterSize = 0;
    }

//    printf("\n\nbefore : %d, after : %d\n", digit->beforeSize, digit->afterSize);
//    printf("\n\ndigit: ");
//    returnValue(digit);
//    printf("\n\n");

    int afterSize = digit->afterSize;
    int carry = 0;
    bool flag = false;
    if(x != 0)
    {
        // x 가 0이 아니라면 뒤 자리 수부터 곱해준다. 이때 10이 넘어가면 carry에 보내준다.
        for(int i= 0; i < digit->afterSize; i++)
        {
            // 캐리는 그 전에 있는 수를 더할 때 사용된다.
            new = (popNum(&(digit->after)) * x) + carry;
            carry = new / 10;
            new = new - (carry * 10);
            // 만약 입력한 값이 0만 나온다면 자리 수 제거를 위해 다음과 같이 처리한다.
            if(new != 0)
            {
                flag = true;
                pushNum(&back, new);
            }
            else
            {
                if(flag)
                {
                    pushNum(&back, new);
                }
                else
                {
                    afterSize--;
                }
            }
        }
        // 뒤 수 처리가 끝나면 앞수를 처리한다. 이때 원리는 위와 동일하다.
        for(int i = 0; i < digit->beforeSize; i++)
        {
            new = (popNum(&(digit->before)) * x) + carry;
            carry = new / 10;
            new = new - (carry * 10);

            pushNum(&front, new);
        }

        if(carry != 0)
        {
            pushNum(&front, carry);
            digit->beforeSize++;
        }

        pNum resultBefore = NULL;
        pNum resultAfter = NULL;

        // 그리고 결과 값을 Stack 구조로 정렬한다.
        for(int i = 0; i < digit->beforeSize; i++)
        {
            pushNum(&resultBefore, popNum(&front));
        }
        for(int i = 0; i < afterSize; i++)
        {
            int tmp = popNum(&back);
            pushNum(&resultAfter, tmp);
        }

        // 마지막으로 결과 값을 정리한다.
        result->beforeSize = digit->beforeSize;
        result->afterSize = afterSize;
        result->before = resultBefore;
        result->after = resultAfter;
    }

//    printf("result : ");
//    returnValue(result);
//    printf("\n");

    if(result->beforeSize == 0)
    {
        pushNum(&(result->before), 0);
        result->beforeSize = 1;
    }


    return result;
}
