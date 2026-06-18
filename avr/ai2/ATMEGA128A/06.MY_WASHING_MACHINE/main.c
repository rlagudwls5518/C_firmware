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

ISR(TIMER0_OVF_vect){
	TCNT0 = 6; 
	min_count++; // 1ms count
}

int main(void)
{
    init_washing_motor_driver()
	washing_comtrol_main();
    while (1) 
    {
		
    }
}
void init_timer0(){
	TCNT2 = 6;
	
	TCCR2 &= ~1 << CS02 | 1 << CS01 | 1<< CS00; //0분주 초기화 설정
	TCCR2 |= 1 << CS22 | 0 << CS21 | 0 << CS20; //256분주
	
	TIMSK |= 1 << TOIE2; // TIMER2 OVERflow INT
	sei(); // 전역(대문) interrupt 허용
}

