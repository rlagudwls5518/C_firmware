/*
 * keypad.c
 *
 * Created: 2026-06-29 오후 2:11:27
 *  Author: kccistc
 */ 
#include "keypad.h"

uint8_t get_button(int row, int col);
void init_keypad();

void init_keypad(){
	KEYPAD_DDR = 0x0f; // row : 입력 col : 출력
	KEYPAD_PORT = 0xff;
}

uint8_t keypad_scan(){
	uint8_t data = 0; // key값 저장변수
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			data = get_button(row, col);
			if(data) return data;
		}
	}
	return 0;
}

uint8_t get_button(int row, int col){
	uint8_t keypad_char[4][4] = {
		{' ', '0', '=', '+'}, 
		{'4', '5', '6', '*'},
		{'7', '8', '9', '-'},
		{'1', '2', '3', '/'},
	 };
	 
	 //버튼 눌렀을때 low로 동작하도록
	 static uint8_t prev_state[4][4] = {
		 {1, 1, 1, 1}, //초기버튼은 눌려지지않은 상태로 한다. low상태
		 {1, 1, 1, 1},
		 {1, 1, 1, 1},
		 {1, 1, 1, 1},	 	 
	 };
	 
	 uint8_t current_state = 1; 
	 KEYPAD_PORT = 0xff;
	 KEYPAD_PORT &= ~(1 << 3 - col); // 해당 col에 전류를 흘린다.
	 //col 4가 PA4에 연결되어있기때문에 
	 
	 
	 for(int delay = 0; delay < 20; delay++); // keypad check를 위한 delay
										  // 0.0625us x 20 = 1.25us
		
		 
	current_state = (KEYPAD_PIN & (1 << (row + 4))) >> (row + 4);
		 // 예) row 0: PA4에 연결
		 //7 6 5 4 3 2 1 0
		 //1 1 1 0 1 1 1 1 KEYPAD_PIN & (1 << (row + 4))
		 //0 0 0 0 1 1 1 0  >= (row + 4) 
	if(current_state == 0 && prev_state[row][col] == 1){ // 처음 눌려진 상태
		prev_state[row][col] = 0;
		return 0;
	}else if(current_state == 1 && prev_state[row][col] == 0){
		//이전에 버튼이 눌러지고 현재는 버튼을 뗀상태이며 버튼을 1번 눌렀다 뗀것으로 인정
		prev_state[row][col] = 1; // prev_state를 초기화
		printf("key : %c\n", keypad_char[row][col]);
		return keypad_char[row][col];
	}
	return 0;
	//delay가 없는 이유? 60미리마다 스캔하는데 60ms면 이미 노이즈가 지나간다>?
}
