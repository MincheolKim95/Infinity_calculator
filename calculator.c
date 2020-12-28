#include "calculate.h"
#include <stdlib.h>
#include <stdio.h>

pNum getNum(int num)
{
    // Num 스택을 초기화 해줌
    Num nullNode = { 0, NULL };
    pNum tmp = malloc(sizeof(Num));
    if(tmp)
    {
        *tmp = nullNode;
        tmp->num = num;
        return tmp;
    }
    return NULL;
}

int pushNum(pNum* top, int num)
{
    pNum new = getNum(num);
    if(new)
    {
        new->next = *top;
        *top = new;
        return 1;
    }
    return 0;
}

int emptyNum(pNum top)
{
    return (top == NULL);
}

int popNum(pNum* top)
{
    if (!emptyNum(*top))
    {
        pNum tmp = *top;
        int result = tmp->num;
        *top = tmp->next;
        free(tmp);
        return result;
    }
    return -1;
}


pOperand getOperand(pDigit digit)
{
    Operand nullNode = { NULL, NULL };
    pOperand tmp = malloc(sizeof(Operand));
    if(tmp)
    {
        *tmp = nullNode;
        tmp->digit = digit;
        return tmp;
    }
    return NULL;
}

int pushOperand(pOperand* top, pDigit digit)
{
    pOperand new = getOperand(digit);
    if(new)
    {
        new->next = *top;
        *top = new;
        return 1;
    }
    return 0;
}

int emptyOperand(pOperand top)
{
    return (top == NULL);
}

pDigit popOperand(pOperand* top)
{
    if (!emptyOperand(*top))
    {
        pOperand tmp = *top;
        pDigit result = tmp->digit;
        *top = tmp->next;
        free(tmp);
        return result;
    }
    return NULL;
}

pOperator getOperator(char op)
{
    Operator nullNode = { 0, NULL };
    pOperator tmp = malloc(sizeof(Operator));
    if(tmp)
    {
        *tmp = nullNode;
        tmp->operator = op;
        return tmp;
    }
    return NULL;
}

pExpression getExpression(int num)
{
    Expression nullNode = { 0, NULL, NULL };
    pExpression tmp = malloc(sizeof(Expression));
    if(tmp)
    {
        *tmp = nullNode;
        tmp->num = num;
        return tmp;
    }
    return NULL;
}

int pushOperator(pOperator* top, char op)
{
    pOperator new = getOperator(op);
    if(new)
    {
        new->next = *top;
        *top = new;
        return 1;
    }
    return 0;
}

int emptyOperator(pOperator top)
{
    return (top == NULL);
}

char popOperator(pOperator* top)
{
    if (!emptyOperator(*top))
    {
        pOperator tmp = *top;
        char result = tmp->operator;
        *top = tmp->next;
        free(tmp);
        return result;
    }
    return -1;
}

void initList(pList list)
{
    list->head = getExpression(0);
    list->tail = getExpression(0);
    list->head->next = list->tail;
    list->tail->prev = list->head;
}

void addExpression(pList list, int num)
{
    pExpression new = getExpression(num);
    new->prev = list->tail->prev;
    new->next = list->tail;
    list->tail->prev->next = new;
    list->tail->prev = new;
}

void viewAll(pList list)
{
    pExpression seek = list->head->next;

    while(seek != list->tail)
    {
        printf("%d ", seek->num);
        seek = seek->next;
    }
}

pDigit initializeDigit()
{
    pDigit tmp = malloc(sizeof(Digit));
    tmp->before = NULL;
    tmp->beforeSize = 0;
    tmp->after = NULL;
    tmp->afterSize = 0;
    tmp->positive = true;
    return tmp;
}



