

#include "ultrasonic.h"

//INT4 : PE4 외부 INT4 초음파 센서 상승, 하강에지 발생시 이곳으로 진입
//결국 상승에지때 1번 들어오고 하강에지일때 1번씩 들어온다

extern volatile int ultrasonic_check_time;

volatile int ultrasonic_distance = 0;
volatile char scm[50];
volatile int left_distance = 0;
volatile int right_distance = 0;
volatile int mid_distance = 0;
volatile uint8_t current_sensor = SENSER_LEFT;

void init_ultrasonic();
void make_trigger(uint8_t pin);
void ultrasonic_processing();


// 1. 왼쪽 센서 (PE4)
ISR(INT4_vect) {
	if (PINE & (1 << PORTE4)) { TCNT3 = 0; }
	else { left_distance = (TCNT3 * 1000000.0 * 1024 / F_CPU)/ 58; }
}

// 2. 중앙 센서 (PE5)
ISR(INT5_vect) {
	if (PINE & (1 << PORTE5)) { TCNT3 = 0; }
	else { mid_distance = (TCNT3 * 1000000.0 * 1024 / F_CPU)/ 58; }
}

// 3. 오른쪽 센서 (PE6)
ISR(INT6_vect) {
	if (PINE & (1 << PORTE6)) { TCNT3 = 0; }
	else { right_distance = (TCNT3 * 1000000.0 * 1024 / F_CPU)/ 58; }
}

void init_ultrasonic(){
	TRIG_DDR |= 1 << TRIG_LEFT | 1 << TRIG_RIGHT | 1 << TRIG_MID; // output mode로 설정
	ECHO_DDR &= ~(1 << ECHO_LEFT | 1 << ECHO_RIGHT | 1 << ECHO_MID); // input mode 설정
	
	//289p 표 12-6 그림 288p 참조
	//0 1 : 상승에지 하강에지 둘다 INT를 띄우도록 요청한다.
	EICRB |= (1 << ISC40) | (1 << ISC50) | (1 << ISC60);
	
	//분주비를 1024로 설정 318p 표14-1
	TCCR3B |= 1 << CS32 | 1 << CS30;
	
	// ----- EINT4, 5, 6 설정 -------
	//287p 그림 12-6
	EIMSK |= 1 << INT4 | 1 << INT5  | 1 << INT6; //외부 INT4(ECHO핀)
}

void make_trigger(uint8_t pin){
	//시작은 뭔지모르니까 low로 떨어뜨려놓고 시작 0부터
	TRIG_PORT &= ~(1 << pin);
	_delay_us(1);
	
	TRIG_PORT |= (1 << pin); // HIGH로 만듬
	_delay_us(15); // 규격에는 10us인데 redauncy로 15us 
	
	TRIG_PORT &= ~(1 << pin);
}
// 초음파를 같이 체크하면 간섭때문에 망함 
// 초음파 사라지기까지 시간 차이를 두고 체크해야함

void ultrasonic_processing(){
	
	if(ultrasonic_check_time >= 60){
		ultrasonic_check_time = 0;
	}
	else if(ultrasonic_check_time == 0){
		current_sensor = SENSER_LEFT;
		make_trigger(TRIG_LEFT);
	}
	else if(ultrasonic_check_time == 20){
		current_sensor = SENSER_MID;
		make_trigger(TRIG_MID);
	}
	else if(ultrasonic_check_time == 40){
		current_sensor = SENSER_RIGHT;
		make_trigger(TRIG_RIGHT);
	}
}

