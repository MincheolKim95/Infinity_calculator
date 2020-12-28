#include "calculate.h"

//
pDigit divide(pDigit first, pDigit second)
{
    // 앞 수와 뒤 수를 모두 정수로 만들기 위해 올림 진행 eg) 0.004, 5 -> 4, 5000
    int maxCount = first->afterSize > second->afterSize ? first->afterSize : second->afterSize;
    round(first, maxCount);
    round(second, maxCount);

    if((second->beforeSize == 0) && (second->afterSize == 0))
    {
        printf("0으로는 나눌수가 없습니다.");
        exit(1);
    }
    else if((second->beforeSize = 1) && (second->afterSize == 0))
    {
        int tmp = popNum(&(second->before));
        if(tmp == 0)
        {
            printf("0으로는 나눌수가 없습니다.");
            exit(1);
        }
        else
        {
            pushNum(&(second->before), tmp);
        }
    }

    // 앞 수와 뒤 수의 차를 구하고 결과 값을 저장할 result 를 선언해주고, 계산 값을 저장할 sum 을 선언해준다.
    int tmp = first->beforeSize - second->beforeSize;
    pDigit result = initializeDigit();
    pDigit sum = initializeDigit();

    // 만약 차가 0보다 크거나 같으면 beforeSize를 tmp + 1로 셋팅해주고 그기 아니면 절대값 - 1로 해준다. : 결과 값은 X.XXX.. 이런식으로 나오기때문에
    if(tmp >= 0)
    {
        result->beforeSize = tmp + 1;
    }
    else
    {
        result->afterSize = (abs(tmp) - 1);
    }

    // 두 번째 자리의 숫자를 올려준다.
    round(second, tmp);

    int count = 0;
    int posCount = 0;
    int max = 500;
    int try = 0;
    while(1)
//    for(int i = 0; i < 10; i++)
    {
        // 만약 first 와 sum이 같다면 종료한다.
        if(isSame(first, sum) || try == max)
        {
            break;
        }
        // first 와 sum 이 다르다면 계산을 진행한다.
        else
        {
            int i = 0;
            for(;i < 9; i++)
            {
                // 만약 자리수 내림을 한 second와 sum을 더한 값이 first보다 작다면 sum에 더해준다.
                if(isBig(plus(copyDigit(second), copyDigit(sum)), first))
                {
                    sum = plus(copyDigit(second), copyDigit(sum));
//                    printf("\n\n");
//                    returnValue(copyDigit(sum));
//                    printf("\n\n");
                }
                // 만약 그게 아니라면 자리 수 내림을 위해 break를 시켜주지만 이때 두 숫자가 같은거라면 sum을 더한 다음에 break를 시켜준다.
                else
                {
                    if(isSame(first, plus(copyDigit(second), copyDigit(sum))))
                    {
                        sum = plus(copyDigit(second), copyDigit(sum));
                    }
                    break;
                }
            }
            // 만약 내린 자리수가 음수라면
            if(tmp < 0)
            {
                // 자리수 계산을 위해 count를 더해주고
                count++;
                // 숫자를 넣어준 후에
                pushNum(&(result->after), i);
                // 결과 값의 afterSize를 더해준다.
                result->afterSize++;
            }
            // 만약 0보다 크거나 같다면 역시 자리수 계산을 위해 posCount를 더해주고 숫자를 넣어준다.
            else
            {
                posCount++;
                pushNum(&(result->before), i);
            }
            // 자리수를 1 빼주고 second 역시 한자리 수 내려준다.
            tmp--;
            round(second, -1);
            try++;
        }
    }

    // 위에서 계산한 count와 posCount를 통해 자리수 계산을 진행한다.
    pNum tmpNum = NULL;
    for(int i = 0; i < result->afterSize; i++)
    {
        if(i < count)
        {
            pushNum(&tmpNum, popNum(&(result->after)));
        }
        else
        {
            pushNum(&tmpNum, 0);
        }
    }

    for(int i = 0; i < result->afterSize; i++)
    {
        pushNum(&(result->after), popNum(&tmpNum));
    }

    if(result->beforeSize == 0)
    {
        pushNum(&(result->before), 0);
        result->beforeSize = 1;
    }

    for(int i = 0; i < (result->beforeSize - posCount); i++)
    {
        pushNum(&(result->before), 0);
    }

    return result;
}