pDigit makeExpression(pList list)
{
    // Double Linked List 로 변환된 입력값을 후위연산식으로 변경하는 함수
    pExpression seek = list->head->next;
    pOperand operand = NULL;
    // digit을 우선 0으로 초기화 해줌.
    pDigit digit = initializeDigit();
    pNum num = NULL;
    bool digitFlag = true;

    // 소수점 기준 앞자리와 뒤자리를 구분하기 위한 타입
    int lengthFlag = 0;
    // 소수점 기준으로 앞자리와 뒤자리의 길이를 저장하는 배열
    int length[] = { 0, 0 };
    while(seek != list->tail)
    {
        // tmp 에는 문자 하나가 들어온다.
        int tmp = seek->num;
        // 만약 tmp 가 0과 9 사이에 있다면-> 숫자라면 Num에 저장하고, Flag에 맞는 자리수 배열을 하나 늘린다.
        if(0 <= tmp && tmp <= 9)
        {
            pushNum(&num, tmp);
            length[lengthFlag]++;
        }
        // 만약 tmp가 10이라면 입력값이 음수라는 의미로 digitFlag 를 false로 바꾼다.
        else if(tmp == 10)
        {
            digitFlag = false;
        }
        // 만약 tmp가 46이라면 소수점이 들어왔다는 의미이므로 digit의 앞 수 및 자리수를 저장해주고 lengthFlag를 변경해준다.
        else if(tmp == 46)
        {
            // 예외 처리 1. 소수점이 많이 나온 오류
            if(lengthFlag == 1)
            {
                printf("소수점에 문제가 있습니다. (8.8.8)");
                exit(1);
            }
            digit->beforeSize = length[0];
            digit->before = num;
            num = NULL;
            lengthFlag = 1;
        }
        // 만약 tmp가 -1이라면 숫자가 끝났다는 의미로 앞자리수가 0이라다면 정수란 의미로 앞자리에 0이 아니라면 뒷자리에 숫자를 정의해주고 각각의 수를 초기화 해준다. 그리고 피연산자 스택에 넣는다.
        else if(tmp == -1)
        {
            if(digit->beforeSize == 0)
            {
                digit->beforeSize = length[0];
                digit->before = num;
                digit->positive = digitFlag;
            }
            else
            {
                digit->afterSize = length[1];
                digit->after = num;
                digit->positive = digitFlag;
            }
            length[0] = 0;
            length[1] = 0;
            lengthFlag = 0;
            digitFlag = true;
            pushOperand(&operand, digit);
            digit = initializeDigit();
        }
        // 만약 input한 숫자가 연산자에 맞는 숫자라면 실행됨.
        else
        {
            // 만약 연산자가 들어온다면 우선 피연산자 스택에서 pop을 해준다.
            pDigit secondDigit = popOperand(&operand);
            pDigit firstDigit = popOperand(&operand);
//            printf("isBig: %d\n", isBig(secondDigit, firstDigit));
//            returnValue(copyDigit(firstDigit));
//            printf(" ");
//            returnValue(copyDigit(secondDigit));
//            printf("   %C\n", tmp);
            pDigit tmpDigit;
            switch (tmp)
            {
                // 만약에 피연산자가 곱셈 이라면
                case 42:
                    tmpDigit = multiply(firstDigit, secondDigit);
                    // 위에서 곱셈을 정의하고 만약 두 수의 positive 의 합이 1이라면 두 부호가 다르다는 의미로 음수로 결과값을 음수로 처리한다.
                    if((firstDigit->positive + secondDigit->positive) == 1)
                    {
                        tmpDigit->positive = false;
                    }
                    // 아니라면 결과값을 양수로 처리한다.
                    else
                    {
                        tmpDigit->positive = true;
                    }
                    pushOperand(&operand, tmpDigit);
                    break;

                // 만약 피연산자가 더하기라면
                case 43:
                    // 두 부호가 다르다는 의미
                    if((firstDigit->positive + secondDigit->positive) == 1)
                    {
                        if(firstDigit->positive)
                        {
                            // 앞수가 양수 뒷 수가 음수면 앞수 minus 뒤수 처리를 한다.
                            tmpDigit = minus(firstDigit, secondDigit);
                        }
                        else
                        {
                            // 앞수가 음수 뒷 수가 양수면 뒤수 minus 앞수 처리를 한다.
                            tmpDigit = minus(secondDigit, firstDigit);
                        }
                    }
                    else
                    {
                        tmpDigit = plus(firstDigit, secondDigit);
                        if(firstDigit->positive)
                        {
                            // 앞수가 양수 뒤수가 양수면 더하기로 처리한다.
                            tmpDigit->positive = true;
                        }
                        else
                        {
                            // 앞수가 음수 뒤수가 음수면 두 수를 더하고 음수로 처리한다.
                            tmpDigit->positive = false;
                        }
                    }
                    pushOperand(&operand, tmpDigit);
                    break;

                case 45:
//                    printf("%d", isBig(firstDigit, secondDigit));
                    // 뒤 수가 음수면
                    if(!(secondDigit->positive))
                    {
                        if(firstDigit->positive)
                        {
                            // 앞 수가 양수고 뒤 수가 음수면 두 수 더한다.
                            tmpDigit = plus(firstDigit, secondDigit);
                        }
                        else
                        {
                            // 앞 수가 음수고, 뒤 수가 양수면 뒤 수에서 앞수를 빼준다ㄴ
                            tmpDigit = minus(secondDigit, firstDigit);
                        }
                    }
                    else
                    {
                        if(firstDigit->positive)
                        {
                            // 만약 앞 수가 양수고, 뒤 수가 양수면 두 수를 바로 빼준다.
                            tmpDigit = minus(firstDigit, secondDigit);
                        }
                        else
                        {
                            // 만약 앞 수가 음수고, 뒤 수가 양수면 두 수를 더해주고 결과값을 음수 처리한다.
                            tmpDigit = plus(firstDigit, secondDigit);
                            tmpDigit->positive = false;
                        }
                    }
                    pushOperand(&operand, tmpDigit);
                    break;

                case 47:
                    tmpDigit = divide(firstDigit, secondDigit);
                    // 위에서 곱셈을 정의하고 만약 두 수의 positive 의 합이 1이라면 두 부호가 다르다는 의미로 음수로 결과값을 음수로 처리한다.
                    if((firstDigit->positive + secondDigit->positive) == 1)
                    {
                        tmpDigit->positive = false;
                    }
                        // 아니라면 결과값을 양수로 처리한다.
                    else
                    {
                        tmpDigit->positive = true;
                    }
                    pushOperand(&operand, tmpDigit);
                    break;
            }
        }
        seek = seek->next;
//        free(seek->prev);
    }
    return popOperand(&operand);
}

