#ifndef USART_H_H_
#define USART_H_H_

#include "../../OnChip_Driver/OnChip_Driver_Inc.h"

#define USART1_RX_LENGTH_MAX 200

extern unsigned int 	usart1_rx_length;			//COM1_RxBuff[Index]
extern Uint16	COM1_RxBuff[USART1_RX_LENGTH_MAX];
extern Uint16 	COM1_TxBuff[USART1_RX_LENGTH_MAX];
extern Uint32  Gu32_modbus_outputIO[USART1_RX_LENGTH_MAX];
extern Uint16  Gu16_modbus_bits_outputIO[USART1_RX_LENGTH_MAX];
extern unsigned int 	time_usart1;

void USARTA_Init(Uint32 buad);
void USART_Transmit(Uint16 data);
void tscia_fifo_init1(void);
void scia_xmit(int a);
void scia_msg(char *msg);


Uint16 USART_GetChar(Uint16* p_array, Uint16 frame_len);
Uint16 USART_GetRate(int32* p_rate);

//数字或字符打印函数
void PrintHex(Uint16 x);		
void PrintShortIntHex(Uint16 x);
void PrintLongInt(Uint32 x);
void Prints(const void * pd);

//void USART1_SendByte(Uint16 a);
void Usart1_Send_String(Uint16 *buff,Uint16 len);
void USART1_SendString(char *msg);
void USART_SendString(char *p_STR);
void USART_SendLine(int32 Data ,
					Uint16 Is_H, 
				    Uint16 Line_ID);
void USART_Debug(Uint16 debug_data);
#endif

