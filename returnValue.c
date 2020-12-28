#include "calculator.h"

void returnValue(pDigit digit)
{
    // Stack 구조에 역순으로 저장된 숫자를 다시 순서대로 표현하기 위해 새로운 Stack을 만든다.
    pNum resultBefore = NULL;
    pNum resultAfter = NULL;

    // 숫자가 음수면 앞에 -를 출력해준다.
    if(!(digit->positive))
    {
        printf("-");
    }

    // Digit에 담긴 숫자를 새로운 Stack에 넣어준다.
    for(int i = 0; i < digit->beforeSize; i++)
    {
        pushNum(&resultBefore, popNum(&(digit->before)));
    }
    for(int i = 0; i< digit->afterSize; i++)
    {
        pushNum(&resultAfter, popNum(&(digit->after)));
    }

    // 만약 맨 앞에 숫자가 0일 수도 있기 때문에 그걸 확인해주는 코드이다.
    bool flag = false;
    for(int i = 0; i < digit->beforeSize; i++)
    {
        int tmp = popNum(&resultBefore);
        if(tmp != 0)
        {
            printf("%d", tmp);
            flag = true;
        }
        else
        {
            if(flag)
            {
                printf("%d", tmp);
            }
        }
    }

    // 만약 flag 가 false 라면 => 앞에 숫자가 모두 0이라는 의미이므로 0.XXX로 표현하기 위해 0을 출력해준다.
    if(!flag)
    {
        printf("0");
    }

    // 만약 뒤에 자리가 0이면 소수 표현을 위해 .0을 출력해준다.
    if(digit->afterSize == 0)
    {
        printf(".0");
    }
    else
    {
        printf(".");
        // 뒤에 자리수를 정상적으로 출력해준다.
        for(int j = 0; j < digit->afterSize; j++)
        {
            printf("%d", popNum(&resultAfter));
        }
    }

//    printf(" before :%d, after : %d", digit->beforeSize, digit->afterSize);
}