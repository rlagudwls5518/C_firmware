#include <avr/io.h>
#include "fnd_button.h"
#include <avr/interrupt.h>

static int time_mode = 0;
static int stopwatch_reset_mode = 0;

extern void init_button();
extern int get_button(int button_num, int button_pin);
extern int fnd_main(int time_mode);
extern void init_fnd();
extern void reset_stopwatch(int stopwatch_reset_mode);
extern void time_stop();
void init_timer2();

volatile uint32_t ms_count = 0;
volatile uint32_t stopwatch_ms_count = 0;
volatile uint32_t sec_count = 0;
volatile uint32_t stopwatch_run = 1;

ISR(TIMER2_OVF_vect){
	TCNT2 = 193;
	DDRA = 0x00;
	PORTA = 0xFF;
	
	ms_count++; // 1ms count
	
	if(ms_count >= 1000){
		ms_count = 0;
		sec_count++;
	}
	
	if(stopwatch_run){
		stopwatch_ms_count++;
		if(stopwatch_ms_count >= 60000){
			stopwatch_ms_count = 0;
		}
	}
}

int main(void)
{
	init_button();
	init_fnd();
	init_timer2();
	
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
	TCNT2 = 193;
	
	TCCR2 = 0x00; //0분주 초기화 설정
	TCCR2 |= (1 << CS22) | (1 << CS21) | (0 << CS20); //256분주
	TIMSK |= (1 << TOIE2); // TIMER2 OVERflow INT
	sei(); // 전역(대문) interrupt 허용
}


