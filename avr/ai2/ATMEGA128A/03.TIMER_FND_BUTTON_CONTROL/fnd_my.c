/*
* fnd.c
*
* Created: 2026-06-12 오전 10:52:32
*  Author: kccistc
*/
#include "fnd_my.h"

//분 초 시계만들기


uint32_t sec_10 = 0; // 초의 10의 자리 (0~5)
uint32_t sec_1  = 0; // 초의 1의 자리 (0~9)
uint32_t ms_10  = 0;  // 밀리초의 10의 자리 (0~9)
uint32_t ms_1   = 0;  // 밀리초의 1의 자리 (0~9)

extern volatile uint32_t ms_count; //volatile 최적화 방지
extern volatile uint32_t stopwatch_ms_count;
extern volatile uint32_t sec_count;
extern volatile uint32_t stopwatch_run;

#if 1					//   0     1     2    3      4    5      6     7     8     9     .
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; //common 애노드
#else					//   0     1     2    3      4    5      6     7     8     9     .
	uint8_t fnd_font[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67, 0x80}; //common 캐소드
#endif

uint8_t big_circle_left[]  = {~0x01, ~0x00, ~0x00, ~0x00, ~0x00, ~0x08, ~0x10, ~0x20};
uint8_t big_circle_right[] = {~0x00, ~0x01, ~0x02, ~0x04, ~0x08, ~0x00, ~0x00, ~0x00};
	
void init_fnd();
void fnd_min_sec_display();
void fnd_sec_display();
void fnd_stopwatch();
int fnd_main(int time_mode);
void reset_stopwatch(int stopwatch_reset_mode);
void time_stop();

int fnd_main(int time_mode){
	
	switch(time_mode){
		case 0:
			fnd_min_sec_display();
			break;
		case 1:
			fnd_sec_display();
			break;
		case 2:
			fnd_stopwatch();
			break;
	}
	
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

void fnd_min_sec_display(){
	
	static int digit_select = 0; // 자리수 선택
	
	switch(digit_select){
		case 0: //1단위
		#if 1
		FND_DIGHT_PORT = 0x80; //애노드
		#else
		FND_DIGHT_PORT = ~0x80; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[sec_count % 10]; //0~9까지 출력
		break;
		
		case 1: // 10단위
		#if 1
		FND_DIGHT_PORT = 0x40; //애노드
		#else
		FND_DIGHT_PORT = ~0x40; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[sec_count/10 % 6]; //0~59까지 출력
		break;
		
		case 2: // 100단위
		#if 1
		FND_DIGHT_PORT = 0x20; //애노드
		#else
		FND_DIGHT_PORT = ~0x20; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[sec_count/60 % 10]; 
		break;
		
		case 3: // 1000단위
		#if 1
		FND_DIGHT_PORT = 0x10; //애노드
		#else
		FND_DIGHT_PORT = ~0x10; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[sec_count/600 % 6]; 
		break;
	}
	
	digit_select = (digit_select + 1) % 4; //다음 표시할 자리 수
}

void fnd_sec_display(){
	
	static int digit_select = 0; // 자리수 선택

	switch(digit_select){
		case 0: //1단위
		#if 1
		FND_DIGHT_PORT = 0x80; //애노드
		#else
		FND_DIGHT_PORT = ~0x80; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[sec_count % 10]; //0~9까지 출력
		break;
		
		case 1: // 10단위
		#if 1		
		FND_DIGHT_PORT = 0x40; //애노드
		#else
		FND_DIGHT_PORT = ~0x40; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[sec_count/10 % 6]; //0~59까지 출력
		break;
		
		case 2: // 100단위
		#if 1
		FND_DIGHT_PORT = 0x20; //애노드
		#else
		FND_DIGHT_PORT = ~0x20; //캐소드
		#endif
		FND_DATA_PORT = big_circle_right[sec_count % 8];
		break;
		
		case 3: // 1000단위
		#if 1
		FND_DIGHT_PORT = 0x10; //애노드
		#else
		FND_DIGHT_PORT = ~0x10; //캐소드
		#endif
		FND_DATA_PORT = big_circle_left[sec_count % 8];
		break;
	}
	
	digit_select = (digit_select + 1) % 4;
	
}

void fnd_stopwatch(){
	
	//10ms 100ms 1000ms 10000ms 속도로 변화
	sec_10 = (stopwatch_ms_count / 10000) % 6;
	sec_1  = (stopwatch_ms_count / 1000) % 10;
	ms_10  = (stopwatch_ms_count / 100) % 10;
	ms_1   = (stopwatch_ms_count / 10) % 10;
	
	static int digit_select = 0; // 자리수 선택
	
	switch(digit_select){
		case 0: //1단위
		#if 1
		FND_DIGHT_PORT = 0x80; //애노드
		#else
		FND_DIGHT_PORT = ~0x80; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[ms_1]; //0~9까지 출력
		break;
		
		case 1: // 10단위
		#if 1
		FND_DIGHT_PORT = 0x40; //애노드
		#else
		FND_DIGHT_PORT = ~0x40; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[ms_10]; //0~59까지 출력
		break;
		
		case 2: // 100단위
		#if 1
		FND_DIGHT_PORT = 0x20; //애노드
		#else
		FND_DIGHT_PORT = ~0x20; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[sec_1];
		break;
		
		case 3: // 1000단위
		#if 1
		FND_DIGHT_PORT = 0x10; //애노드
		#else
		FND_DIGHT_PORT = ~0x10; //캐소드
		#endif
		FND_DATA_PORT = fnd_font[sec_10];
		break;
	}
	
	digit_select = (digit_select + 1) % 4; //다음 표시할 자리 수
}

void reset_stopwatch(int stopwatch_reset_mode){
	if(stopwatch_reset_mode == 0){
		stopwatch_ms_count = 0;	
		stopwatch_run = 0;
	}
	else{
		stopwatch_run = 1;
	}
}

void time_stop(){
	stopwatch_run = !stopwatch_run;
}

