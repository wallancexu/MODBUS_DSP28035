/*
 * timer.c
 *
 *  Created on: 2022年3月5日
 *      Author: Administrator
 */

#include "timer.h"
#include "../MyBSP/MyBspInc.h"

void TIM0_Init(float Freq,float Period)
{
	EALLOW;
	SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;//CPU Timer 0
	EDIS;

	//设置定时器0的中断入口地址为中断向量表的INT0
	EALLOW;
	PieVectTable.TINT0 = &TIM0_IRQn;
	EDIS;


    // CPU Timer 0
	// Initialize address pointers to respective timer registers:
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	// Initialize timer period to maximum:
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	// Make sure timer is stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;
	ConfigCpuTimer(&CpuTimer0, Freq, Period);

	//开启定时器功能
	CpuTimer0Regs.TCR.bit.TSS=0;
	//开启CPU第一组中断并使能第一组中断的第7个小中断，即定时器0
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7=1;
	//使能总中断
	EINT;
	ERTM;
}

interrupt void TIM0_IRQn(void)
{
	EALLOW;
//	CpuTimer0Regs.TCR.bit.TIF=1;// 15    Interrupt flag
	PieCtrlRegs.PIEACK.all = 1;//
	if(time_usart1 <= 200)
	{
		time_usart1++;
//		Prints("/r/nI'm here/r/n");
	}
	PieCtrlRegs.PIEACK.bit.ACK1=1;
	EDIS;
}
