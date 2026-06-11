/*
 * 01.LED_CONTROL.c
 *
 * Created: 2026-06-10 오전 10:20:29
 * Author : kccistc
 */ 
#define F_CPU 16000000UL // 16MHz
#include <avr/io.h> // PORTA PORTD 등의 I/O register들이 들어있다.
#include <util/delay.h> // _delay_ms _delay_us 등의 함수가 들어있다.
#include "button.h"
extern void init_led(void); // 다른파일에 들어있을때 extern 사용
extern void init_button(void);
extern int get_button(int button_num, int button_pin);
extern void led_all_on(void);
extern void led_all_off(void);
extern void led_first_on(void);
extern void led_second_on(void);
extern void led_odd_on(void);
extern void led_even_on(void);

#if 1
int main(void){
	int button0_state = 0; // 초기상태를 led all off로 출발하자
	//int button2_state = 0;
    init_button();
	init_led();
	
	while (1) {
		// toggle off <--> on
		if(get_button(BUTTON0, BUTTON0PIN)){//노이즈 체크 
			button0_state++;
		}
		
		switch (button0_state){
			case 1:
				led_odd_on();
				break;
			case 2:
				led_even_on();
				break;
			case 3:
				led_all_on();
				break;
			default:
				led_all_off();
				button0_state = 0;
				break;
		}
		
		if(get_button(BUTTON2, BUTTON2PIN)){//노이즈 체크
			led_all_off();
			button0_state = 0;
		}
    }
}
#endif	

#if 0
int main(void)
{
	
	DDRA = 0b11111111; // PORTA에 LED가 8개 연결되어있다
	//DDR(Data Direction Register) 방향설정
	// 1: 출력 0: 입력
	while (1) {
		PORTA = 0b11111111; // all on
		_delay_ms(1000); // 1초 유지 1000ms : 1초 500ms : 0.5초
		PORTA = 0b00000000; // all off
		_delay_ms(1000); // 1초 sleep
	}
}
#endif

