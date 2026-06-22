/*
* fnd.c
*
* Created: 2026-06-12 오전 10:52:32
* Author: kccistc
*/
#include "fnd_my.h"

// 0  1  2  3  4  5  6  7  8  9  .
// Common Cathode (1일 때 LED ON) - ~ 연산자 제거 후 16진수 직접 입력
uint8_t fnd_font[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x80};

uint8_t big_circle_left[]  = {0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x20};
uint8_t big_circle_right[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00};


uint8_t fnd_text[4][4] = {
	// 1. "go  " : 'g', 'o', 빈칸, 빈칸
	{0x6F, 0x5C, 0x00, 0x00},

	// 2. "bACk" : 'b', 'A', 'C', 'k'
	{0x7C, 0x77, 0x39, 0x75},

	// 3. "LEFt" : 'L', 'E', 'F', 't'
	{0x38, 0x79, 0x71, 0x78},

	// 4. "righ" : 'r', 'i', 'g', 'h'
	{0x50, 0x04, 0x6F, 0x74}
};


extern volatile uint32_t msec_count;


static int row = 0;

void init_fnd();
int fnd_main(int time_mode);
void fnd_state_display(int state);
void fnd_racing_time_display();

void init_fnd(){
	FIRST_FND_DATA_DDR = 0xff; //출력모드로 설정
	SECOND_FND_DATA_DDR = 0xff;
	
	FIRST_FND_DIGHT_DDR |= (1 <<  FIRST_FND_DIGHT_D1) | (1 <<  FIRST_FND_DIGHT_D2) | (1 <<  FIRST_FND_DIGHT_D3) | (1 <<  FIRST_FND_DIGHT_D4);
	SECOND_FND_DIGHT_DDR |= (1 << SECOND_FND_DIGHT_D1) | (1 << SECOND_FND_DIGHT_D2) | (1 << SECOND_FND_DIGHT_D3) | (1 << SECOND_FND_DIGHT_D4);
	
	// FND를 전체 off하는 작업 (캐소드는 0x00을 주어야 꺼짐)
	FIRST_FND_DATA_PORT = 0x00;
	SECOND_FND_DATA_PORT = 0x00;
}

void fnd_state_display(int state) { // 주행 상태 fnd
	if(state == 0) {
		row = 2;
		} else if(state == 1) {
		row = 3;
		} else if(state == 2) {
		row = 1;
		} else {
		row = 0;
	}
	
	static int digit_select = 0; // 자리수 선택
	
	// 애니메이션 속도를 100ms 단위로 늦추고, 두 자리의 프레임이 다르게 보이도록 오프셋(4) 적용
	int ani_frame1 = (msec_count / 100) % 8;
	int ani_frame2 = ((msec_count / 100) + 4) % 8;

	switch(digit_select) {
		case 0: // 가장 오른쪽 글자 - PA3을 LOW로 (1111 0111)
		SECOND_FND_DIGHT_PORT = 0xF7;
		SECOND_FND_DATA_PORT = fnd_text[row][3];
		break;
		
		case 1: // 오른쪽에서 2번째 글자 - PA4를 LOW로 (1110 1111)
		SECOND_FND_DIGHT_PORT = 0xEF;
		SECOND_FND_DATA_PORT = fnd_text[row][2];
		break;
		
		case 2: // 오른쪽에서 3번째 글자 - PA5를 LOW로 (1101 1111)
		SECOND_FND_DIGHT_PORT = 0xDF;
		if (row == 0) SECOND_FND_DATA_PORT = fnd_text[row][1];
		else SECOND_FND_DATA_PORT = big_circle_right[ani_frame1];
		break;
		
		case 3: // 가장 왼쪽 글자 - PA6을 LOW로 (1011 1111)
		SECOND_FND_DIGHT_PORT = 0xBF;
		if (row == 0) SECOND_FND_DATA_PORT = fnd_text[row][0];
		else SECOND_FND_DATA_PORT = big_circle_right[ani_frame2];
		break;
	}

	digit_select = (digit_select + 1) % 4;
}

// 주행시간 fnd (00.00초 형태 출력)
void fnd_racing_time_display() {
	static int digit_select = 0; // 자리수 선택
	
	switch(digit_select) {
		case 0: // 0.01초 (10ms) 단위 (가장 오른쪽) - PB0
		FIRST_FND_DIGHT_PORT = 0xFE;
		FIRST_FND_DATA_PORT = fnd_font[(msec_count / 10) % 10];
		break;
		
		case 1: // 0.1초 (100ms) 단위 - PB1
		FIRST_FND_DIGHT_PORT = 0xF7;
		FIRST_FND_DATA_PORT = fnd_font[(msec_count / 100) % 10];
		break;
		
		case 2: // 1초 단위 (소수점 포함) - PB2
		FIRST_FND_DIGHT_PORT = 0xFB;
		FIRST_FND_DATA_PORT = fnd_font[(msec_count / 1000) % 10] | 0x80; // 점 켜기
		break;
		
		case 3: // 10초 단위 (가장 왼쪽) - PB3
		FIRST_FND_DIGHT_PORT = 0xFD;
		FIRST_FND_DATA_PORT = fnd_font[(msec_count / 10000) % 10];
		break;
	}


	digit_select = (digit_select + 1) % 4;
}