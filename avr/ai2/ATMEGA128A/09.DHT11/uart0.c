/*
* uart0.c
*
* Created: 2026-06-16 오전 9:58:03
*  Author: kccistc
*/
#include "uart0.h"

extern int func_state;
extern void (*fp[]) ();

void init_uart0();
void UART0_transmit(uint8_t data);
void pc_command_processing();

//pc로부터 1바이트가 들어오면 자동적으로 이곳으로 진입
//예) led_all_on/n 아면 11번 이곳으로 들어옴
ISR(USART0_RX_vect){
	//문장상으로는 복사이지마 하드웨어에서는 그냥 없어짐
	volatile uint8_t data;
	volatile static int i = 0;
	data = UDR0; // UDR0의 내용이 data에 복사된 후 UDR0의 내용이 빈 상태로 된다
	if((rear + 1) % QUEUE_SIZE == front % QUEUE_SIZE) return; // 큐 다찬 상태 확인
	
	if(data == '\n' || data == '\r'){
		rx_buff[rear][i] = '\0'; // 문장의 끝은 NULL을 넣는다
		i = 0; // 다음 string을 저장하기 위해서
		rear = (rear + 1) % QUEUE_SIZE; //  0 ~ 9
		
	}else{
		rx_buff[rear][i++] = data;
	}
}
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
	while(!(UCSR0A & 1 << UDRE0)); // no operation 데이터가 송신중이면 송신이 끝날때 까지 기다림
	UDR0 = data; //HW 전송 레지스터에 데이터를 송신한다
}

void pc_command_processing(){
	if(front != rear){ // data가 rx_buff에 존재하는지 체크
		printf("%s", rx_buff[front]); // printf("%s", &rx_buff[front][0]) 같은 의미
		if(strncmp((char *)rx_buff[front], "led_shift_left_on", strlen("led_shift_left_on")) == 0){
			func_state = 0;
		}
		else if(strncmp((char *)rx_buff[front], "led_shift_right_on", strlen("led_shift_right_on")) == 0){
			func_state = 1;
		}
		else if(strncmp((char *)rx_buff[front], "led_shift_left_keepon", strlen("led_shift_left_keepon")) == 0){
			func_state = 2;
		}
		else if(strncmp((char *)rx_buff[front], "led_all_off", strlen("led_all_off")) == 0){
			func_state = 3;
		}
		else if(strncmp((char *)rx_buff[front], "led_shift_right_keepon", strlen("led_shift_right_keepon")) == 0){
			func_state = 4;
		}
		else if(strncmp((char *)rx_buff[front], "led_all_off", strlen("led_all_off")) == 0){
			func_state = 5;
		}	
		else if(strncmp((char *)rx_buff[front], "led_flower_on", strlen("led_flower_on")) == 0){
			func_state = 6;
		}
		else if(strncmp((char *)rx_buff[front], "led_flower_off", strlen("led_flower_off")) == 0){
			func_state = 7;
		}											
		front = (front + 1) % QUEUE_SIZE;
	}
	fp[func_state] ();
}
