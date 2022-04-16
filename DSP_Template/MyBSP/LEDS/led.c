/*
 * led.c
 *
 *  Created on: 2022年3月17日
 *      Author: Administrator
 */
#include "leds.h"

void Gpio_LedInit(void)
{
	EALLOW;									//GPIO寄存器受保护
	//LED端口配置
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;		//GPIO40作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;		//GPIO40方向为输出
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;		//使能GPIO上拉电阻
//	GpioDataRegs.GPADAT.bit.GPIO3 = 1;  	//GPIO40输出高电平

	EDIS;
}


