/*
 * def.h
 *
 * Created: 2026-06-22 오후 1:20:19
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h> // sei 등 함수
#include <util/delay.h>

#define MANUAL_MODE 0
#define AUTO_MODE 1
#define COUNT_MODE 2

#define WARNING_LEFT 0
#define WARNING_RIGHT 1
#define WARNING_MID 2
#define NO_WARNING 3

#define TIME 4


extern void init_button();
extern int get_button(int button_num, int button_pin);
extern void init_led();
extern void init_fnd();
extern void led_on();
extern void led_off();
extern void fnd_state_display(int state);
extern void fnd_racing_time_display();
extern void final_count_display(int count, int index);
extern void init_uart1();
extern void UART1_transmit(uint8_t bt_data);
extern void init_ultrasonic();
extern void ultrasonic_processing();
extern void init_motor_driver();
extern void init_timer1_pwm();
extern void forward(int speed);
extern void backward(int speed);
extern void turn_left(int speed);
extern void turn_right(int speed);
extern void stop();

void init_f();
void init_timer0();
void mode_check();
void manual_mode();
void auto_mode();
void mode_check();
void distance_check();
void count_mode();