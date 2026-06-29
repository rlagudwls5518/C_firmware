/*
 * 10.DS1302_MY.c
 *
 * Created: 2026-06-29 오전 9:51:54
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h> // sei 등 함수
#include <util/delay.h>

extern void init_uart0();
extern void UART0_transmit(uint8_t data);
extern void ds1302_main();

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);//화면으로 출력되도록 매핑



int main(void)
{
	
	init_uart0();
	
	stdout = &OUTPUT; // printf가 동작 할 수 있도록 stdout을 설정
	sei();
	while (1) {
		ds1302_main();
		_delay_ms(1000);//안정화 시간
	}

	
}