// Digit을 복사해주는 메서드로 pDigit을 입력받아 복사한 pDigit을 복사 해준 것을 리턴해준다.
pDigit copyDigit(pDigit digit)
{
    // 리턴할 pDigit 을 초기화해준다.
    pDigit result = initializeDigit();
    pNum tmpBefore = NULL;
    pNum tmpAfter = NULL;

    // before부터 하나씩 입력받아서 초기화 해준다.
    for(int i = 0; i < digit->beforeSize; i++)
    {
        pushNum(&tmpBefore, popNum(&(digit->before)));
    }
    for(int i = 0; i< digit->afterSize; i++)
    {
        pushNum(&tmpAfter, popNum(&(digit->after)));
    }

    for(int i = 0; i < digit->beforeSize; i++)
    {
        int tmp = popNum(&tmpBefore);
        pushNum(&(digit->before), tmp);
        pushNum(&(result->before), tmp);
    }

    for(int i = 0; i < digit->afterSize; i++)
    {
        int tmp = popNum(&tmpAfter);
        pushNum(&(digit->after), tmp);
        pushNum(&(result->after), tmp);
    }

    result->beforeSize = digit->beforeSize;
    result->afterSize = digit->afterSize;
    result->positive = digit->positive;
    return  result;
}



void makePostfix(char* str, pList pList, pOperator Operator)
{
    int i = 0;
    List list = *pList;
    pOperator topOperator = Operator;

    // str을 후위연산식으로 처리하는 메서드
    while(1)
    {
        int flag = 0;
        // 만약 문자열이 끝났으면 연산자 스택에서 남은 것들을 전부 pop 해준다.
        if(str[i] == '\0')
        {
            while(!emptyOperator(topOperator))
            {
                int tmp = popOperator(&topOperator);
                // 예외 처리 2. 괄호의 갯수가 다릅니다.
                if((tmp == 40) || (tmp == 41))
                {
                    printf("( 와 ) 의 숫자가 맞지 않습니다.\n");
                    exit(1);
                }
                addExpression(&list, tmp);
            }
            break;
        }

        // 만약 46번이 나온다면 아스키 코드로 .을 의미한다 넣어준다.
        if(str[i] == 46)
        {
            addExpression(&list, str[i]);
        }

        // 만약 48 ~ 57번 사이의 수가 나온다면 아스키 코드로 0 ~ 9를 의미하므로 '0'을 빼서 넣어준다.
        else if((48 <= str[i] && str[i] <= 57))
        {
            addExpression(&list, str[i] - '0');
            // 만약 다음 수가 숫자 혹은 소수점이 아니면 숫자가 끝났음을 의미하므로 -1을 추가해준다.
            if(!((48 <= str[i+1] && str[i+1] <= 57) || (str[i+1] == 46)))
            {
                addExpression(&list, -1);
            }
        }

        // 피연산자가 들어왔을 때 처리
        else if((str[i] == 40) || (str[i] == 41) || (str[i] == 42) || (str[i] == 43) || (str[i] == 45) || (str[i] == 47))
        {
            // 만약 스택이 비어있다면 무조건 추가한다.
            if(emptyOperator(topOperator))
            {
                // 하지만 만약 ( 앞에 숫자라면 *가 생략된 것 이므로 * 연산을 추가해준다.
                if(str[i] == 40)
                {
                    if((i != 0) && (48 <= str[i-1] && str[i-1] <= 57))
                    {
                        pushOperator(&topOperator, 42);
                    }
                }
                pushOperator(&topOperator, str[i]);
            }
            else
            {
                char tmp = popOperator(&topOperator);
                // 만약 ) 가 나왔다면 ( 가 나올 때 까지 스택을 전부 뺀다.
                if(str[i] == 41)
                {
                    if(tmp != 40)
                    {
                        addExpression(&list, tmp);
                        while(popOperator(&topOperator) != 40)
                        {
                            addExpression(&list, popOperator(&topOperator));
                        }
                    }

                    // 만약 다음이 0 ~ 9 사이에 있다면 마찬가지로 *를 더해준다.
                    if((48 <= str[i+1] && str[i+1] <= 57))
                    {
                        pushOperator(&topOperator, 42);
                    }
                }
                // 만약 연산자가 * 혹은 / 라면 두 번째 우선 순위
                else if((str[i] == 42) || (str[i] == 47))
                {
                    // 만약 그 전 연산자가 ( + - 라면 그냥 푸쉬를 한다.
                    if((tmp == 43) || (tmp == 45) || (tmp == 40))
                    {
                        pushOperator(&topOperator, tmp);
                        pushOperator(&topOperator, str[i]);
                    }
                    // 그게 아니라면 pop 을 한 이후에 push를 한다.
                    else
                    {
                        addExpression(&list, tmp);
                        pushOperator(&topOperator, str[i]);
                    }
                }
                // 만약 연산자가 + - 라면 세 번째 우선 순위
                else if((str[i] == 43) || (str[i] == 45))
                {
                    // 만약 (- 형태라면 음수라는 걸 표현해준다.
                    if((str[i-1] == 40) && (str[i] == 45))
                    {
                        addExpression(&list, 10);
                        pushOperator(&topOperator, tmp);
                    }
                    else
                    {
                        // 만약 그 전이 ( 라면 무시하고 push를 해준다.
                        if(tmp == 40)
                        {
                            pushOperator(&topOperator, tmp);
                            pushOperator(&topOperator, str[i]);
                        }
                        // 그게 아니라면 pop을 한 이후 후위식에 넣어주고 들어 온 것을 push 해준다.
                        else
                        {
//                            printf("%C", tmp);
                            addExpression(&list, tmp);
                            pushOperator(&topOperator, str[i]);
                        }
                    }
                }
                // 만약 ( 라면
                else
                {
                    if(i != 0)
                    {
                        // 전이 숫자라면 3( 의 경우
                        if((48 <= str[i-1] && str[i-1] <= 57))
                        {
                            // ( + - 일때는 스택에 쌓아주고 ( 를 넣는다.
                            if((tmp == 43) || (tmp == 45) || (tmp == 40))
                            {
                                pushOperator(&topOperator, tmp);
                                pushOperator(&topOperator, 42);
                                pushOperator(&topOperator, str[i]);
                            }
                            // 만약 * / 라면 pop을 해서 연산자 Stack에 쌓아주고 * 를 push 해준다.
                            else
                            {
                                addExpression(&list, tmp);
                                pushOperator(&topOperator, 42);
                                pushOperator(&topOperator, str[i]);
                            }
                        }
                        // 만약 숫자가 아니라면 바로 push를 해준다.
                        else
                        {
                            pushOperator(&topOperator, tmp);
                            pushOperator(&topOperator, str[i]);
                        }
                    }
                }
            }
        }
        else
        {
            if((33 <= str[i]) && (str[i] <= 126))
            {
                // 예외 처리 3. 옳지 않은 문자(%, @)가 들어있습니다.
                printf("옳지 않은 문자가 들어있습니다.\n");
                exit(1);
            }
        }

        if(((str[i] == 42) || (str[i] == 43) || (str[i] == 45) || (str[i] == 47)) && ((str[i+1] == 42) || (str[i+1] == 43) || (str[i+1] == 45) || (str[i+1] == 47)))
        {
            // 예외 처리 4. 연산자가 중복해서 들어있습니다.
            printf("연산자에 문제가 있습니다 eg) ++ , --, **, //");
            exit(1);
        }
        i++;
    }
}

