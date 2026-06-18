/*
* utrasonic.c
*
* Created: 2026-06-17 오후 1:33:06
*  Author: kccistc
*/

#include "ultrasonic.h"

//INT4 : PE4 외부 INT4 초음파 센서 상승, 하강에지 발생시 이곳으로 진입
//결국 상승에지때 1번 들어오고 하강에지일때 1번씩 들어온다

extern volatile int ultrasonic_check_time;
extern void led_ultrasonic_on(int distance);

volatile int ultrasonic_distance = 0;
volatile char scm[50];
void init_ultrasonic();
void make_trigger();
void ultrasonit_processing();

enum FLAG{
	YES = 1,
	NO = 0
};
volatile int trigger_check_detect = YES;

// P278 표12.3
// INT4 : PE4 외부 INT4 초음파 센서 상승.하강에지 발생시 이곳으로 진입
// 결국 상승에지떄 1번들어오고 하강에지때 1번씩 들어 온다.
// 0x000A

ISR(INT4_vect){
	//1. 상승에지 
	if(ECHO_PORT & (1 << ECHO_PIN)){
		TCNT1 = 0;
		
	}
	//2. 하강에지
	else{
		//TCNT1  : TimerCount 1
		// 예) TCNCT1이 10이 들어있다고 가정
		//15.635KHz의 1주기 : 64us
		//0.000064sec *10개 = 0.00064us -> 640us
		//640us / 58(1cm이동하는데 소요시간) : 11cm
		//1sec : 1000000us
		
		ultrasonic_distance = (TCNT1 * 1000000.0 * 1024 / F_CPU)/58;
		
		// 소요시간을 cm로 환산
		sprintf((char *)scm, "dis : %dcm\n", ultrasonic_distance);
		trigger_check_detect = YES;
	}
}

void init_ultrasonic(){
	TRIG_DDR |= 1 << TRIG_PIN; // output mode로 설정
	ECHO_DDR &= ~(1 << ECHO_PIN); // input mode 설정
	
	//289p 표 12-6 그림 288p 참조
	//0 1 : 상승에지 하강에지 둘다 INT를 띄우도록 요청한다.
	EICRB |= 0 << ISC41 | 1 << ISC40;
	//16bit timer/counter 1번을 사용하기로 하자
	//timer int를 사용하지 않는다
	//16bit timer1 16bit로 표시할 수 있는 최대값 65535(max) : 0xffff
	//16MHz / 1024분주 : 16000000Hz / 1024 => 15625Hz  -> 15.625KHz
	//1주기( 1개의 필수 소요시간) : T = 1/f 1/15625 -> 0.000064sec -> 64us
	
	//분주비를 1024로 설정 318p 표14-1
	TCCR1B |= 1 << CS12 | 1 << CS10;
	
	// ----- EINT4 설정 -------
	//287p 그림 12-6
	EIMSK |= 1 << INT4; //외부 INT4(ECHO핀)
	
	 
}

void make_trigger(void)
{
	TRIG_PORT &= ~(1 << TRIG_PIN);   // low로 만든다
	_delay_us(1);
	
	TRIG_PORT |= (1 << TRIG_PIN);   // high로 만든다	
	_delay_us(15);       // 규격에는 10us인데 reduancy로 15us
	
	TRIG_PORT &= ~(1 << TRIG_PIN);   // low로 만든다
	trigger_check_detect = NO;
}

void ultrasonit_processing(){
	if(trigger_check_detect){ 
		ultrasonic_check_time = 0;
		printf("%s", scm);
		make_trigger();	
	}
	led_ultrasonic_on(ultrasonic_distance);
}

