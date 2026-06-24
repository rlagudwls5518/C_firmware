#include "def.h"
#include "button.h"

int UART1_transmit_printf(char c, FILE *stream) {
	UART1_transmit(c); // 내부적으로 기존 전송 함수를 호출
	return 0;
}
FILE OUTPUT = FDEV_SETUP_STREAM(UART1_transmit_printf, NULL, _FDEV_SETUP_WRITE);//화면으로 출력되도록 매핑

volatile uint32_t msec_count = 0; //volatile 최적화 방지
volatile uint32_t sec_count = 0;
volatile uint32_t m_count = 0;
volatile uint32_t time_run = 0;
volatile int ultrasonic_check_time = 0;
extern volatile uint8_t bt_data;
extern volatile int left_distance;
extern volatile int right_distance;
extern volatile int mid_distance;
int function_state = MANUAL_MODE;
static int warning_state = 0;
int back_count = 0;
int left_count = 0;
int right_count = 0;
static int count_index = 0;

void (*pfunc[]) () =
{
	manual_mode,
	auto_mode
};

ISR(TIMER0_OVF_vect){
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count 하기 위해
	msec_count ++; // 1ms count
	ultrasonic_check_time ++;
	if(time_run){
		if(msec_count >= 1000){
			msec_count = 0;
			sec_count++;
		}
		
		if(sec_count >= 60){
			sec_count = 0;
			m_count++;
		}	
	}
}

int main(void)
{
	init_f();
	sei();
	
	while (1) {
		
		if(get_button(BUTTON0,BUTTON0PIN)){
			time_run = !time_run;
			function_state = !function_state;
			mode_check();
		}
		pfunc[function_state] ();
		fnd_racing_time_display();
		fnd_state_display(warning_state);
	}

}

void init_f(){
	init_button();
	init_led();
	init_fnd();
	init_timer0();
	init_uart1();
	init_timer1_pwm();
	init_motor_driver();
	init_ultrasonic();
}

void init_timer0(){
	TCNT0 = 6; //TCNT0 6~256 : 250게 펄스 count하기 위해
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1 << CS00); //0분주 초기화 설정
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //64분주
	TIMSK |= 1 << TOIE0; // TIMER0 OVERflow INT
}

void manual_mode(){
	switch(bt_data){
		case 'F':
		case 'f':
			forward(500); //4 * 500 = 0.002(2md)
			break;
		case 'B':
		case 'b':
			backward(700);
			break;
		case 'L':
		case 'l':
			turn_left(700);
			break;
		case 'R':
		case 'r':
			turn_right(500);
			break;
		case 'S':
		case 's':
			stop();
			break;
		default:
		break;
	}
}
void auto_mode(){
	distance_check();
	if(warning_state == NO_WARNING){
		forward(600);
	}
	
	else if(warning_state == WARNING_RIGHT) {
		turn_right(700); // 그 다음 우회전
		right_count++;
		
		
	}
	else if(warning_state == WARNING_LEFT) {
		turn_left(700); // 그 다음 좌회전
		left_count++;
		

	}
	else if(warning_state == WARNING_MID){
		backward(400);
		back_count++;
	}
	fnd_state_display(warning_state);
}

void mode_check(){
	if(function_state == AUTO_MODE){
		led_on();
	}
	
	else if(function_state == MANUAL_MODE){
		led_off();
	}

}
/*
void count_mode(){
	switch(count_index){
		case 0:
			final_count_display(back_count,WARNING_MID);
			break;
		case 1:
			final_count_display(right_count,WARNING_RIGHT);
			break;
		case 2:
			final_count_display(left_count,WARNING_LEFT);
			break;
		case 3:
			final_count_display(sec_count,TIME);
			break;
		default:
			break;
	}
}
*/

void distance_check(){
	 
	ultrasonic_processing();
	
	//방향전환 구간
	if (mid_distance > 0 && mid_distance < 15) warning_state = WARNING_MID;
	else if (left_distance > 0 && left_distance < 30) warning_state = WARNING_LEFT;
	else if (right_distance > 0 && right_distance < 30) warning_state = WARNING_RIGHT;
	else warning_state = NO_WARNING;
}





