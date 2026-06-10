/*
 * 01.LED_CONTROL.c
 *
 * Created: 2026-06-10 오전 10:20:29
 * Author : kccistc
 */ 
#define F_CPU 16000000UL // 16MHz
#include <avr/io.h> // PORTA PORTD 등의 I/O register들이 들어있다.
#include <util/delay.h> // _delay_ms _delay_us 등의 함수가 들어있다.

int main(void)
{
    
    DDRA = 0b11111111; // PORTA에 LED가 8개 연결되어있다
	//DDR(DAta Direction Register) 방향설정
	// 1: 출력 0: 입력
	while (1) {
		PORTA = 0b11111111; // all on
		_delay_ms(1000); // 1초 유지 1000ms : 1초 500ms : 0.5초
		PORTA = 0b00000000; // all off
		_delay_ms(1000); // 1초 sleep
    }
}

