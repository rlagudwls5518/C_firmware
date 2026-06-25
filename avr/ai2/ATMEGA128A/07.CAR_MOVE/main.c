/*
 * 07.CAR_MOVE.c
 *
 * Created: 2026-06-20 오전 7:52:13
 * Author : rlagu
 */ 

#define F_CPU 16000000UL // 16MHz 클럭 설정 (실제 오실레이터 주파수에 맞게 수정)
#include <avr/io.h>
#include <util/delay.h>

// 핀 정의를 통한 가독성 확보
#define MOTOR_PORT_DIR  PORTF
#define MOTOR_PORT_EN   PORTA

#define IN1 0
#define IN2 1
#define IN3 2
#define IN4 3

#define ENA 3
#define ENB 4


// 모터 초기화 함수
void motor_init() {
	// PORTF의 0, 1, 2, 3번 핀을 출력(1)으로 설정 (IN1 ~ IN4)
	DDRF |= (1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4);
	
	// PORTA의 3, 4번 핀을 출력(1)으로 설정 (ENA, ENB)
	DDRA |= (1 << ENA) | (1 << ENB);
	
	// 초기 상태: 모터 드라이버 Enable 핀을 HIGH로 만들어 모터 활성화
	// (PWM 제어를 하지 않을 때, 단순 ON/OFF를 위해 1을 출력합니다)
	MOTOR_PORT_EN |= (1 << ENA) | (1 << ENB);

}

// 전진 함수
void move_forward() {
	// 1. 먼저 IN2와 IN4 비트를 확실하게 0으로 끕니다.
	MOTOR_PORT_DIR &= ~((1 << IN2) | (1 << IN4));
	
	// 2. 그다음 IN1과 IN3 비트를 확실하게 1로 켭니다.
	MOTOR_PORT_DIR |= (1 << IN1) | (1 << IN3);
}

// 후진 함수
void move_backward() {
	// 1. 먼저 IN1과 IN3 비트를 확실하게 0으로 끕니다.
	MOTOR_PORT_DIR &= ~((1 << IN1) | (1 << IN3));
	
	// 2. 그다음 IN2와 IN4 비트를 확실하게 1로 켭니다.
	MOTOR_PORT_DIR |= (1 << IN2) | (1 << IN4);
}

// 정지 함수
void move_stop() {
	// 모든 제어 핀을 LOW로 만들어 정지
	MOTOR_PORT_DIR &= ~((1 << IN1) | (1 << IN2) | (1 << IN3) | (1 << IN4));
}

int main(void) {
	// 모터 포트 초기화
	motor_init();
	DDRG |= (1 << PG2);
	
	while (1) {
		PORTG |= (1 << PG2);
		// 1. 2초간 전진
		move_forward();
		_delay_ms(2000);
		
		// 2. 1초간 정지
		move_stop();
		_delay_ms(1000);
		
		// 3. 2초간 후진
		move_backward();
		_delay_ms(2000);
		
		// 4. 1초간 정지
		move_stop();
		_delay_ms(1000);
	}
	
	return 0;
}

