/*
* fnd.c
*
* Created: 2026-06-12 오전 10:52:32
* Author: kccistc
*/
#include "fnd_my.h"
#define TIME 1
#define COUNT 0

uint8_t fnd_font[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6D, 0x7d, 0x27, 0x7f, 0x67, 0x80};

uint8_t big_circle_left[]  = {0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x20};
uint8_t big_circle_right[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00};


uint8_t fnd_text[4][4] = {
	// 1. "F -  " : 'F', '-', 빈칸, 빈칸 (WARNING_MID 또는 기본)
	{0x71, 0x40, 0x00, 0x00},

	// 2. "b -  " : 'b', '-', 빈칸, 빈칸 (WARNING_RIGHT)
	{0x7C, 0x40, 0x00, 0x00},

	// 3. "L -  " : 'L', '-', 빈칸, 빈칸 (WARNING_LEFT)
	{0x38, 0x40, 0x00, 0x00},

	// 4. "r -  " : 'r', '-', 빈칸, 빈칸 (TIME / 타이머 표시 등)
	{0x50, 0x40, 0x00, 0x00}
};


extern volatile uint32_t msec_count;
extern volatile uint32_t sec_count;
extern volatile uint32_t m_count;
extern int back_count;
extern int left_count;
extern int right_count;


static int row = 0;

void init_fnd();
int fnd_main(int time_mode);
void fnd_state_display(int state);
void fnd_racing_time_display();
void final_count_display();

void init_fnd(){
	FIRST_FND_DATA_DDR = 0xff; //출력모드로 설정
	SECOND_FND_DATA_DDR = 0xff;
	
	FIRST_FND_DIGHT_DDR |= (1 <<  FIRST_FND_DIGHT_D1) | (1 <<  FIRST_FND_DIGHT_D2) | (1 <<  FIRST_FND_DIGHT_D3) | (1 <<  FIRST_FND_DIGHT_D4);
	SECOND_FND_DIGHT_DDR |= (1 << SECOND_FND_DIGHT_D1) | (1 << SECOND_FND_DIGHT_D2) | (1 << SECOND_FND_DIGHT_D3) | (1 << SECOND_FND_DIGHT_D4);
	
	// FND를 전체 off하는 작업 (캐소드는 0x00을 주어야 꺼짐)
	FIRST_FND_DATA_PORT = 0x00;
	SECOND_FND_DATA_PORT = 0x00;
}

void fnd_state_display(int state) {
	
	
	if(state == 0) row = 2;// left
	else if(state == 1) row = 3; // right
	else if(state == 2) row = 1; // back
	else row = 0; // for
	
	static int digit_select = 0;
	
	int ani_frame1 = (msec_count / 100) % 8;
	int ani_frame2 = ((msec_count / 100) + 4) % 8;

	SECOND_FND_DATA_PORT = 0x00;

	switch(digit_select) {
		case 0:
		
		SECOND_FND_DIGHT_PORT = 0xFE; 
		if (row == 0) SECOND_FND_DATA_PORT = fnd_text[row][0];
		else SECOND_FND_DATA_PORT = big_circle_left[ani_frame1];
		break;
		
		case 1:
		SECOND_FND_DIGHT_PORT = 0xFD; 
		if (row == 0) SECOND_FND_DATA_PORT = fnd_text[row][1];
		else SECOND_FND_DATA_PORT = big_circle_right[ani_frame2];
		break;
		
		case 2:
		SECOND_FND_DIGHT_PORT = 0xF7; 
		SECOND_FND_DATA_PORT = fnd_text[row][2];
		
		break;
		
		case 3:
		SECOND_FND_DIGHT_PORT =  0xFB; 
		SECOND_FND_DATA_PORT = fnd_text[row][3];
		break;
	}
	_delay_ms(1);

	digit_select = (digit_select + 1) % 4;
}

// 주행시간 fnd 
void fnd_racing_time_display() {
	static int digit_select = 0; // 자리수 선택
	
	FIRST_FND_DATA_PORT = 0x00;
	switch(digit_select) {
		case 0: // 10분단위
		FIRST_FND_DIGHT_PORT = 0xFE;
		FIRST_FND_DATA_PORT = fnd_font[m_count/10];
		break;
		
		case 1: // 1분단위
		FIRST_FND_DIGHT_PORT = 0xFD;
		FIRST_FND_DATA_PORT = fnd_font[m_count % 10] | 0x80;
		break;
		
		case 2: // 10초 단위 (소수점 포함) - PB2
		FIRST_FND_DIGHT_PORT = 0xF7;
		FIRST_FND_DATA_PORT = fnd_font[sec_count / 10];
		break;
		
		case 3: // 1초 단위 (가장 오른쪽) - PB3
		FIRST_FND_DIGHT_PORT = 0xFB;
		FIRST_FND_DATA_PORT = fnd_font[sec_count % 10];
		break;
	}
	_delay_ms(1);
	digit_select = (digit_select + 1) % 4;
}

void final_count_display(int count, int display_mode) {
	static int digit_select = 0;
	
	if( display_mode == 0) row = 2;
	else if( display_mode == 1) row = 3;
	else if( display_mode == 2) row = 1;
	else row = 0;
	
	int ten_digit = (count / 10) % 10;
	int one_digit = count % 10;

	SECOND_FND_DATA_PORT = 0x00;
	
	switch(digit_select) {
		case 0:
		SECOND_FND_DIGHT_PORT = ~(1 << SECOND_FND_DIGHT_D1); // [1번째 자리] 문자 표시 ('r' 등)
		SECOND_FND_DATA_PORT = fnd_text[row][0];
		break;
		
		case 1:
		SECOND_FND_DIGHT_PORT = ~(1 << SECOND_FND_DIGHT_D2); // [2번째 자리] 대시 기호 표시
		SECOND_FND_DATA_PORT = fnd_text[row][1];  // '-' 모양 (G 세그먼트)
		break;
		
		case 2:
		SECOND_FND_DIGHT_PORT = ~(1 << SECOND_FND_DIGHT_D3); // [3번째 자리] 기존 코드 순서 유지
		SECOND_FND_DATA_PORT = fnd_font[ten_digit]; // 10의 자리 숫자 (0)
		break;
		
		case 3:
		SECOND_FND_DIGHT_PORT = ~(1 << SECOND_FND_DIGHT_D4); // [4번째 자리] 기존 코드 순서 유지
		SECOND_FND_DATA_PORT = fnd_font[one_digit]; // 1의 자리 숫자 (4)
		break;
	}

	_delay_ms(1); // 잔상을 위한 1ms 딜레이 추가
	digit_select = (digit_select + 1) % 4;
}