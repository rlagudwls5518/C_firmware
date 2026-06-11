/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 

#include "led.h"
void init_led(void);
void led_all_on(void);
void led_all_off(void);
void led_shift_left_on(void);
void led_shift_right_on(void);
void led_shift_left_keepon(void);
void led_shift_right_keepon(void);
void led_flower_on(void);
void led_flower_off(void);

void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off 	
}

int led_main(void){
	
	led_all_off();

	
	while(1){
		led_flower_on();
		led_flower_off();
	}
	
}

void led_all_on(void)
{
	PORTA=0xff;
}

void led_all_off(void)
{
	PORTA=0x00;
}

void led_shift_left_on(void){
	for(int i = 0; i< 8; i++){
		PORTA = 1 << i; //PORTA : 0x1B
		_delay_ms(300);
	}
}

void led_shift_right_on(void){

	for(int i = 7; i >= 0; i--){
		PORTA = 1 << i; //PORTA : 0x1B
		_delay_ms(300);
	}
}

void led_shift_left_keepon(void){
	for(int i = 0; i< 8; i++){
		PORTA |= (1 << i); 
		_delay_ms(300);
	}
}

void led_shift_right_keepon(void){
	for(int i = 7; i >= 0; i--){
		PORTA |= (1 << i);
		_delay_ms(300);
	}
}

void led_flower_on(void){
	for(int i = 0; i < 4 ; i++){
		PORTA |= (0x10 << i); //left
		PORTA |= (0x08 >> i); //right
		_delay_ms(300);
	}
}

void led_flower_off(void){
	for(int i = 0; i < 4 ; i++){
		PORTA &= ~(0x80 >> i); // left
		PORTA &= ~(0x01 << i); // right
		_delay_ms(300);
	}
}
