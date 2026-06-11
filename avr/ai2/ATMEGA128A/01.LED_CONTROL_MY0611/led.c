/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 

#include "led.h"
void init_led(void);
void led_main(int button_state);
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

void led_main(int button_state){
	
	switch(button_state){
		case 0:
			led_all_off();
			break;
		case 1:
			led_all_on();
			break;
		case 2:
			led_shift_left_on();
			break;
		case 3:
			led_shift_right_on();
			break;
		case 4:
			led_shift_left_keepon();
			break;
		case 5:
			led_shift_right_keepon();
			break;
		case 6:
			led_flower_on();
			break;
		case 7:
			led_flower_off();
			break;
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
	
#if 1
	static int i = 0;
	*(unsigned char *) 0x3B = 1 << i; //PORTA : 0x1B
	_delay_ms(30); //240ms소요
	
	i = (i + 1) % 8; //다음 인덱스 값을 계산
	
#else	//오리지날
	for(int i = 0; i< 8; i++){
		*(unsigned char *) 0x3B = 1 << i; //PORTA : 0x1B
		_delay_ms(30); //240ms소요
	}
#endif	
}

void led_shift_right_on(void){

#if 1
	static int i = 0;
	*(unsigned char *) 0x3B = 0x80 >> i; //PORTA : 0x1B
	_delay_ms(30); //240ms소요
	
		i = (i + 1) % 8; //다음 인덱스 값을 계산
		
#else
	for(int i = 7; i >= 0; i--){
		*(unsigned char *) 0x3B = 1 << i; //PORTA : 0x3B
		_delay_ms(30);
	}
#endif	
}

void led_shift_left_keepon(void){
#if 1
	static int i = 0;
	PORTA |= 1 << i;
	_delay_ms(30); 

	i++;
	if(i >= 8){ 
		i = 0;
		
	}
#else
	for(int i = 0; i< 8; i++){
		PORTA |= 1 << i;
		_delay_ms(300);
	}
#endif	
}

void led_shift_right_keepon(void){
	
#if 1
	static int i = 7;
	PORTA |= 1 << i;
	_delay_ms(30); 

	i--;
	if(i < 0){
		i = 7;
		
	}
		
#else
	for(int i = 7; i >= 0; i--){
		PORTA |= 1 << i;
		_delay_ms(300);
	}
#endif	
}

void led_flower_on(void){
#if 1
	static int i = 0;
	if(i >= 4)return;
	PORTA |= 0x10 << i; //left
	PORTA |= 0x08 >> i; //right
	_delay_ms(300);
	i++;
	
#else	
	for(int i = 0; i < 4 ; i++){
		PORTA |= 0x10 << i; //left
		PORTA |= 0x08 >> i; //right
		_delay_ms(300);
	}
#endif	
}

void led_flower_off(void){
#if 1	
	static int i = 0;
	if(i >= 4)return;
	PORTA &= ~(0x80 >> i); // left
	PORTA &= ~(0x01 << i); // right
	_delay_ms(300);
	i++;
#else	
	for(int i = 0; i < 4 ; i++){
		PORTA &= ~(0x80 >> i); // left
		PORTA &= ~(0x01 << i); // right
		_delay_ms(300);
	}
#endif	
}
