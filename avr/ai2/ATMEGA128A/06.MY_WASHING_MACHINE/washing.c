/*
 * washing.c
 *
 * Created: 2026-06-18 오후 4:04:34
 *  Author: kccistc
 */ 

#include "washing.h"
#include "button.h"

enum WASHING_MODE{
	WAITING = 0,
	WASHING = 1,
	RINSE = 2,
	DRY = 3
};

int start_stop_button = 0; // 시작버튼 플래그
uint32_t mode_time = 0; // 모드 시간
static int current_mode = WAITING; // 현재모드
static int set_mode = WAITING; //설정모드
static int time_store[4] = {0};//시간 저장
volatile uint32_t left_time;//남은 시간


extern void fnd_washing_time_display();
extern void fnd_washing_time_set_display(int set_time);

void init_washing_motor_driver();
void washing_comtrol_main();
void init_timer3_pwm();
void mode_change(int mode);

extern int get_button(int button_num, int button_pin);


void init_washing_motor_driver(){
	DDRF |= 1 << 6 | 1 << 7;
	PORTF &= ~(1 << 6 | 1 << 7);
	PORTF |= 1 << 6;
}

void init_timer3_pwm(){
	//PE4 초음파 연결
	DDRE |= 1 << 3 | 1 << 5;
	
	//모드 5번 사용 8bit 고속 BWM모드
	TCCR3A |= 1 << WGM30;
	TCCR3B |= 1 << WGM32;
	
	//비반전 모드 top : 0x00ff 비교일치 값 (PWM) 지정 OCR3
	TCCR3A |= 1 << COM3C1;
	
	TCCR3B |= 1 << CS31 | 1 << CS30;
	OCR3C = 0; 
}


//버튼 제어함수
void washing_comtrol_main(){
	
	int forward = 0;
	
	while(1){
		if(get_button(BUTTON0, BUTTON0PIN)){ // 시작버튼
			start_stop_button = !start_stop_button;
			
			if(start_stop_button && current_mode == WAITING){
				mode_change(WASHING);
			}else{
				mode_change(WAITING);
				start_stop_button = 0;
				set_mode = WAITING;
			}
			
		} else if(start_stop_button == 0 && get_button(BUTTON1, BUTTON1PIN)){ // 모드변경버튼
			set_mode++;	
			if(set_mode > DRY) set_mode = WASHING;
			
			if(set_mode == WASHING) PORTA = 0x01;
			else if(set_mode == RINSE) PORTA = 0x03;
			else if(set_mode == DRY) PORTA = 0x07;
			
		} else if(start_stop_button == 0 && get_button(BUTTON2, BUTTON2PIN)){ // 시간추가버튼
			time_store[set_mode] += 60;//1분씩 추가
			
		} else if(get_button(BUTTON3, BUTTON3PIN)){ // 정방향 역방향 버튼
			if(start_stop_button){
				forward = !forward;
				PORTF &= ~(1 << 6 | 1 << 7);
				if(forward) PORTF |= 1 << 6; // 정방향
				else PORTF |= 1 << 7; // 역방향	
			}
		}
	
		
		if(start_stop_button && current_mode != WAITING) { // FND화면
			fnd_washing_time_display(); 
		} else {
			fnd_washing_time_set_display(time_store[set_mode]); 
		}
			
			
		if (start_stop_button && left_time == 0){ // 시간이끝났을때 다음모드로 이동
			if(current_mode == DRY){
				mode_change(WAITING);
				start_stop_button = 0;
				set_mode = WAITING;
			}
			else mode_change(current_mode + 1);
		}
	}
}

//모드 변경 함수
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
}