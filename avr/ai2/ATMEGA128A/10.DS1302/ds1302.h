/*
 * ds1302.h
 *
 * Created: 2026-06-26 오후 2:42:00
 *  Author: kccistc
 */ 


#define F_CPU 16000000UL  //16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <stdio.h>
#include <string.h>

#define DS1302_CLK_DDR DDRF
#define DS1302_CLK_PORT PORTF

#define DS1302_DATA_DDR DDRF
#define DS1302_DATA_PORT PORTF //write
#define DS1302_DATA_PIN PINF // read

#define DS1302_RST_DDR DDRF
#define DS1302_RST_PORT PORTF

#define DS1302_CLK 0 // 0번 pin
#define DS1302_DATA 1 // 1번 pin
#define DS1302_RST 2 // 2번 pin

#define ADDR_SECONDS 0x80
#define ADDR_MINUTES 0x82
#define ADDR_HOUR 0x84
#define ADDR_DATE 0x86
#define ADDR_MONTH 0x88
#define ADDR_DAY_OF_WEEK 0x8A
#define ADDR_YEAR 0x8C
#define ADDR_WRITEPROTECTED 0x8E

typedef struct _ds1032
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t day_of_week; // 1: sun  2: mon
	uint8_t year;
	uint8_t am_pm; // 1: pm  0: am
	uint8_t hourmode; // 0: 24 1: 12
}t_ds1302;


t_ds1302 ds1302;