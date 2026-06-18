/*
 * 06.MY_WASHING_MACHINE.c
 *
 * Created: 2026-06-18 오후 3:46:40
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h> // sei 등 함수
#include <util/delay.h>

volatile uint32_t min_count = 0;
extern volatile uint32_t left_time;
extern int timer_run;

extern void init_fnd(void);
extern void init_button(void);
extern void init_led(void);
extern void init_timer3_pwm();
extern void init_washing_motor_driver();
extern void washing_comtrol_main();
void init_timer0();

ISR(TIMER0_OVF_vect){
	TCNT0 = 6;
	
	if(left_time > 0){
		min_count--;
	}
}

int main(void)
{
	init_fnd();
	init_button();
	init_led();
	init_timer0();
	init_timer3_pwm();
	init_washing_motor_driver();
	
	washing_comtrol_main();

}

void init_timer0(){
	TCNT0 = 6;
	
	TCCR0 &= ~1 << CS02 | 1 << CS01 | 1<< CS00; //0분주 초기화 설정
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //256분주
	
	TIMSK |= 1 << TOIE0; // TIMER0 OVERflow INT
	sei(); // 전역(대문) interrupt 허용
}

