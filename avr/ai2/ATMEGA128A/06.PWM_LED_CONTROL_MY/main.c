/*
 * 06.PWM_LED_CONTROL_MY.c
 *
 * Created: 2026-06-18 오전 10:24:51
 * Author : kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED_TIME 20
#define PB4 PORTB4

void turn_on_led_in_PWM_manner(int dim);

int main(void)
{
	DDRB |= 1 << PB4;
   
   int dim = 0;
   int direction = 1;
   
   
   TCCR0 |= 1 << WGM01 | 1<< WGM00;
   TCCR0 |= 1 << COM01;
   TCCR0 |= 1 << CS02 | 1 << CS01 | 1 << CS00;
   
    while (1) {
		OCR0 = dim;
		_delay_ms(10);
		
		dim += direction;
		
		if(dim == 0) direction = 1;
		if(dim == 255) direction = -1;
    }
}

void turn_on_led_in_PWM_manner(int dim){
	
	PORTB = 0xff;
	
	for(int i =0; i< 256; i++){
		if(i > dim)PORTB = 0x00;
		_delay_us(LED_TIME);
	}
}

