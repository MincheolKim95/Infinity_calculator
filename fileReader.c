#define _CRT_SECURE_NO_WARRINGS // fopen 보안 경고로 인한 컴파일 에러 방지
#include "calculator.h"

char* openFile(FILE* ifp)
{
    // 만약 ifp가 NULL 이라면 파일이 없는 에러 출력
    if(ifp == NULL)
    {
        printf("File not detected!\n");
        exit(0);
    }

    // 문자열을 저장할 str 선언 및 임시 저장할 c와 size를 초기화
    char tmp, *str = NULL;
    int size = 0;

    // 우선 처음 모든 텍스트를 확인하여 글자의 크기를 맞춘다.
    while(fscanf(ifp, "%c", &tmp) == 1) size++;

    // 글자 크기에 맞춰서 저장할 str을 동적할당함.
    str = (char*)malloc(sizeof(char) * (size + 1));

    // 파일을 다시 처음부터
    rewind(ifp);

    // 문자열을 다시 읽어 이제는 str에 저장해줌.
    int i = 0;
    while(fscanf(ifp, "%c", &tmp) == 1)
    {
        *(str + i++) = tmp;
    }
    *(str + i) = '\0';
    return str;
}