bool isBig(pDigit first, pDigit second)
{
    bool flag = false;

//    printf("\n\nisBig: ");
//    returnValue(copyDigit(first));
//    printf ("   ");
//    returnValue(copyDigit(second));
//    printf("\n\n");
    if(first->beforeSize < second->beforeSize)
    {
        flag = true;
    }

    else if(first->beforeSize == second-> beforeSize)
    {
        pNum firstTmp = NULL; pNum secondTmp = NULL;
        int firstNum, secondNum;
        if(first->afterSize > second->afterSize)
        {
            for(int i = 0; i < (first->afterSize - second->afterSize); i++)
            {
                pushNum(&firstTmp, popNum(&(first->after)));
            }

            for(int i = 0; i < second->afterSize; i++)
            {
                firstNum = popNum(&(first->after));
                secondNum = popNum(&(second->after));
                if(firstNum < secondNum)
                {
                    flag = true;
                }
                else if(firstNum > secondNum)
                {
                    flag = false;
                }
                pushNum(&firstTmp, firstNum);
                pushNum(&secondTmp, secondNum);
            }
        }
        else
        {
            flag = true;
            for(int i = 0; i < (second->afterSize - first->afterSize); i++)
            {
                pushNum(&secondTmp, popNum(&(second->after)));
            }

            for(int i = 0; i < first->afterSize; i++)
            {
                firstNum = popNum(&(first->after));
                secondNum = popNum(&(second->after));
                if(firstNum < secondNum)
                {
                    flag = true;
                }
                else if(firstNum > secondNum)
                {
                    flag = false;
                }
                pushNum(&firstTmp, firstNum);
                pushNum(&secondTmp, secondNum);
            }
        }

        if(first->afterSize > second->afterSize)
        {
            for(int i = 0; i < (first->afterSize - second->afterSize); i++)
            {
                pushNum(&(first->after), popNum(&firstTmp));
            }
            for(int i = 0; i < second->afterSize; i++)
            {
                pushNum(&(first->after), popNum(&firstTmp));
                pushNum(&(second->after), popNum(&secondTmp));
            }
        }
        else
        {
            for(int i = 0; i < (second->afterSize - first->afterSize); i++)
            {
                pushNum(&(second->after), popNum(&secondTmp));
            }
            for(int i = 0; i < first->afterSize; i++)
            {
                pushNum(&(first->after), popNum(&firstTmp));
                pushNum(&(second->after), popNum(&secondTmp));
            }
        }

        firstTmp = NULL; secondTmp = NULL;
        for(int i = 0; i < first->beforeSize; i++)
        {
            firstNum = popNum(&(first->before));
            secondNum = popNum(&(second->before));
            if(firstNum < secondNum)
            {
                flag = true;
            }
            else if(firstNum > secondNum)
            {
                flag = false;
            }
            pushNum(&firstTmp, firstNum);
            pushNum(&secondTmp, secondNum);
        }
        for(int i = 0; i < first->beforeSize; i++)
        {
            pushNum(&(first->before), popNum(&firstTmp));
            pushNum(&(second->before), popNum(&secondTmp));
        }
    }
    return flag;
}

