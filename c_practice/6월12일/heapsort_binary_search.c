#include <stdio.h>
#include "heap_sort.h"
/*
------- 아래 ------
 1. 최대 입력 data는 20개
 2. 입력 fgets 함수
 enter data (종료: exit) : 22
 enter data (종료: exit) : 11
 enter data (종료: exit) : 7
 enter data (종료: exit) : 3
 enter data (종료: exit) : 2
 enter data (종료: exit) : 1
 enter data (종료: exit) : exit
3. Heap sort로 data를 sort 한다.
 **** input data ****
 22 11 7 3 2 1
 **** sort data ****
 1 2 3 7 11 22
4. 찾고자 하는 data는 ?(전체: all 종료:exit) 22
   원하는 data 22를 찾았습니다
   찾고자 하는 data는 ?(전체: all 종료:exit) 99
    99를 찾을 수 없습니다.
    찾고자 하는 data는 ?(전체: all 종료:exit) all
     1 2 3 7 11 22
    찾고자 하는 data는 ?(전체: all 종료:exit) exit
   프로그램이 종료 되었습니다 !!!!!
*/
void heap_sort(element a[], int n);
void binary_search(int a[], int n, int key);

int main(void)
{
    char buff[40];
    int array[100];
	int idx = 0;

    while (1)
    {
        printf("enter data (종료: exit) : ");
        fgets(buff, 40, stdin);
		if (buff[0] == 'e' && buff[1] == 'x' && buff[2] == 'i' && buff[3] == 't') {
			break;
		}
		if (idx >= 20) {
			printf("최대 입력 data는 20개입니다.\n");
			break;
		}
		array[idx] = atoi(buff);
		idx++;

    }
	printf("**** input data ****\n");
	for (int i = 0; i < idx; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");

	heap_sort(array, idx);

	printf("**** sort data ****\n");
	for (int i = 0; i < idx; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");


	while (1)
	{
		printf("찾고자 하는 data는 ?(전체: all 종료:exit) ");
		fgets(buff, 40, stdin);
		if (buff[0] == 'e' && buff[1] == 'x' && buff[2] == 'i' && buff[3] == 't') {
			break;
		}
		else if (buff[0] == 'a' && buff[1] == 'l' && buff[2] == 'l') {
			for (int i = 0; i < idx; i++) {
				printf("%d ", array[i]);
			}
			printf("\n");
		}
		else {
			binary_search(array, idx, atoi(buff));
		}

	}
	printf("프로그램이 종료 되었습니다 !!!!!");
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

void binary_search(int a[], int n, int key) {
	int left = 0;
	int right = n - 1;
	while (left <= right) {
		int mid = (left + right) / 2;
		if (key == a[mid]) {
			printf("원하는 data %d를 찾았습니다\n", key);
			return;
		}
		else if (key < a[mid]) {
			right = mid - 1;
		}
		else {
			left = mid + 1;
		}
	}
	printf("%d를 찾을 수 없습니다.\n", key);
}