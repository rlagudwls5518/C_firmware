/*
* 02.FND_CONTROL_MY.c
*
* Created: 2026-06-12 오후 1:20:56
* Author : kccistc
*/

#include <avr/io.h>
#include "fnd_button.h"

extern void init_button();
extern int get_button(int button_num, int button_pin);
extern int fnd_main(int time_mode);
extern void init_fnd();
extern void reset_stopwatch(int stopwatch_reset_mode);
extern void time_stop();
extern void init_fnd_stopwatch();


int main(void)
{
	static int time_mode = 0;
	static int stopwatch_reset_mode = 0;
	
	init_button();
	init_fnd();
	
	while (1){
		if(get_button(BUTTON0, BUTTON0PIN)){
			time_mode = (time_mode + 1) % 3;
		}
		
		fnd_main(time_mode);
		
		if(get_button(BUTTON1, BUTTON1PIN)){
			//스톱워치 시작 일시정지
			time_stop();
		}
		
		if(get_button(BUTTON2, BUTTON2PIN)){
			//스톱워치 리셋   한번더 누르면 시작
			stopwatch_reset_mode = !stopwatch_reset_mode;
			reset_stopwatch(stopwatch_reset_mode);
		}
	}
}

