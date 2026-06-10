/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:44
 *  Author: kccistc
 */ 
#include "led.h"

void init_led(void);
void led_all_on(void);
void led_all_off(void);


void init_led(void){
	DDRA=0xff; //PORTA를 출력모드로 설정
	PORTA = 0x00; //PORTA에 물려있는 led를 all off
}

void led_all_on(void){
	PORTA=0xff;
}

void led_all_off(void){
	PORTA=0x00;
}

void led_first_on(void){
	PORTA=0xF0;
}

void led_second_on(void){
	PORTA=0x0F;
}

void led_odd_on(void){
	PORTA=0x55;
}

void led_even_on(void){
	PORTA=0xAA;
}