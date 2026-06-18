/*
* fnd.c
*
* Created: 2026-06-12 오전 10:52:32
*  Author: kccistc
*/
#include "fnd_my.h"

//분 초 시계만들기
uint32_t stopwatch_ms_count = 0;
uint32_t ms_count = 0;
uint32_t sec_count = 0;
uint8_t dot_display = 0;
uint32_t sec_10 = 0; // 초의 10의 자리 (0~5)
uint32_t sec_1  = 0; // 초의 1의 자리 (0~9)
uint32_t ms_10  = 0;  // 밀리초의 10의 자리 (0~9)
uint32_t ms_1   = 0;  // 밀리초의 1의 자리 (0~9)

#if 1					//   0     1     2    3      4    5      6     7     8     9     .
uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; //common 애노드
#else					//   0     1     2    3      4    5      6     7     8     9     .
uint8_t fnd_font[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67, 0x80}; //common 캐소드
#endif

uint8_t big_circle_left[]  = {~0x01, ~0x00, ~0x00, ~0x00, ~0x00, ~0x08, ~0x10, ~0x20};
uint8_t big_circle_right[] = {~0x00, ~0x01, ~0x02, ~0x04, ~0x08, ~0x00, ~0x00, ~0x00};

int stopwatch_run = 1;

void init_fnd();
void fnd_min_sec_display();
void fnd_sec_display();
void fnd_stopwatch();
int fnd_main(int time_mode);
void reset_stopwatch(int stopwatch_reset_mode);
void time_stop();
void fnd_washing_time_display(int left_time);
void fnd_washing_time_set_display();

int fnd_main(int time_mode){
	
	return 0;
}


void init_fnd(){
	FND_DATA_DDR = 0xff; //출력모드로 설정
	FND_DIGHT_DDR |= 1 << FND_DIGHT_D1 | 1 << FND_DIGHT_D2 | 1 << FND_DIGHT_D3 | 1 << FND_DIGHT_D4;
	
	//FND를 전체 off하는 작업
	#if 1 // commom 애노우드
	FND_DATA_PORT = 0xff;

	#else // commom 캐소우드
	FND_DATA_PORT = ~0xff;

	#endif
}

void fnd_washing_time_set_display(){
	static int digit_select = 0; // 자리수 선택

	switch(digit_select){
		case 0: //1단위
		FND_DIGHT_PORT = 0x80; //애노드
		FND_DATA_PORT = fnd_font[0++]; //0~9까지 출력
		break;
		
		case 1: // 10단위
		FND_DIGHT_PORT = 0x40; //애노드
		FND_DATA_PORT = fnd_font[left_time/10 % 6]; //0~59까지 출력
		break;
		
		case 2: // 100단위
		FND_DIGHT_PORT = 0x20;
		FND_DATA_PORT = fnd_font[0];
		break;
		
		case 3: // 1000단위
		FND_DIGHT_PORT = 0x20;
		FND_DATA_PORT = fnd_font[0];
	}
	
	digit_select = (digit_select + 1) % 4;
}
void fnd_washing_time_display(int left_time){
	
	static int digit_select = 0; // 자리수 선택

	switch(digit_select){
		case 0: //1단위
		FND_DIGHT_PORT = 0x80; //애노드
		FND_DATA_PORT = fnd_font[left_time % 10]; //0~9까지 출력
		break;
		
		case 1: // 10단위
		FND_DIGHT_PORT = 0x40; //애노드
		FND_DATA_PORT = fnd_font[left_time/10 % 6]; //0~59까지 출력
		break;
		
		case 2: // 100단위
		FND_DIGHT_PORT = 0x20;
		FND_DATA_PORT = big_circle_right[left_time % 8];
		break;
		
		case 3: // 1000단위
		FND_DIGHT_PORT = 0x20;
		FND_DATA_PORT = big_circle_left[left_time % 8];
		break;
	}
	
	digit_select = (digit_select + 1) % 4;
	
}

