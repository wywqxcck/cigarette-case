#ifndef __USART_H
#define __USART_H


#include "stdio.h"	
#include "sys.h"
#include "string.h" 

void uart_init(u32 bound);
void numberToChinese(int num, char* result);

#endif


