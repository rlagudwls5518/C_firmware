/*
 * 08.PWM_SERVO_PIEZO_BUZZER.c
 *
 * Created: 2026-06-25 오전 9:46:53
 * Author : kccistc
 */ 

#if 1
#include "button.h"
#define F_CPU	16000000UL
#include <avr/io.h>
#include <util/delay.h>

extern int get_button(int button_num, int button_pin);
extern void Music_Player(const int *tone, const int *Beats);
extern void init_speaker(void);
extern void init_button();
extern void Beep(int  repeat);
extern void Siren(int repeat);
extern void RRR(void);
extern void power_on_melody();
extern void open_buzzer();
extern void drum_waching_stop_melody(); 

extern const int Elise_Tune[];
extern const int Elise_Beats[];
extern int Love_Attack_Tune[];
extern const int Love_Attack_Beats[];
extern const int LG_WASHING_MASHINE_END_Tune[];
extern const int LG_WASHING_MASHINE_END_Beat[];

int melody_start = 0;
int buzzer_start = 0;
int washing_melody_start = 0;
// PE3 (OC3A) PWM 출력 사용.
// 16bit Timer/Counter
// OCR3A값이 같아지면 Low 출력.

int main(void)
{
	init_speaker();	
	init_button();

	while(1)
	{
		
		if(get_button(BUTTON0, BUTTON0PIN)){
			melody_start = !melody_start;
			if(melody_start) power_on_melody(); 
			else OCR3A = 0;
			
		}
		if(get_button(BUTTON1, BUTTON1PIN)){
			 buzzer_start = ! buzzer_start;
			if(buzzer_start) open_buzzer();
			else OCR3A = 0;
		}
		if(get_button(BUTTON2, BUTTON2PIN)){
			washing_melody_start = !washing_melody_start;
			if(washing_melody_start) drum_waching_stop_melody(); 
			else OCR3A = 0;
		}
/*
		OCR3A=1702;   // 삐~뽀 레 
		_delay_ms(1000);
		OCR3A=1431;   // 파  
		_delay_ms(1000);

*/
/*		
		Beep(5);
		RRR();
 		_delay_ms(3000);
 		Siren(5);
 		_delay_ms(3000);
		Music_Player(Elise_Tune, Elise_Beats);		
*/		
	}
}
#endif






#if 0 // servo motor
#include <avr/io.h>

extern int servo_motor_main();


int main(void)
{
    
	servo_motor_main();
	while(1){
		
	}
}
#endif

