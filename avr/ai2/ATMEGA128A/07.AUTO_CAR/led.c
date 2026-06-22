

#include "led.h"
void init_led(void);
void led_on(void);
void led_off(void);

void init_led(void)
{
	DDRG |= (1 << PORTG2);  // PORTG 를 출력 모드로 설정
	
}

void led_on(void)
{
	PORTG |= (1 << PORTG2);
}

void led_off(void)
{
	PORTG &= ~(1 << PORTG2);

}
