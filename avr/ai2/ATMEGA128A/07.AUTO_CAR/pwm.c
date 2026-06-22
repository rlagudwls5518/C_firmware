/*
 * pwm.c
 *
 * Created: 2026-06-18 오후 2:23:06
 *  Author: kccistc
 */
#include "pwm.h" 

void init_timer3_pwm();
void init_motor_driver();
void dcmoter_pwm_comtrol_main();

// 16 bit 1번 timer/counter를 사용
/*
	pwm 출력신호
	==========
	PB5 = OC1A(왼쪽바퀴)
	PB6 = OC1B(오른쪽바퀴)
	BTN0 : auto/manual mode 설정 
	
	방향 설정
	=========
	왼쪽 바퀴
	 PORTF0(IN1) (DC moter Driver)
	 PORTF1(IN2)
	 
	오른쪽 바퀴
	 PORTF2(IN3) (DC moter Driver)
	 PORTF3(IN4)
	 
	 
	 IN1/IN3     IN2/IN4
	 =======     =======
	    0            1   : 역회전
	    1            0   : 정회전
	    1            1   : stop				
*/

#define MOTOR_LEFT_PORT_DDR  5 //OC1A
#define MOTOR_RIGHT_PORT_DDR  6 //OC1B

#define MOTOR_PWM_DDR  DDRB

#define MOTOR_DIRECTION_PORT PORTF

#define MOTOR_DIRECTION_PORT_DDR DDRF

void init_motor_driver();
void init_timer1_pwm();
void forward(int speed);
void backward(int speed);
void turn_left(int speed);
void turn_right(int speed);
void stop();

void init_timer1_pwm(){
	
	//분주비 64분주
	//16000000Hz / 64 => 250000Hz (250KHz)
	//펄스 하나당 T = 1/f  1/250000Hz --> 0.000004sec -> 4us
	//250000Hz에서 256개의 펄스를 카운트하면 소요시간 : 1.02ms
	//127개 펄스를 카운트 하면 0.5ms
	//0x3ff(1023)개 펄스를 카운트 하면 4ms
	//TCNT3 : 0~256(0x00ff) 까지 카운트한 후 0으로 돌아감
	TCCR1B |= 1 << CS11 | 1 << CS10;

	
	//모드 14번 사용 16bit 고속 BWM모드 327p
	TCCR1A |= 1 << WGM11; // top -> ICR1로 설정
	TCCR1B |= 1 << WGM12 | 1 << WGM13;
	
	// 비반전 모드 top : ICR1 비교일치 값 (PWM) 지정 OCR1A OCR1B P350 표15-7  
	// 비교 일치 발생시 OCR1, OCR1B의 출력핀은 LOW로 바뀌고 BOTTOM에서 HIGH로 바뀐다. 
	TCCR1A |= 1 << COM1A1;  
	TCCR1A |= 1 << COM1B1;  
	
	ICR1 = 0x3ff; // 1023 * 4us = 4ms TOP값
	
	

}

/*
	방향 설정
	=========
	왼쪽 바퀴
	PORTF0(IN1) (DC moter Driver)
	PORTF1(IN2)
	
	오른쪽 바퀴
	PORTF2(IN3) (DC moter Driver)
	PORTF3(IN4)
	
	
	IN1/IN3     IN2/IN4
	=======     =======
	0            1   : 역회전
	1            0   : 정회전
	1            1   : stop
*/
void init_motor_driver(void)
{
	// 1. 출력 모드로 설정
	MOTOR_PWM_DDR &= ~(1 << 5 | 1 << 6);   // 0 으로 초기화 하고 시작 하자 
	MOTOR_PWM_DDR |= 1 << 5 | 1 << 6;
	MOTOR_DIRECTION_PORT_DDR &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT_DDR |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3;// 출력모드 설정
	
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 2 | 1 << 0; //모터를 전진모드로 IN1IN2  IN3IN4 전진모드 1010

}

void forward(int speed){
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 2 | 1 << 0; //모터를 전진모드로 IN1IN2  IN3IN4 전진모드 1010
	
	OCR1A = OCR1B = speed; // OCR1A PWM left  OCR1B PWM right
}

void backward(int speed){
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 3 | 1 << 1; //모터를 전진모드로 IN1IN2  IN3IN4 전진모드 1010
	
	OCR1A = OCR1B = speed; // OCR1A PWM left  OCR1B PWM right
}

void turn_left(int speed)
{
	//  모터를 전진 모드로
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);  // 0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |=  1 << 2 | 1 << 0;   // 모터를 전진 모드로 IN4IN3IN2IN1
	//                    0  1 0  1
	OCR1A=0;
	OCR1B=speed;   // OCR1A: PWM left OCR1B: PWM right
}
//죄, 우회전에서는 반대쪽 속도 제로로만듬

void turn_right(int speed)
{
	//  모터를 전진 모드로
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);  // 0 으로 초기화 하고 시작
	MOTOR_DIRECTION_PORT |=  1 << 2 | 1 << 0;   // 모터를 전진 모드로 IN4IN3IN2IN1
	//                    0  1 0  1
	OCR1A=speed;
	OCR1B=0;   // OCR1A: PWM left OCR1B: PWM right
}

void stop(){
	MOTOR_DIRECTION_PORT &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3);
	MOTOR_DIRECTION_PORT |= 1 << 0 | 1 << 1 | 1 << 2 | 1 << 3; //모터를 전진모드로 IN1IN2  IN3IN4 전진모드 1010
	
	OCR1A = 0; // OCR1A PWM left  OCR1B PWM right
	OCR1B  = 0;
}

