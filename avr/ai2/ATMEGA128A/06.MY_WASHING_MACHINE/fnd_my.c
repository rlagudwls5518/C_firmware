/*
* fnd.c
*
* Created: 2026-06-12 오전 10:52:32
*  Author: kccistc
*/
#include "fnd_my.h"


					//   0     1     2    3      4    5      6     7     8     9     .
uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; //common 애노드


uint8_t big_circle_left[]  = {~0x01, ~0x00, ~0x00, ~0x00, ~0x00, ~0x08, ~0x10, ~0x20};
uint8_t big_circle_right[] = {~0x00, ~0x01, ~0x02, ~0x04, ~0x08, ~0x00, ~0x00, ~0x00};
	
extern volatile uint32_t left_time;

void init_fnd();
int fnd_main(int time_mode);
void fnd_washing_time_display();
void fnd_washing_time_set_display(int set_time);

int fnd_main(int time_mode){
	
	return 0;
}


void init_fnd(){
	FND_DATA_DDR = 0xff; //출력모드로 설정
	FND_DIGHT_DDR |= 1 << FND_DIGHT_D1 | 1 << FND_DIGHT_D2 | 1 << FND_DIGHT_D3 | 1 << FND_DIGHT_D4;
	
	//FND를 전체 off하는 작업
	FND_DATA_PORT = 0xff;

}

//설정모드 FND화면 
void fnd_washing_time_set_display(int set_time){
	static int digit_select = 0; // 자리수 선택

	switch(digit_select){
		case 0: //1단위
		FND_DIGHT_PORT = 0x80; 
		FND_DATA_PORT = fnd_font[(set_time / 6) % 10]; 
		break;
		
		case 1: // 10단위
		FND_DIGHT_PORT = 0x40; 
		FND_DATA_PORT = fnd_font[set_time / 6]; 
		break;
		
		case 2: // 100단위
		FND_DIGHT_PORT = 0x20;
		FND_DATA_PORT =  0xff;
		break;
		
		case 3: // 1000단위
		FND_DIGHT_PORT = 0x10;
		FND_DATA_PORT =  0xff;
	}
	_delay_ms(1);
	digit_select = (digit_select + 1) % 4;
}

//동작모드 FND화면
void fnd_washing_time_display(){
	
	static int digit_select = 0; // 자리수 선택

	switch(digit_select){
		case 0: //1단위
		FND_DIGHT_PORT = 0x80; 
		if(left_time >= 60) FND_DATA_PORT = fnd_font[(left_time / 60) % 10]; 
		else FND_DATA_PORT = fnd_font[left_time % 10]; 
		break;
		
		case 1: // 10단위
		FND_DIGHT_PORT = 0x40; 
		if(left_time >= 60) FND_DATA_PORT = fnd_font[(left_time / 60) / 10]; 
		else FND_DATA_PORT = fnd_font[left_time / 10]; 
		break;
		
		case 2: // 100단위
		FND_DIGHT_PORT = 0x20;
		FND_DATA_PORT = big_circle_right[left_time % 8];
		break;
		
		case 3: // 1000단위
		FND_DIGHT_PORT = 0x10;
		FND_DATA_PORT = big_circle_left[left_time % 8];
		break;
	}
	_delay_ms(1);
	digit_select = (digit_select + 1) % 4;
	
}

