/*
 * button.c
 *
 * Created: 2026-06-10 오후 1:25:21
 *  Author: kccistc
 */ 
#include "button.h"

void init_button(void); //버튼 초기화 방향설정(입력으로)
int get_button(int button_num, int button_pin);


void init_button(void){
	BUTTON_DDR &= ~(1 << BUTTON0PIN | 1 << BUTTON1PIN | 1 << BUTTON2PIN | 1 << BUTTON3PIN);
	// BUTTON_DDR &= 0xf0;
}

// 예) BUTTON0 3
// 버튼을 눌렀다 떼면 : 1을 리턴
// 버튼이 idle 상태 : 0을 리턴
int get_button(int button_num, int button_pin){
	 static unsigned char button_status[BUTTON_NUMBER] = 
	{
		BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE
	};
	// static 지역변수에 static을 선언하면 함수를 빠져나와 다시 들어가도 이전 값을 유지
	int current_state;
	
	//1. 버튼을 읽는다
	current_state = BUTTON_PIN & (1 << button_pin);
	
	//2. 버튼상태 체크
	if(current_state && button_status[button_num] == BUTTON_RELEASE){ // 버튼이 처음 눌려진 상태
		_delay_ms(15); //노이즈가 지나가길 기다린다
		button_status[button_num] = BUTTON_PRESS;
		
		return 0; // 아직은 눌렀다 뗀상태가 아님
	
	}
	else if(button_status[button_num] == BUTTON_PRESS && current_state == BUTTON_RELEASE){
		//버튼이 이전에 눌러진 상태 였으며 지금은 떼어진 상태
		_delay_ms(15); //노이즈가 지나가길 기다린다
		button_status[button_num] = BUTTON_RELEASE; // 다음 버튼을 체크하기 위해 초기화
		
		return 1; // 완전히 1번 눌렀다 뗀상태로 인정한다
	}
	
	return 0; //버튼이 open 상태이다
	
}

