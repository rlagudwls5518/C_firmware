/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:06
 *  Author: kccistc
 */
#include "pwm.h" 

void init_timer3_pwm();
void init_motor_driver();
void dcmoter_pwm_comtrol_main();

extern int get_button(int button_num, int button_pin);

// 16 bit 3번 timer/counter를 사용
/*
	pwm 출력신호
	==========
	PE3 = OC3A
	PE4 = OC3B : INT4 사용(초음파 센서)
	PE5 = OC3C ==> DC MOTOR (PWM출력)
	BTN1 : speed - up
	BTN2 : speed - down
*/
void init_timer3_pwm(){
	//PE4 초음파 연결
	DDRE |= 1 << 3 | 1 << 5;
	
	//모드 5번 사용 8bit 고속 BWM모드
	TCCR3A |= 1 << WGM30;
	TCCR3B |= 1 << WGM32;
	
	//비반전 모드 top : 0x00ff 비교일치 값 (PWM) 지정 OCR3
	TCCR3A |= 1 << COM3C1;
	
	//분주비 64분주 
	//16000000Hz / 64 => 250000Hz (250KHz)
	//펄스 하나당 T = 1/f  1/250000Hz --> 0.000004sec -> 4us
	//250000Hz에서 256개의 펄스를 카운트하면 소요시간 : 1.02ms
	//127개 펄스를 카운트 하면 0.5ms
	//TCNT3 : 0~256(0x00ff) 까지 카운트한 후 0으로 돌아감
	TCCR3B |= 1 << CS31 | 1 << CS30;
	OCR3C = 0; //OCR(사용자가 지정해준 output compare register) : PWM값
	//OCR3C = 50이라면 듀티가 몇프로인가 ->high가 몇 % 인가
	//듀티사이클 = OCR3C / TOP x 100 = 50 / 255 x 100 = 19.61%
}

/*
 PE5 : PWM control
 PF6 = IN1 방향설정
 PF7 : IN2
*/
void init_motor_driver(){
	DDRF |= 1 << 6 | 1 << 7; // 출력모드 설정
	PORTF &= ~(1 << 6 | 1 << 7); //6 7 reset
	PORTF |= 1 << 6; // 정회전
}

void dcmoter_pwm_comtrol_main(){
	
	int start_button = 0;
	int forward = 0;
	
	while(1){
		if(get_button(BUTTON0, BUTTON0PIN)){
			start_button = !start_button;
			if(start_button) OCR3C = 250;
			else OCR3C = 0;
			
		} else if(get_button(BUTTON1, BUTTON1PIN)){
			if(OCR3C >= 250) OCR3C = 250;
			else OCR3C += 20;
			
		} else if(get_button(BUTTON2, BUTTON2PIN)){
			if(OCR3C <= 70) OCR3C = 70;
			else OCR3C -= 20;
			
		} else if(get_button(BUTTON3, BUTTON3PIN)){
			forward = !forward;
			PORTF &= ~(1 << 6 | 1 << 7);
			if(forward) PORTF |= 1 << 6; // 정방향
			else PORTF |= 1 << 7; // 역방향
		}
	}
}