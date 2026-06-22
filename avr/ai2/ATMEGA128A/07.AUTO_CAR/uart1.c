/*
 * uart1.c
 *
 * Created: 2026-06-22 오전 10:38:35
 *  Author: kccistc
 */ 

#include "uart1.h"

volatile uint8_t bt_data;

void init_uart1();
void UART1_transmit(uint8_t bt_data);

//pc로부터 1바이트가 들어오면 자동적으로 이곳으로 진입
//예) led_all_on/n 아면 11번 이곳으로 들어옴
ISR(USART1_RX_vect){

	bt_data = UDR1;
	UART1_transmit(bt_data);// bt로 부터 들어온 byte를 확인하기 위해서 comport로 출력
	
}
/*
1. 전송속도 : 9600bps ( 초당 9600  -> 0.96ms -> 1바이트)
2. start/stop 설정
3. RX(수신) : interrupt 로 설절
*/
void init_uart1(){
	
	//1. 전송속도 : 9600bps
	UBRR1H = 0x00;
	UBRR1L = 207; //9600bps 표 8-9
	UCSR1A |= 1 << U2X1; // 2배속 설정(sampling 8)
	//UART0를 송수신이 다가능하고 RX INT가 가능하도록 설정한다
	UCSR1B |= 1 << RXEN1 | 1 << TXEN1 | 1 << RXCIE1;
}

//UART0 로 1byte를 전송하는 함수
void UART1_transmit(uint8_t bt_data){
	while(!(UCSR1A & 1 << UDRE1)); // no operation 데이터가 송신중이면 송신이 끝날때 까지 기다림
	UDR1 = bt_data; //HW 전송 레지스터에 데이터를 송신한다
}
