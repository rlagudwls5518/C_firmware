#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void swap(int* a, int* b);
void quicksort(int* arr, int start, int end);

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

    quicksort(array, 0, count - 1);
    printf("\n");
    printf("**** sort data ****\n");
    for (int i = 0; i < count; i++) {
        printf("%d ", array[i]);
    }

}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort(int* arr, int start, int end) {
    if (start >= end) return;
    int pivot = start;
    int i = start + 1;
    int j = end;

    while (i <= j) {
        while (i <= end && arr[i] <= arr[pivot]) i++;
        while (j > start && arr[j] >= arr[pivot]) j--;

        if (i > j) break;
        else swap(&arr[i], &arr[j]);
    }

    swap(&arr[pivot], &arr[j]);

    quicksort(arr, start, j - 1);
    quicksort(arr, j + 1, end);
}