bool isSame(pDigit first, pDigit second)
{
    bool flag = true;

    if((first->beforeSize == second->beforeSize) && (first->afterSize == second->afterSize))
    {
        pNum firstBefore = NULL; pNum secondBefore = NULL;
        for(int i = 0; i < first->beforeSize; i++)
        {
            int firstNum, secondNum;
            firstNum = popNum(&(first->before));
            secondNum = popNum(&(second->before));
            if(firstNum != secondNum)
            {
                flag = false;
            }
            pushNum(&firstBefore, firstNum);
            pushNum(&secondBefore, secondNum);
        }

        for(int i = 0; i < first->beforeSize; i++)
        {
            pushNum(&(first->before), popNum(&firstBefore));
            pushNum(&(second->before), popNum(&secondBefore));
        }

        pNum firstAfter = NULL; pNum secondAfter = NULL;
        for(int i = 0; i < first->afterSize; i++)
        {
            int firstNum, secondNum;
            firstNum = popNum(&(first->after));
            secondNum = popNum(&(second->after));
            if(firstNum != secondNum)
            {
                flag = false;
            }
            pushNum(&firstAfter, firstNum);
            pushNum(&secondAfter, secondNum);
        }

        for(int i = 0; i < first->afterSize; i++)
        {
            pushNum(&(first->after), popNum(&firstAfter));
            pushNum(&(second->after), popNum(&secondAfter));
        }

        return flag;
    }
    return false;
}