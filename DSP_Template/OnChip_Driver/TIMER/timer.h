/*
 * timer.h
 *
 *  Created on: 2022Äê3ÔÂ17ÈÕ
 *      Author: Administrator
 */

//DSP2803x_CPU_TIMERS_H

#ifndef ONCHIP_DRIVER_TIMER_TIMER_H_
#define ONCHIP_DRIVER_TIMER_TIMER_H_

#include "DSP2803x_Device.h"
#include "DSP2803x_Examples.h"
//#include "DSP25035x_CPU_TIMERS.h"

void TIM0_Init(float Freq,float Period);
interrupt void TIM0_IRQn(void);

#endif /* ONCHIP_DRIVER_TIMER_TIMER_H_ */
