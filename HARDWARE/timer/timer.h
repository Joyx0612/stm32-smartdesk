#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"
#include "wai_bian.h"

void Tim3_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);


#endif


