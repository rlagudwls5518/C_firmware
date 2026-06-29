/*
 * calculator.c
 *
 * Created: 2026-06-29 오후 4:09:43
 *  Author: kccistc
 */ 
#include <stdio.h>
#define STACK_SIZE 10

long num_stack[STACK_SIZE];  // 숫자 전용 서랍
char op_stack[STACK_SIZE];  // 연산자 전용 서랍
long input_value = 0;
int num_top = -1;           // 숫자 서랍의 위치 포인터
int op_top = -1;
long result = 0;

int priority(char op);
void calculator(char key_value);
void print_calculator(int key);

void print_calculator(int key){
	printf("%c ", key);
	if(key == '=') printf(" %ld\n", result);
	
}

void calculator(char key_value) {
	// 1. 숫자가 들어온 경우 (문자 '0'~'9' 처리)
	if (key_value >= '0' && key_value <= '9') {
		input_value = (input_value * 10) + (key_value -'0'); 
	}
	
	// 2. 사칙연산자(+, -, *, /)가 들어온 경우
	else if (key_value == '+' || key_value == '-' || key_value == '*' || key_value == '/') {
		num_stack[++num_top] = input_value;
		input_value = 0;

		while (op_top != -1 && priority(op_stack[op_top]) >= priority(key_value)) {
			
			char op = op_stack[op_top--];
			
			int n2 = num_stack[num_top--];
			int n1 = num_stack[num_top--];
			
			// 3) 계산 후 결과를 다시 숫자 스택에 넣기 (Push)
			int res = 0;
			if (op == '+') res = n1 + n2;
			else if (op == '-') res = n1 - n2;
			else if (op == '*') res = n1 * n2;
			else if (op == '/') res = n1 / n2; 
			
			num_stack[++num_top] = res;
		}
		
		
		op_stack[++op_top] = key_value;
	}
	
	// 3. '=' 버튼이 들어온 경우
	else if (key_value == '=') {
		num_stack[++num_top] = input_value;
		input_value = 0;
		
		while (op_top != -1) {
			char op = op_stack[op_top--];
			int n2 = num_stack[num_top--];
			int n1 = num_stack[num_top--];
			
			int res = 0;
			if (op == '+') res = n1 + n2;
			else if (op == '-') res = n1 - n2;
			else if (op == '*') res = n1 * n2;
			else if (op == '/') res = n1 / n2;
			
			num_stack[++num_top] = res;
		}
		
		result = num_stack[num_top];
		
		num_top = -1;
		op_top = -1;
	}
}

int priority(char op){
	if(op == '+' || op == '-') return 1;
	if(op == '*' || op == '/') return 2;
	return 0;
}