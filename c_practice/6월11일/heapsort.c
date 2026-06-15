#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap_sort.h"
/*
------- 아래 ------
 1. 최대 입력 data는 10개로
 2. 입력 fgets 함수

 enter data (종료: exit) : 11
 enter data (종료: exit) : 7
 enter data (종료: exit) : 3
 enter data (종료: exit) : exit
 **** input data ****
 11 7 3
 **** sort data ****
 3 7 11

*/

char buff[40];
int array[100];
void heap_sort(element a[], int n);

int main(void)
{
    int count = 0;

    while (1)
    {
        printf("enter data (종료: exit) : ");
        fgets(buff, 40, stdin);
        if (!strcmp(buff, "exit\n")) break;
        else array[count] = atoi(buff);
        count++;
        if (count >= 10) {
            printf("입력은 10개를 초과할 수 없습니다.\n");
            break;
        }
    }
    printf("**** input data ****\n");

    for (int i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }

	heap_sort(array, count);

    printf("\n");
    printf("**** sort data ****\n");
    for (int i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }

}

void heap_sort(element a[], int n) {
    HeapType* h;
    h = create();
    init(h);
    for (int i = 0; i < n; i++) {
        insert_max_heap(h, a[i]);
    }
    for (int i = n - 1; i >= 0; i--) {
        a[i] = delete_max_heap(h);
    }
}