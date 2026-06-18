/*
 * washing.c
 *
 * Created: 2026-06-18 오후 4:04:34
 *  Author: kccistc
 */ 

#include "button.h"
#include "pwm.h" 

extern volatile uint32_t min_count;
int start_stop_button = 0;
int washing_mode = 0;
enum MODE{
	WASHING = 0;
	RINSE = 1;
	DRY = 2;
	WAITING = 3;
	};
	
void init_timer3_pwm();
void init_washing_motor_driver();
void washing_comtrol_main();

int time_store[3] = {0};
static int mode_time = 0;

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
void washing_main(){
	
}

void washing_comtrol_main(){
	
	while(1){
		if(get_button(BUTTON0, BUTTON0PIN)){
			if(start_stop_button){
				waiting_mode();
			}else{
				washing_mode();
			}
			start_stop_button = ! start_stop_button;
			
		} else if(get_button(BUTTON1, BUTTON1PIN)){
			static int mode_time = 0;
			switch(mode_time){
				case 0:
					
					break;
				case 1:
					break;
				case 2:
					break;
			}
			mode_time = mode_time % 3;
			
		} else if(get_button(BUTTON2, BUTTON2PIN)){
			
			if(mode_time == 0) time_store[0]++;
			else if(mode_time == 1) time_store[1]++;
			else time_store[2]++;
			
		} else if(get_button(BUTTON3, BUTTON3PIN)){
			
		}
	}
}

void waiting_mode(){
	washing_mode = WAITING;
	PORTA = 0x00;
	min_count = 0;
	
}
void washing_mode(){
	washing_mode = WASHING;
	PORTA = 0x01;
}
void rinse_mode(){
	washing_mode = RINSE;
	PORTA = 0x02;
}
void dry_mode(){
	washing_mode = DRY;
	PORTA = 0x03;
}