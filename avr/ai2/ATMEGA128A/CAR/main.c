#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {


	// 2. 포트 방향 설정 (PB5, PB6 / PF0~PF3 출력)
	DDRB|= (1 << PORTB5) | (1 << PORTB6);
	DDRF |= (1 << PORTF0) | (1 << PORTF1) | (1 << PORTF2) | (1 << PORTF3);
	DDRG |= 1 << PORTG2;

	while(1) {
		// 3. 무조건 전진 신호 고정 출력
		PORTB |= (1 << PORTB5) | (1 << PORTB6); // ENA, ENB High
		PORTF |= (1 << PORTF0) | (1 << PORTF2); // INT1, INT3 High
		PORTF &= ~((1 << PORTF1) | (1 << PORTF3)); // INT2, INT4 Low
		PORTG |=  1 << PORTG2;
	}
	return 0;
}