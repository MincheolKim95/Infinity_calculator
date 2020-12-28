#include "calculator.h"

void round(pDigit digit, int round)
{
    // 자리수를 올림해 주어야 할때는
    if(round > 0)
    {
        pNum tmpNum = NULL;
        if(digit->beforeSize == 1)
        {
            int tmp = popNum(&(digit->before));
            if(tmp == 0)
            {
                digit->beforeSize = 0;
            }
            else
            {
                pushNum(&(digit->before), tmp);
            }
        }

        // 만약 소수점 뒤가 있다면 pop을 하여 앞자리에 push를 해준다.
        if(digit->afterSize != 0)
        {
            if(round > digit->afterSize)
            {
                for(int i = 0; i < round; i++)
                {
                    if(i < digit->afterSize)
                    {
                        pushNum(&tmpNum, popNum(&(digit->after)));
                    }
                    else
                    {
                        pushNum(&tmpNum, 0);
                    }
                }
            }
            else
            {
                for(int i = 0; i < digit->afterSize; i++)
                {
                    pushNum(&tmpNum, popNum(&(digit->after)));
                }
            }

            bool flag = false;
            int count = 0;
            for(int i = 0; i < round; i++)
            {
                int tmp = popNum(&tmpNum);
                if(tmp != 0)
                {
                    count++;
                    flag = true;
                }
                else
                {
                    if(flag)
                    {
                        count++;
                    }
                }
                pushNum(&(digit->before), tmp);
            }

            if(digit->afterSize > round)
            {
                for(int i = 0; i < (digit->afterSize - round); i++)
                {
                    int tmp = popNum(&tmpNum);
                    pushNum(&(digit->after), tmp);
                }
            }

            digit->beforeSize += count;
        }
        // 소수점 뒤가 없다면 0을 푸쉬해준다.
        else
        {
            for(int i = 0; i < round; i++)
            {
                pushNum(&(digit->before), 0);
            }
            digit->beforeSize += round;
        }
        digit->afterSize -= round;
    }

        // 만약 자리수를 내림해 주어야 할때는
    else if (round < 0)
    {
        pNum tmpNum = NULL;
        // 뒤 자리수가 존재한다면
        if(digit->afterSize != 0)
        {

            // 뒤에 자리수를 전부 꺼낸 다음
            bool flag = false;
            int afterSize = digit->afterSize;

            for(int i = 0; i < digit->afterSize; i++)
            {
                int tmp = popNum(&(digit->after));
                if(tmp != 0)
                {
                    pushNum(&tmpNum, tmp);
                    flag = true;
                }
                else
                {
                    if(flag)
                    {
                        pushNum(&tmpNum, tmp);
                    }
                    else
                    {
                        afterSize--;
                    }
                }
            }
            // 앞에 자리수에서 pop을 하여 임시 저장소에 push를 한다.
            int absNum = abs(round);
            for(int i = 0; i < abs(round); i++)
            {
                if(digit->beforeSize > i)
                {
                    int tmp = popNum(&(digit->before));
                    if(tmp != 0)
                    {
                        pushNum(&tmpNum, tmp);
                        flag = true;
                    }
                    else
                    {
                        if(flag)
                        {
                            pushNum(&tmpNum, tmp);
                        }
                        else
                        {
                            absNum--;
                        }
                    }
                }
                else
                {
                    pushNum(&tmpNum, 0);
                }
            }
            // 그리고 방금 꺼낸 수를 다시 넣어 준다.
            for(int i = 0; i < afterSize + absNum; i++)
            {
                pushNum(&(digit->after), popNum(&tmpNum));
            }
            digit->afterSize = afterSize + absNum;
        }
            // 뒤 자리수가 존재 하지 않다면
        else
        {
            bool flag = false;
            int afterSize = abs(round);
            // 앞 수를 꺼내서 뒤 자리에 보내준다.
            for(int i = 0; i < abs(round); i++)
            {
                int tmp = popNum(&(digit->before));
                if(tmp != 0)
                {
                    pushNum(&tmpNum, tmp);
                    flag = true;
                }
                else
                {
                    if(flag)
                    {
                        pushNum(&tmpNum, tmp);
                    }
                    else
                    {
                        afterSize--;
                    }
                }
            }
            for(int i = 0; i < afterSize; i++)
            {
                pushNum(&(digit->after), popNum(&tmpNum));
            }
            digit->afterSize = afterSize;
        }
        digit->beforeSize -= abs(round);
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

    if(digit->beforeSize == 0)
    {
        pushNum(&(digit->before), 0);
        digit->beforeSize = 1;
    }
}