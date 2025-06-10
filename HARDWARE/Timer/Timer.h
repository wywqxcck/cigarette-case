#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "key.h"

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM1_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);

uint64_t Get_OS_Time(void);

#endif
