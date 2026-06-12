/*
* fnd.c
*
* Created: 2026-06-12 오전 10:52:32
*  Author: kccistc
*/
#include "fnd.h"

//분 초 시계만들기
uint32_t ms_count = 0;
uint32_t sec_count = 0;
uint8_t dot_display = 0;
void init_fnd();
void fnd_display();
int fnd_main();

int fnd_main(){
	
	init_fnd();

	while(1){
		fnd_display();
		_delay_ms(1);
		ms_count++;
		
		if(ms_count >= 1000){
			ms_count = 0;
			sec_count++;
			dot_display = !dot_display;
		}
	}
	
	return 0;
}


void init_fnd(){
	FND_DATA_DDR = 0xff; //출력모드로 설정
	FND_DIGHT_DDR |= 1 << FND_DIGHT_D1 | 1 << FND_DIGHT_D2 | 1 << FND_DIGHT_D3 | 1 << FND_DIGHT_D4;
	
	//FND를 전체 off 하는 작업
#if 1 // commom 애노우드
	FND_DATA_PORT = 0xff;

#else // commom 캐소우드
	FND_DATA_PORT = ~0xff;

#endif
}
void fnd_display(){
#if 1
						//   0     1     2    3      4    5      6     7     8     9     .
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; //common 애노드
#else
						//   0     1     2    3      4    5      6     7     8     9     .
	uint8_t fnd_font[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xd8, 0x80, 0x98, 0x7f}; //common 캐소드
#endif
	
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