/*
 * leds.h
 *
 *  Created on: 2022Äê3ÔÂ17ÈÕ
 *      Author: Administrator
 */

#ifndef APP_LEDS_LEDS_H_
#define APP_LEDS_LEDS_H_

#include "../../OnChip_Driver/OnChip_Driver_Inc.h"

void Gpio_LedInit();

//#define LED_ON			(GpioDataRegs.GPASET.bit.GPIO3 = 1)
//#define LED_OFF			(GpioDataRegs.GPACLEAR.bit.GPIO3=1)
#define LED_OFF			(GpioDataRegs.GPASET.bit.GPIO3 = 1)	 	  //?
#define LED_ON			(GpioDataRegs.GPACLEAR.bit.GPIO3=1)
#define LED_TOGGLE		(GpioDataRegs.GPATOGGLE.bit.GPIO3=1)

#endif /* APP_LEDS_LEDS_H_ */
