#include <avr/io.h>
#include "fnd_button.h"

extern void init_button();
extern int get_button(int button_num, int button_pin);
extern int fnd_main(int time_mode);
extern void init_fnd();
extern void reset_stopwatch(int stopwatch_reset_mode);
extern void time_stop();
extern void init_fnd_stopwatch();

void init_timer0();
volatile uint32_t msec_count = 0;
volatile uint32_t stopwatch_msec_count = 0;

ISR(TIMER2_OVF_vect){
	TCNT2 = 6; // TCNT2 6~256 : 250개 펄스 count하기 위해
	msec_count++; // 1ms count
}

int main(void)
{
	static int time_mode = 0;
	static int stopwatch_reset_mode = 0;
	
	init_button();
	init_fnd();
	init_timer0();
	
	while (1){
		if(get_button(BUTTON0, BUTTON0PIN)){
			time_mode = (time_mode + 1) % 3;
		}
		
		fnd_main(time_mode);
		
		if(get_button(BUTTON1, BUTTON1PIN)){
			//스톱워치 시작 일시정지
			time_stop();
		}
		
		if(get_button(BUTTON2, BUTTON2PIN)){
			//스톱워치 리셋   한번더 누르면 시작
			stopwatch_reset_mode = !stopwatch_reset_mode;
			reset_stopwatch(stopwatch_reset_mode);
		}
	}
}

void init_timer2(){
	TCNT2 = 6; //TCNT0 6~256 : 250게 펄스 count하기 위해
	
	TCCR2 = 0x00; //0분주 초기화 설정
	TCCR2 |= 1 << CS22 | 1 << CS21 | 0 << CS20; //256분주
	TIMSK |= 1 << TOIE2; // TIMER2 OVERflow INT
	sei(); // 전역(대문) interrupt 허용
}

