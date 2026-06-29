/*
 * calculator.c
 *
 * Created: 2026-06-29 오후 4:09:43
 *  Author: kccistc
 */ 
#include <stdio.h>
int result = 0;
int current_num = 0;

void calculator(int key);
void print_calculator(int key);

void print_calculator(int key){
	printf("%c ", key);
	if(key == '='){
		 
		 printf(" %d\n", result);
		 result = 0;
	}
}

void calculator(int key){
	int input_digit = key - '0';

	if(input_digit >= 0 && input_digit <= 9){
		current_num = (current_num * 10) + input_digit;
	}
	else{
		switch(key){
			
			case '+':
			result += current_num;
			break;
			case '-':
			result -= current_num;
			break;
			case '*':
			result *= current_num;
			break;
			case '/':
			result /= current_num;
			break;
		}
		current_num = 0;
	}
	
	
	
}