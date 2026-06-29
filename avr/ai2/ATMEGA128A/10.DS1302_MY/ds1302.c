/*
 * ds1302.c
 *
 * Created: 2026-06-26 오후 2:42:15
 *  Author: kccistc
 */ 

#include "ds1302.h"

extern void pc_command_processing(t_ds1302* ds1302);

void ds1302_main();
void init_date_time(t_ds1302* ds1302);
void init_gpio_ds1302();
void init_ddr_ds1302();
void clook_de1302();
void tx_ds1302(uint8_t data);
void rx_ds1302(uint8_t *pdata);
void write_ds1302(uint8_t addr, uint8_t data);
uint8_t read_ds1302(uint8_t addr);
uint8_t dec_to_bcd(uint8_t data);
uint8_t bcd_to_dec(uint8_t data);
void init_ds1302(t_ds1302* ds1302);
void read_time_ds1302(t_ds1302* ds1302);
void read_date_ds1302(t_ds1302* ds1302);
char* date_kor[7] = {"토", "일", "월", "화", "수", "목", "금"};

	
void ds1302_main(){
	
	t_ds1302 ds1302;
	
	init_ddr_ds1302();
	init_date_time(&ds1302);
	init_gpio_ds1302(); // all low로 설정
	init_ds1302(&ds1302);
	
	while(1){
		
		pc_command_processing(&ds1302);
		
		//1. read time
		read_time_ds1302(&ds1302);
		//2. read date
		read_date_ds1302(&ds1302);
		//3. printf date, time
		printf("%d년%d월%d일 %d시%d분%d초\n",ds1302.year, ds1302.month, ds1302.date, ds1302.hours, ds1302.minutes, ds1302.seconds);
		//4. delay_ms(1000);
		_delay_ms(1000);
	}	
	
	
}
void set_rtc_from_uart(char* str, t_ds1302* ds1302){
	char temp[3];
	temp[2] ='\0';
	
	ds1302->year = atoi(strncpy(temp, str + 6, 2));
	ds1302->month = atoi(strncpy(temp, str + 8, 2));
	ds1302->date = atoi(strncpy(temp, str + 10, 2));
	ds1302->hours = atoi(strncpy(temp, str + 12, 2));
	ds1302->minutes = atoi(strncpy(temp, str + 14, 2));
	ds1302->seconds = atoi(strncpy(temp, str + 16, 2));
	
	init_ds1302(&ds1302);
}


void read_time_ds1302(t_ds1302* ds1302){
	ds1302->hours = read_ds1302(ADDR_HOUR);
	ds1302->minutes = read_ds1302(ADDR_MINUTES);
	ds1302->seconds =  read_ds1302(ADDR_SECONDS);
}

void read_date_ds1302(t_ds1302* ds1302){
	ds1302->year = read_ds1302(ADDR_YEAR);
	ds1302->month = read_ds1302(ADDR_MONTH);
	ds1302->day_of_week = read_ds1302(ADDR_DAY_OF_WEEK);
	ds1302->date = read_ds1302(ADDR_DATE);
}

void init_ds1302(t_ds1302* ds1302){
	write_ds1302(ADDR_SECONDS, ds1302->seconds);
	write_ds1302(ADDR_MINUTES, ds1302->minutes);
	write_ds1302(ADDR_HOUR, ds1302->hours);
	write_ds1302(ADDR_DATE, ds1302->date);
	write_ds1302(ADDR_MONTH, ds1302->month);
	write_ds1302(ADDR_DAY_OF_WEEK, ds1302->day_of_week);
	write_ds1302(ADDR_YEAR, ds1302->year);
}

//dec --> bcd변환
//예 25 
//dec           bcd변환
//0001 1001 -> 0010 0101
uint8_t dec_to_bcd(uint8_t data){
	uint8_t high, low;
	high = (data / 10) << 4;
	low = data % 10; 
	
	return  high + low;
}
// 1. 입력 bcd
// 예 26년의 bcd
// 7654 3210
// 0010 0110
//    2    6
//  x10   x1
//============
//   26
uint8_t bcd_to_dec(uint8_t data){
	uint8_t high, low;
	high = (data >> 4) * 10;
	low = data & 0x0f;
	
	return  high + low;
}

void tx_ds1302(uint8_t data){
	DS1302_DATA_DDR |= 1 << DS1302_DATA;// write mode
	
	// 예) 0x80
	//  MSB     LSB
	//  1000 0000
	
	for(int i = 0; i < 8; i++){
		if(data & (1 << i)) DS1302_DATA_PORT |= 1 << DS1302_DATA;
		else DS1302_DATA_PORT &= ~(1 << DS1302_DATA);
		
		clook_de1302();
	}
}

void rx_ds1302(uint8_t *pdata){
	
	uint8_t temp = 0;
	DS1302_DATA_DDR &= ~(1 << DS1302_DATA);
	
	//LSB로부터 차례로 입력
	for(int i = 0; i < 8; i++){
		if(DS1302_DATA_PIN & (1 << DS1302_DATA)){
			temp |= 1 << i; //1을 조건만 set
		}
		if(i == 7) break; //마지막비트는 clk 안보냄
		clook_de1302();
	}	
	*pdata = temp;
}

void clook_de1302(){
	//LOW -> HIGH --> LOW
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK);
	DS1302_CLK_PORT |= 1 << DS1302_CLK;
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK);
}

uint8_t read_ds1302(uint8_t addr){
	
	uint8_t data8bits = 0; // 1비트씩 읽어서 담을 변수
	
	//1. CE high로 변경
	DS1302_RST_PORT |=  1 << DS1302_RST;
	//2. addr 전송
	tx_ds1302(addr + 1); // read addr
	//3. data를 읽어들임( bct to dec 다시변환)
	rx_ds1302(&data8bits);
	//4. CE low
	DS1302_RST_PORT &=  ~(1 << DS1302_RST);
	//5. return
	return  bcd_to_dec(data8bits);
}

void write_ds1302(uint8_t addr, uint8_t data){
	
	// 1. CE low --> HIGH
	DS1302_RST_PORT |=  1 << DS1302_RST;
	// 2. ADDR 전송
	tx_ds1302(addr);
	// 3. DATA 전송
	tx_ds1302(dec_to_bcd(data));
	// 4. CE HIGH --> LOW
	DS1302_RST_PORT &=  ~(1 << DS1302_RST);
}

void init_ddr_ds1302(){
	DDRF &= ~(1 << DS1302_CLK | 1 << DS1302_DATA | 1 << DS1302_RST);
	DDRF |= 1 << DS1302_CLK | 1 << DS1302_DATA | 1 << DS1302_RST;
}

void init_gpio_ds1302(){
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK | 1 << DS1302_DATA | 1 << DS1302_RST);
	_delay_ms(2);
}

void init_date_time(t_ds1302* ds1302){
	ds1302->year = 26;
	ds1302->month = 6;
	ds1302->date = 26;
	ds1302->day_of_week = 6; // Friday
	ds1302->hours = 9;
	ds1302->minutes = 50;
	ds1302->seconds = 00;
}