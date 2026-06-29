/*
 * 11.KEYPAD_MY.c
 *
 * Created: 2026-06-29 오후 4:08:37
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "queue.h"

extern uint8_t keypad_scan();
extern void init_keypad();
extern void init_uart0();
extern void UART0_transmit(uint8_t data);
extern void calculator();
extern void print_calculator(int key);
void init_timer0();
volatile uint32_t keypad_count = 0;

FILE OUTPUT = FDEV_SETUP_STREAM((int(*)(char, FILE*))UART0_transmit, NULL, _FDEV_SETUP_WRITE);//화면으로 출력되도록 매핑



ISR(TIMER0_OVF_vect){
	volatile uint8_t key_data = 0;
	
	TCNT0 = 6;
	if(++keypad_count >= 60){ // 60ms count하기위해
		keypad_count = 0;
		key_data = keypad_scan();
		if (key_data !=0){ // keypad를 체크해서 눌러진 것이 있으면
			insert_queue(key_data);		// circular queue에 저장한다
		}
	}
}

int main(void)
{
	uint8_t key_value = 0;
	init_uart0();
	init_timer0();
	init_keypad();
	
	sei();
	stdout = &OUTPUT;
	
	while (1) {
		if(!queue_empty()){
			key_value = read_queue();
			print_calculator(key_value);
			calculator(key_value);
			
		}
	}
}

void init_timer0(){
	TCNT0 = 6; //TCNT0 6~256 : 250게 펄스 count하기 위해
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); //0분주 초기화 설정
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //64분주
	TIMSK |= 1 << TOIE0; // TIMER0 OVERflow INT
}

