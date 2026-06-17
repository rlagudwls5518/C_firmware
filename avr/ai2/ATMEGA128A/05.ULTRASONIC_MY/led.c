/*
 * 05.ULTRASONIC.c
 *
 * Created: 2026-06-17 오후 1:29:55
 * Author : kccistc
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
void led_ultrasonic_on(int distance);

extern volatile uint32_t msec_count; //volatile 최적화 방지
#define FUNC_SU 8

int func_state = 0;
void (*fp[]) () = 
{
	led_shift_left_on, // func_state = 0
	led_shift_right_on,
	led_shift_left_keepon,
	led_all_off,
	led_shift_right_keepon,
	led_all_off,
	led_flower_on,
	led_flower_off // func_state = 8
};

void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off 	
}

int led_main(void){
	led_all_off();

	
	while(1){
#if 1
		fp[func_state] ();
	}
#else // org
	uint8_t led_toggle = 0;
	while(1){
		if(msec_count >= 500){ //500ms
			msec_count = 0;
			led_toggle = !led_toggle;
			if(led_toggle) led_all_on();
			else led_all_off();
		}
	}
#endif
}

void led_all_on(void)
{
	PORTA=0xff;
}

void led_all_off(void)
{
	PORTA=0x00;
	//func_state = (func_state + 1) % FUNC_SU;
}

void led_shift_left_on(void){
	
#if 1
	static int i = 0;
	
	if(msec_count >= 100){
		msec_count = 0;
		*(unsigned char *) 0x3B = 1 << i; //PORTA : 0x1B
		i = (i + 1) % 8;
		//if((i = (i + 1) % 8) == 0) //다음 인덱스 값을 계산
			//func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 점프
	}
	
#endif
	
#if 0
	static int i = 0;
	*(unsigned char *) 0x3B = 1 << i; //PORTA : 0x1B
	_delay_ms(30); //240ms소요
	
	i = (i + 1) % 8; //다음 인덱스 값을 계산
	
#endif	
}

void led_shift_right_on(void){

#if 1
	static int i = 0;
	
	if(msec_count >= 100){
		msec_count = 0;
		*(unsigned char *) 0x3B = 0x80 >> i; //PORTA : 0x1B
		i = (i + 1) % 8;
		//if((i = (i + 1) % 8) == 0) //다음 인덱스 값을 계산
		//func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 점프
	}
	
		
#else
	for(int i = 7; i >= 0; i--){
		*(unsigned char *) 0x3B = 1 << i; //PORTA : 0x3B
		_delay_ms(30);
	}
#endif	
}

void led_shift_left_keepon(void){
	static int i = 0;
	if(msec_count >= 100){
		msec_count = 0;
		PORTA |= 1 << i;
		i = (i + 1) % 8;
		//if((i = (i + 1) % 8) == 0) //다음 인덱스 값을 계산
		//func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 점프
	}
}

void led_shift_right_keepon(void){
	static int i = 0;
	if(msec_count >= 100){
		msec_count = 0;
		PORTA |= 1 << i;
		i = (i + 1) % 8;
		//if((i = (i - 1 + 8) % 8) == 0) //다음 인덱스 값을 계산
		//func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 점프
	}
}

void led_flower_on(void){
	static int i = 0;
	if(msec_count >= 100){
		msec_count = 0;
		PORTA |= 0x10 << i; //left
		PORTA |= 0x08 >> i; //right
		i = (i + 1) % 8;
		//if((i = (i + 1) % 8) == 0) //다음 인덱스 값을 계산
		//func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 점프
	}
}

void led_flower_off(void){
	static int i = 0;
	if(msec_count >= 100){
		msec_count = 0;
		PORTA &= ~(0x80 >> i); // left
		PORTA &= ~(0x01 << i); // right
		i = (i + 1) % 8;
		//if((i = (i + 1) % 8) == 0) //다음 인덱스 값을 계산
		//func_state = (func_state + 1) % FUNC_SU; // 다음 실행할 func으로 점프
	}
}

void led_ultrasonic_on(int distance){
	if(msec_count >= 100){
		msec_count = 0;
		PORTA = 0x00;
		
		if(distance <= 2) {
			PORTA = 0x01; // 0번 켜기
		}
		else if(distance <= 3) {
			PORTA = 0x03; // 0, 1번 켜기
		}
		else if(distance <= 4) {
			PORTA = 0x07; // 0, 1, 2번 켜기
		}
		else if(distance <= 5) {
			PORTA = 0x0F; // 0, 1, 2, 3번 켜기
		}
		else if(distance <= 6) {
			PORTA = 0x1F; // 0 ~ 4번 켜기
		}
		else if(distance <= 7) {
			PORTA = 0x3F; // 0 ~ 5번 켜기
		}
		else if(distance <= 10) {
			PORTA = 0x7F; // 0 ~ 6번 켜기
		}
		else {
			led_all_on(); 
		}
	}
}