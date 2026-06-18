

#define F_CPU 16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h> // sei 등 함수
#include <util/delay.h>

void init_timer0();
extern void init_button();
extern void init_led();
extern void init_uart0();
extern void UART0_transmit(uint8_t data);
extern void init_ultrasonic();
extern void ultrasonit_processing();
extern void init_timer3_pwm();
extern void init_motor_driver();
extern void dcmoter_pwm_comtrol_main();

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);//화면으로 출력되도록 매핑

volatile uint32_t msec_count = 0; //volatile 최적화 방지
volatile int ultrasonic_check_time = 0;

ISR(TIMER0_OVF_vect){
	TCNT0 = 6; // TCNT0 6~256 : 250개 펄스 count 하기 위해
	msec_count ++; // 1ms count
	ultrasonic_check_time ++;
}

int main(void)
{
	init_button();
	init_led();
	init_timer0();
	init_uart0();
	init_timer3_pwm();
	init_motor_driver();
	
	//init_ultrasonic();
	
	stdout = &OUTPUT; // printf가 동작 할 수 있도록 stdout을 설정
	sei(); // 전역(대문) interrupt 허용
	
	dcmoter_pwm_comtrol_main();
	
	while (1) {
		//ultrasonit_processing();
	}
}


void init_timer0(){
	TCNT0 = 6; //TCNT0 6~256 : 250게 펄스 count하기 위해
	
	TCCR0 &= ~(1 << CS02 | 1 << CS01 | 1<< CS00); //0분주 초기화 설정
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //64분주
	TIMSK |= 1 << TOIE0; // TIMER0 OVERflow INT
}




