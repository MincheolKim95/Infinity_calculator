#include <stdlib.h>
#include <stdio.h>

#define bool   _Bool
#define false  0
#define true   1

typedef struct expression
{
    int num;
    struct expression* prev;
    struct expression* next;
} Expression, *pExpression;

typedef struct list
{
    pExpression head;
    pExpression tail;
} List, *pList;

typedef struct operator
{
    char operator;
    struct operator* next;
} Operator, *pOperator;

typedef struct num
{
    int num;
    struct num* next;
} Num, *pNum;

typedef struct digit
{
    bool positive;
    int beforeSize;
    int afterSize;
    struct num* before;
    struct num* after;
} Digit, *pDigit;

typedef struct operand
{
    struct digit* digit;
    struct operand* next;
} Operand, *pOperand;


void initList(pList list);
pNum getNum(int num);
int pushNum(pNum* top, int op);
int emptyNum(pNum top);
int popNum(pNum* top);
pOperand getOperand(pDigit digit);
int pushOperand(pOperand* top, pDigit digit);
int emptyOperand(pOperand top);
pDigit popOperand(pOperand* top);
pOperator getOperator(char op);
int pushOperator(pOperator* top, char op);
int emptyOperator(pOperator top);
char popOperator(pOperator* top);
void addExpression(pList list, int num);
void makePostfix(char* str, pList list, pOperator topOperator);
pDigit makeExpression(pList list);
void returnValue(pDigit digit);
pDigit initializeDigit();
pDigit copyDigit(pDigit digit);
void viewAll(pList list);
char* openFile(FILE* ifp);
bool isBig(pDigit first, pDigit second);
void round(pDigit digit, int round);
bool isSame(pDigit first, pDigit second);