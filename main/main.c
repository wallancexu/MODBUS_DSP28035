/***********************************************************************
 *                                  最终版
 *************************************************************************/
#include "../OnChip_Driver/OnChip_Driver_Inc.h"
#include "../MyBSP/MyBspInc.h"

//宏定义声明区

//函数声明区
void Gpio_LedInit(void);

#if DEBUG_EN	> 0
#else
#pragma CODE_SECTION(Gpio_LedInit, "ramfuncs");	//作为演示,将函数Gpio_LedInit()拷贝到RAM中运行

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif

//变量声明区

Uint16 buffer[9] = {0};
unsigned char test[5] = {0};

//主函数入口处(modbus版)
void main(void)
{
// 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
  	InitSysCtrl();

// 第二步: 清除所有中断标志并初始化PIE中断向量表
   	DINT;			//先禁止CPU中断
   	InitPieCtrl();	//给PIE寄存器赋初值
   	IER = 0x0000;	//禁止CPU的中断并清楚所有相关的中断标志
   	IFR = 0x0000;
   	InitPieVectTable();//初始化中断向量表

	#if DEBUG_EN	> 0				/* 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,
										 否则进入Flash固化模式,即要执行flash的初始化并将设定的部分在
										 RAM里面运行的代码拷贝到flash运行*/
	#else
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
		InitFlash();			//初始化flash
	#endif

// 第三步：初始化其他使用的外设
   	Gpio_LedInit();			//初始化GPIO(作为普通IO使用,方向为输出)
	USARTA_Init(57600);
//	Prints("hello wrold!");
//	Prints("USART Test Begin:\r\n");
//	Prints("Send DSP Some data,It Will Send Them Bcak!\r\n");
	TIM0_Init(60,500);//定时器初始化--500us
	Flash_Modbus_ADDR = 0x0001;
	Flash_Usart_BaudRate = 0x0007;
	LED_ON;
	Delay_nMS(1000);
	LED_OFF;
	while(1)
	{
		MODBUS_event();
	}
}
