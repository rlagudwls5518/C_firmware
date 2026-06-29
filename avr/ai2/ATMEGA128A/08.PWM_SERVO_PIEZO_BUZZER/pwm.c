/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:06
 *  Author: kccistc
 */
#include "button.h" 


// 16 bit 1번 timer/counter를 사용
/*
	pwm 출력신호
	==========
	PB5 = SERVO MOTOR
		
*/

void init_timer1_pwm();
int servo_motor_main();


void init_timer1_pwm(){
	
	//모드 14번 사용 16bit 고속 BWM모드 327p
	TCCR1A |= 1 << WGM11; // top -> ICR1로 설정
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	
	// 비반전 모드 top : ICR1 비교일치 값 (PWM) 지정 OCR1A OCR1B P350 표15-7
	// 비교 일치 발생시 OCR1, OCR1B의 출력핀은 LOW로 바뀌고 BOTTOM에서 HIGH로 바뀐다.
	TCCR1A |= 1 << COM1A1;
	

	//---- 분주비 설정 ----------
	// 분주비 8 를 하는 이유 16MHz가지고 50Hz의 주파수를 바로 만들지 못한다.
	// 16000000Hz / 8 --> 2MHz
	// T=1/f 1/16000000Hz ==> 0.0000000625sec x 8 ==> 0.000005sec (0.5us)
	// T=1/f 1/2000000Hz ==> 0.000005sec (0.5us)
	// 16bit 까지 count할 수 있는 최대 0xffff(65535)
	// 0.000005sec x 65535 ==> 0.0327675sec (32.7675ms) 32.7675ms 마다 timer INT
	// 20ms 길이(Duty)를 갖는 것을 만든다고 한다면
	// 0.000005sec x 40000개 ==> 0.02sec ( 20ms )
	// TCNT1 : 0~39999 까지 count한 후 0으로 다시 돌아 간다.
	TCCR1B &= ~(1 << CS12 | 1 << CS11 | 1 << CS10);   // all reset (기존의 값을 무시)
	TCCR1B |= 1 << CS11;   // 분주비 8

	ICR1 = 40000-1;  // 0.000005sec x 40000개 ==> 20ms TOP 값

}

int servo_motor_main(){
	//servo motor가 pb5으로 출력설정
	DDRB |= 1 << 5;
	init_timer1_pwm();
	
	//0(1ms) --> 90(1.5ms) -> 180(2ms)
	while(1){
		//1ms : 2000개 count
		//40000(20ms) / 20 --> 1ms(2000)
		OCR1A = 2000;
		_delay_ms(1000);
		
		//1.5ms : 3000개 count
		OCR1A = 4000;
		_delay_ms(1000);
		
		//2ms : 4000개 count
		OCR1A = 5000;
		_delay_ms(1000);
	}
	return 0;
}
