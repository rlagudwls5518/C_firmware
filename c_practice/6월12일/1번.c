#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

void printBinary(int n);
void changeNum(char str[], int num[]);

void main_1() {
	printf("비트연산을 수행할 두 개의 정수를 입력하세요. >> ");

	char str[10];
	int result[9] = { 0 };
    int num[2] = { 0 };
    char form[100];

    fgets(str, 10, stdin);
	changeNum(str, num);

    if (num[0] == 999 && num[1] == 999) {
		printf("종료합니다.\n");
		return;
    }

    for (int i = 0; i < 2; i++) {
        printf("%25d:", num[i]);
        printBinary(num[i]);
    }

    sprintf_s(form,sizeof(form), "%d  &   %d  =  %d", num[0], num[1], num[0] & num[1]);
	printf("%25s:", form);
    printBinary(num[0] & num[1]);

    sprintf_s(form, sizeof(form), "%d  |   %d  =  %d", num[0], num[1], num[0] | num[1]);
    printf("%25s:", form);
    printBinary(num[0] | num[1]);

    sprintf_s(form, sizeof(form), "%d  ^   %d  =  %d", num[0], num[1], num[0] ^ num[1]);
    printf("%25s:", form);
    printBinary(num[0] ^ num[1]);

    sprintf_s(form, sizeof(form), "%d  >>   %d  =  %d", num[0], num[1], num[0] >> num[1]);
    printf("%25s:", form);
    printBinary(num[0] >> num[1]);

    sprintf_s(form, sizeof(form), "%d  <<   %d  =  %d", num[0], num[1], num[0] << num[1]);
    printf("%25s:", form);
    printBinary(num[0] << num[1]);

    sprintf_s(form, sizeof(form), "~%d   =  %d", num[0], ~num[0]);
    printf("%25s:", form);
    printBinary(~num[0]);

    sprintf_s(form, sizeof(form), "~%d   =  %d", num[1], ~num[1]);
    printf("%25s:", form);
    printBinary(~num[1]);

    sprintf_s(form, sizeof(form), "~%d  +  1   =  %d", num[0], ~num[0] + 1);
    printf("%25s:", form);
    printBinary(~num[0] + 1);

    sprintf_s(form, sizeof(form), "~%d  +  1   =  %d", num[1], ~num[1] + 1);
    printf("%25s:", form);
    printBinary(~num[1] + 1);

}

void changeNum(char str[], int num[]) {
    int str_idx = 0;
    int num_idx = 0;
    
    while (str[str_idx] != '\n') {
    //5 387
        if (str[str_idx] >= '0' && str[str_idx] <= '9') {
            int number = str[str_idx] - '0';
			num[num_idx] = (num[num_idx] * 10) + number;
        }
        str_idx++;

        if(str[str_idx] == ' ') num_idx++;
        
    }
}

void printBinary(int n) {
    printf("                ");
    unsigned int mask = 1 << 31;

    for (int i = 0; i < 32; i++) {

        // 비트가 1인지 0인지 AND 연산으로 판별
        if ((n & mask) != 0) {
            printf("1");
        }
        else {
            printf("0");
        }
        if (i % 4 == 3) {
			printf(" ");
        }
        // 다음 비트를 확인하기 위해 mask를 오른쪽으로 1칸 이동
        mask >>= 1;
    }
    printf("\n");
}