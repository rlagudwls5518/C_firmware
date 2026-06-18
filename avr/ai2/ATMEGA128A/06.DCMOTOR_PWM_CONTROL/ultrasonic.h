 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define TRIG_DDR	DDRG
#define TRIG_PORT	PORTG	
#define TRIG_PIN	4

#define ECHO_DDR	DDRE
#define ECHO_PORT	PINE // external INT 4
#define ECHO_PIN	4
