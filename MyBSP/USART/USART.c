#include "USART.h"
#include "time.h"
//串口的波特率是由SCIHBAUD、SCILBAUD组成的一个16位寄存器BRR和LSPCLK低速外设时钟共同决定的
//它们的关系是:SCI Buad(波特率) = LSPCLK / [(BRR +1)*8],反推之即:BRR = (LSPCLK/Buad*8) - 1
//同时要注意,如果BRR = 0,那么Buad = LSPCLK/16.
//其中LSPCLK是主时钟分频得到的,本示例中主时钟为60M,低速时钟分频寄存器为0x02,对应4分频,
//即LSPCLK为60/4=15MHZ(请查阅InitSysCtrl()函数了解相关时钟寄存器的配置过程)

//TX_INT_EN为1使能发送中断,为0不使能发送中断
#define TX_INT_EN			0

interrupt void USARTA_RxIntHandler(void);		//接收中断中断函数
#if TX_INT_EN 		> 0
	interrupt void USARTA_TxIntHandler(void);	//发送中断中断函数
#endif

#define USART1_RX_LENGTH_MAX 200

//中断接收用的标志和缓存变量定义
unsigned int	usart1_rx_length = 0;
unsigned int	Frame_Length = 0;
Uint16 	COM1_RxBuff[USART1_RX_LENGTH_MAX]={0};//Uint16 COM1_RxBuff[15]={0}
Uint16 	COM1_TxBuff[USART1_RX_LENGTH_MAX]={0};//int8（f103）没有八位寄存器
Uint32  Gu32_modbus_outputIO[USART1_RX_LENGTH_MAX];
Uint16  Gu16_modbus_bits_outputIO[USART1_RX_LENGTH_MAX];


unsigned int 	time_usart1,time_usart2;

/*******************************************************************************
									串口接收中断
 ********************************************************************************/
interrupt void USARTA_RxIntHandler(void)
{
	if((SciaRegs.SCIRXST.bit.RXRDY) == 1)
	{
		time_usart1 = 0;
		COM1_RxBuff[usart1_rx_length] = SciaRegs.SCIRXBUF.all;
		if(usart1_rx_length < USART1_RX_LENGTH_MAX)//Frame_Length
		{
			usart1_rx_length ++;
		}
	}
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;       	// Issue PIE ack
}
#if TX_INT_EN 		> 0
//如果使能了发送中断,该中断服务函数需要根据需要添加用户代码
interrupt void USARTA_TxIntHandler(void)
{   
    Uint16 data = 0xAA;
   
 	SciaRegs.SCITXBUF = data;  				//模拟发送一个数据
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;       	// Issue PIE ack
}
#endif
static void USARTA_IntHandlerConfig(void)
{
	EALLOW;
   	PieVectTable.SCIRXINTA= &USARTA_RxIntHandler;
#if TX_INT_EN 		> 0
   	PieVectTable.SCITXINTA = &USARTA_TxIntHandler;
#endif
   	EDIS;
}

static void USARTA_CpuIntEn(void)
{
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   	// Enable the PIE block
   	PieCtrlRegs.PIEIER9.bit.INTx1=1;     	// PIE Group 9, INT1
#if TX_INT_EN 		> 0
   	PieCtrlRegs.PIEIER9.bit.INTx2=1;     	// PIE Group 9, INT2
#endif
	IER |= M_INT9;							// Enable CPU INT
   	EINT;  									//使能全局中断
}

//Initalize the SCI FIFO
void USARTA_Init(Uint32 buad)
{
	//注意要在函数InitSysCtrl()中开启SCIA外设时钟
	Uint16 brr_reg = (1875000/buad) - 1;	//15000000/8 = 1875000
//	SciaRegs.SCIFFTX.all =0xE040;
//	SciaRegs.SCIFFRX.all =0x204f;
//	SciaRegs.SCIFFCT.all =0x0;
 	InitSciaGpio();							//USARTA使用端口初始化
 	USARTA_IntHandlerConfig();				//PIE中断函数配置
	
 	SciaRegs.SCICCR.all =0x0007;   			// 1 stop bit,  No loopback
                                   			// No parity,8 char bits,
	                               			// async mode, idle-line protocol
	SciaRegs.SCICTL1.all =0x0003;  			// enable TX, RX, internal SCICLK,
                                   			// Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCICTL2.all =0x0003;
#if TX_INT_EN 		> 0
	SciaRegs.SCICTL2.bit.TXINTENA =1;		//使能发送中断
#else
	SciaRegs.SCICTL2.bit.TXINTENA =0;		//禁止发送中断
#endif
	SciaRegs.SCICTL2.bit.RXBKINTENA =1;		//使能接收中断

	SciaRegs.SCIHBAUD    =(brr_reg>>8)&0x00FF;
    SciaRegs.SCILBAUD    =brr_reg&0x00FF;

	SciaRegs.SCICTL1.all =0x0023;  			// SCI退出复位状态

	USARTA_CpuIntEn();						//使能PIE中断
	// SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	SciaRegs.SCICTL1.all =0x0023; // Relinquish SCI from Reset
}

void USART1_SendString(char *msg)
{
	int i=0;
	while(msg[i] != '\0')
	{
	USART_Transmit(msg[i]);
	i++;
	}
}
//查询方式发送一个字节
void USART_Transmit(Uint16 data)
{
	while (SciaRegs.SCICTL2.bit.TXRDY == 0) 
	{	//如果SCITXBUF没有准备好接收新的数据则一直等待直到标志置1
		;		
	}
    SciaRegs.SCITXBUF = data;
}

