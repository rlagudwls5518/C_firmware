/*
 * washing.c
 *
 * Created: 2026-06-18 오후 4:04:34
 *  Author: kccistc
 */ 

#include "washing.h"
#include "button.h"

extern volatile uint32_t min_count;
int start_stop_button = 0; // 시작버튼 플래그
int current_mode = 0; // 현재모드
int set_mode = 0; //설정모드
uint32_t mode_time = 0; // 모드 시간
static int time_store[4] = {0};//시간 저장
volatile uint32_t left_time = 0;//남은 시간

extern void fnd_washing_time_display(int left_time);

enum WASHING_MODE{
	WAITING = 0,
	WASHING = 1,
	RINSE = 2,
	DRY = 3
};

void init_timer3_pwm();
void init_washing_motor_driver();
void washing_comtrol_main();
void mode_change(int mode);

extern int get_button(int button_num, int button_pin);

void init_timer3_pwm(){
	//PE4 초음파 연결
	DDRE |= 1 << 3 | 1 << 5;

	TCCR3A |= 1 << WGM30;
	TCCR3B |= 1 << WGM32;

	TCCR3A |= 1 << COM3C1;
	
	TCCR3B |= 1 << CS31 | 1 << CS30;
	OCR3C = 0;

}


void init_washing_motor_driver(){
	DDRF |= 1 << 6 | 1 << 7;
	PORTF &= ~(1 << 6 | 1 << 7);
	PORTF |= 1 << 6;
}

void washing_comtrol_main(){
	
	while(1){
		if(get_button(BUTTON0, BUTTON0PIN)){
			start_stop_button = ! start_stop_button;
			if(start_stop_button){
				mode_change(WASHING);
			}else{
				mode_change(WAITING);
			}
			
		} else if(!start_stop_button && get_button(BUTTON1, BUTTON1PIN)){
			set_mode++;	
			if(set_mode > 3) set_mode = 1;
			
		} else if(!start_stop_button && get_button(BUTTON2, BUTTON2PIN))
			time_store[set_mode] += 60000;//1분씩 추가
		
		
		if (start_stop_button && left_time == 0){
			if(current_mode == DRY){
				mode_change(WAITING);
				start_stop_button = 0;
			}
			else mode_change(current_mode + 1);
		}
	}
}

void mode_change(int mode){
	current_mode = mode;
	
	if(current_mode == WAITING) {
		PORTA = 0x00;
		OCR3C = 0;
	}
	
	else if(current_mode == WASHING){
		OCR3C = 100;
		PORTA = 0x01;
	}
	else if(current_mode == RINSE){
		OCR3C = 200;
		PORTA = 0x03;
	}
	else if(current_mode == DRY){
		OCR3C = 250;
		PORTA = 0x07;
	}
	
	left_time = time_store[mode];
	fnd_washing_time_display(left_time);
}