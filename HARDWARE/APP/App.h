#ifndef __APP_H
#define __APP_H	 


#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "BLU.h"
#include "oled.h"
#include "DHT11.h"
#include "lock.h"
#include "timer.h"
#include "key.h"


enum Data{
	RESTRICT,
	IN_ALL,
	T,
	H,
	DATA_MAX,
};


extern uint64_t open_time,old_open_time;
extern uint8_t open_flage,add_flage,err_flage;


void App_Init(void);
void App_Run(void);
		 				    
#endif
