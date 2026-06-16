/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:58:03
 *  Author: kccistc
 */ 
#include "uart0.h"

void init_uart0();
void UART0_transmit(uint8_t data);

/*
1. 전송속도 : 9600bps ( 초당 9600  -> 0.96ms -> 1바이트)
2. start/stop 설정
3. RX(수신) : interrupt 로 설절
*/
void init_uart0(){
	
	//1. 전송속도 : 9600bps
	UBRR0H = 0x00;
	UBRR0L = 207; //9600bps 표 8-9
	UCSR0A |= 1 << U2X0; // 2배속 설정(sampling 8)
	//UART0를 송수신이 다가능하고 RX INT가 가능하도록 설정한다
	UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
}

//UART0 로 1byte를 전송하는 함수
void UART0_transmit(uint8_t data){
	while(!(UCSR0A & 1 << UDRE0))
		; // no operation 데이터가 송신중이면 송신이 끝날때 까지 기다림
	UDR0 = data; //HW 전송 레지스터에 데이터를 송신한다
}
