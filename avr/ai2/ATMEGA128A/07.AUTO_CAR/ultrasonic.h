 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define TRIG_DDR	DDRA
#define TRIG_PORT	PORTA		
#define TRIG_LEFT	0
#define TRIG_MID	1
#define TRIG_RIGHT	2

#define ECHO_DDR	DDRE
#define ECHO_PORT	PORTE // external INT 4
#define ECHO_LEFT	4
#define ECHO_MID	5
#define ECHO_RIGHT	6

#define SENSER_LEFT 0
#define SENSER_RIGHT 1
#define SENSER_MID 2