//////////////接收///////////////////
Uint16 USART_GetChar(Uint16* p_array, Uint16 frame_len)
{
	Uint16 i = 0;
	if(Frame_Length != frame_len)
	{
		Frame_Length = frame_len;
	}
	if(usart1_rx_length == Frame_Length)
	{
		for(i=0; i< Frame_Length; i++)
		{
			p_array[i] = COM1_RxBuff[i];
		}
		usart1_rx_length = 0;
		return 1;
	}
	return 0;
}
Uint16 USART_GetRate(int32* p_rate)
{
	Uint16 temp_array[9] = {0};
	Uint16 i =0;										
	int32 temp_rate = 0;
	Uint16 ch_cmd = 0;						
	if(USART_GetChar(temp_array, 9))		//如果收到新数据才进行数据校验与恢复
	{										//这样提高了查询速度				
		if((temp_array[0] == 0xaa)&&(temp_array[8] == 0x55))
		{
			for(i = 3; i < 7; i ++)
			{
				temp_rate |= temp_array[i]<<((i-3)<<3);//8*(i-3)				
			}
			ch_cmd = temp_array[2];			//获取命令位						
			*p_rate = temp_rate;			//获取数据位						
		}
	}
	return ch_cmd;
}

void Usart1_Send_String(Uint16 *buff,Uint16 len)
{
	Uint16 t;
	for(t=0;t<len;t++)
	{
		while((SciaRegs.SCIRXST.bit.RXRDY) == 1);
		USART_Transmit(buff[t]);
	}
}

void USART_SendString(char *p_STR)
{
	while(*p_STR)
	{
		USART_Transmit(*p_STR ++);
	}
}
void USART_Debug(Uint16 debug_data)
{
	USART_Transmit(0xaa);
	USART_Transmit(debug_data);
	USART_Transmit(0x55);
}
void USART_SendLine(int32 Data ,
					Uint16 Is_H, 
				    Uint16 Line_ID)
{
	Uint16 Temp_A_L;				
	Uint16 Temp_A_H;
	Uint16 Temp_B_L;
	int16 Temp_B_H;
	
	Uint16 ch_XOR = 0;
	
	Temp_A_L = Data;
	Temp_A_H = Data>>8;
	Temp_B_L = Data >> 16;
	Temp_B_H = Data >> 24;
	
	ch_XOR ^= 0x09;
	ch_XOR ^= Line_ID;
	
	ch_XOR ^= Temp_B_H;
	ch_XOR ^= Temp_B_L;
	ch_XOR ^= Temp_A_H;
	ch_XOR ^= Temp_A_L;
	
	USART_Transmit(0xaa);
	USART_Transmit(0x09);
	USART_Transmit(Line_ID);
	
	if(Is_H)							//是否是先发高字节？					
	{
		USART_Transmit(Temp_B_H);		//是									
		USART_Transmit(Temp_B_L);
		USART_Transmit(Temp_A_H);
		USART_Transmit(Temp_A_L);
	}
	else
	{
		USART_Transmit(Temp_A_L);		//不是									
		USART_Transmit(Temp_A_H);
		USART_Transmit(Temp_B_L);
		USART_Transmit(Temp_B_H);
	}
	
	USART_Transmit(ch_XOR);
	USART_Transmit(0x55);
}

//////////////发送///////////////////
#define PRINT_HEX_EN		1

#if PRINT_HEX_EN		> 0
/////////////////////////////////////////////////////////////////////
//调试用串口打印函数
# define UartPutChar	USART_Transmit
# define Printc			USART_Transmit

/********************************************************************
函数功能：发送一个字符串。
入口参数：pd：要发送的字符串指针。
返    回：无。
备    注：无。
********************************************************************/
void Prints(const void * pstr)
{
	Uint16* pd = (Uint16*)pstr;
	while((*pd)!='\0') //发送字符串，直到遇到0才结束
	{
		UartPutChar(*pd); //发送一个字符
		pd++;  //移动到下一个字符
	}
}
////////////////////////End of function//////////////////////////////
/********************************************************************
函数功能：将整数转按十进制字符串发送。
入口参数：x：待显示的整数。
返    回：无。
备    注：无。
********************************************************************/
void PrintLongInt(Uint32 x)
{
 	int16 i;
 	Uint16 display_buffer[10];

	for(i=9;i>=0;i--)
	{
		display_buffer[i]='0'+x%10;
		x/=10;
	}
	for(i=0;i<9;i++)
	{
		if(display_buffer[i]!='0')
		{
			break;	
		}
	}
	for(;i<10;i++)
	{
		UartPutChar(display_buffer[i]);	
	}
}
////////////////////////End of function//////////////////////////////
const Uint16 HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
/********************************************************************
函数功能：将短整数按十六进制发送。
入口参数：待发送的整数。
返    回：无。
备    注：无。
********************************************************************/
void PrintShortIntHex(Uint16 x)
{
 	Uint16 i;
 	Uint16 display_buffer[7];
 	display_buffer[6]=0;
 	display_buffer[0]='0';
 	display_buffer[1]='x';
 	for(i=5;i>=2;i--) //将整数转换为4个字节的HEX值
 	{
	  	display_buffer[i]=HexTable[(x&0xf)];
	  	x>>=4;
 	}
 	Prints(display_buffer);
}

/********************************************************************
函数功能：以HEX格式发送一个byte的数据。
入口参数：待发送的数据
返    回：无。
备    注：无。
********************************************************************/
void PrintHex(Uint16 x)
{
	Printc('0');
	Printc('x');
	Printc(HexTable[x>>4]);
	Printc(HexTable[x&0xf]);
	Printc(' ');
}
#endif

//===========================================================================
// No more.
//===========================================================================
