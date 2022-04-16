/*
 * modbus.c
 *
 *  Created on: 2022年3月19日
 *      Author: Administrator
 */
#include "../MyBSP/MyBspInc.h"
#include "../OnChip_Driver/OnChip_Driver_Inc.h"

/*******************************************************************************
									宏定义
 ********************************************************************************/
#define MODBUS_LENGTH 100 //Usart.h
#define ADDR 1
#define NULL 0
/*******************************************************************************
									变量声明区
 ********************************************************************************/
//Uint32  Gu32_modbus_outputIO[USART1_RX_LENGTH_MAX];
//Uint16  Gu16_modbus_bits_outputIO[USART1_RX_LENGTH_MAX];
Uint32 MOSBUS_BaudRate[10]={4800,9600,14400,19200,38400,56000,57600,115200,128000,256000};
Uint16 Flash_Usart_BaudRate;
Uint16 Flash_Modbus_ADDR;		//modbusid
unsigned short modbus_reg[MODBUS_LENGTH];

unsigned int UsartNUM;
unsigned int Calculate_FLAG;
const unsigned char auchCRCHi[256] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
const unsigned  char auchCRCLo[256] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,0x04,
0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,0x40
} ;
//函数入口
/*******************************************************************************
									crc校验值计算
 ********************************************************************************/
unsigned int CRC_Calculate(Uint16 *str,unsigned int usDataLen)
{
	unsigned char uchCRCHi = 0xFF ; 	/* high byte of CRC initialized */
	unsigned char uchCRCLo = 0xFF ; 	/* low byte of CRC initialized */
    unsigned uIndex ; 							/* will index into CRC lookup table */
    while (usDataLen--)							/* pass through message buffer */
    {
      uIndex = uchCRCHi ^ *str++ ; /* calculate the CRC */
      uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
      uchCRCLo = auchCRCLo[uIndex] ;
    }
	return (uchCRCHi << 8 | uchCRCLo) ;
}
static Uint16 get_crc16(Uint16 *pbuff, Uint16 len)   //
{

	Uint16 i;
	Uint16 ret1 = 0;
	Uint16 crc = 0xFFFF;

	if (len == 0)
	{
		len = 1;
	}
	while (len--)
	{
		crc ^= *pbuff;
		for (i = 0; i<8; i++)
		{
			if (crc & 1)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
		pbuff++;
	}
	ret1 = crc >> 8;
	ret1 = ret1 | (crc << 8);
	return(ret1);
}

/******************************************************************************
							modbus发送函数
*******************************************************************************/
void MODBUS_Send(Uint16 *buf,unsigned int length)///??length???
{
	//从机1
	if(UsartNUM==1)
		Usart1_Send_String(buf,length);
//	//从机2
//	if(UsartNUM==2)
//	{
//		Usart2_SendString(buf,length);
//	}
}

/*******************************************************************************
									modbus执行函数
 ********************************************************************************/
void MODBUS(Uint16 *buf, unsigned int length, unsigned int NUM)//MODBUS(usart1_rx_buf,usart1_rx_length,1);
{
	static unsigned int startaddr,number,code;	//干啥用的(起始地址、数据长度，功能码)
	static unsigned int crc;					//校验码
	UsartNUM = NUM;
	crc = CRC_Calculate(buf,length-2);	//
	if (buf[0] == Flash_Modbus_ADDR&&buf[length-2] == (crc / 256)&&buf[length-1] == (crc % 256))//地址位正确
	{
		unsigned int Write_EN = 0;
		MODBUS_datain();				//代码数据输入
		switch(buf[1])
		{
			case 0x01:
				modbus_01_solve();
				break;
			case 0x03:
				modbus_03_solve();
				startaddr = buf[2]*256  + buf[3];
				number = buf[4]*256 + buf[5];
			//				Prints("I'm here:1\r\n");
				if((startaddr+number) > MODBUS_LENGTH) //
				{
			//					Prints("I'm here:2\r\n");
					break;
				}
//				MODBUS_03_Return(COM1_TxBuff,number);
				break;
			case 0x06:
				modbus_06_solve();
			    startaddr = buf[2]*256 + buf[3];
			    code = buf[4]*256 + buf[5];
				if ((startaddr) > MODBUS_LENGTH)
				{
					break;
				}
				if(((buf[2]<<8|buf[3])== 0x0001)&&((buf[4]<<8|buf[5]) == 0x0100))	//灯亮01 06 00 01 01 00 D9 9A
				{
					LED_ON;
			//					Prints("I'm here:1\r\n");
				}
				if(((buf[2]<<8|buf[3])== 0x0001)&&(buf[4] == 0x06))					//延时01 06 00 01 06 0A 5B AD 延时10
				{
					LED_ON;
					Delay_nMS((Uint16)buf[5]*10);	//u8未定义，改为u16了
					LED_OFF;
					Delay_nMS(100);
					LED_ON;
					Delay_nMS(100);
					LED_OFF;
			//					Prints("I'm here:2\r\n");
				}
				if(((buf[2]<<8|buf[3])== 0x0001)&&((buf[4]<<8|buf[5]) == 0x0200))	//灯灭01 06 00 01 02 00 D9 6A																							//01 06 00 01 02 00 D9 6A
				{
					LED_OFF;
			//					Prints("I'm here:3\r\n");
				}
			    modbus_reg[startaddr] = code;
			    MODBUS_Send(COM1_TxBuff,length);//
			    Write_EN = 1;
				break;
            case 0x05:      //功能码05
            	modbus_05_solve();
			    startaddr = buf[2]*256 + buf[3];
			    code = buf[4]*256 + buf[5];
				if ((startaddr) > MODBUS_LENGTH)
				{
					break;
				}
				if(((buf[2]<<8|buf[3])== 0x0001)&&((buf[4]<<8|buf[5]) == 0xFF00))//01 05 00 01 FF 00 DD FA
				{
					LED_ON;
				}
				if(((buf[2]<<8|buf[3])== 0x0001)&&((buf[4]<<8|buf[5]) == 0x0000))//01 05 00 01 00 00 9C 0A
				{
					LED_OFF;
				}
			    modbus_reg[startaddr] = code;
			    MODBUS_Send(COM1_TxBuff,length);
				Write_EN = 1;
				break;
			case 0x10:
			    startaddr = buf[2]*256 + buf[3];
			    code = buf[4]*256 + buf[5];
				if ((startaddr) > MODBUS_LENGTH)
				{
					break;
				}
				if(((buf[2]<<8|buf[3])== 0x0001)&&((buf[4]<<8|buf[5]) == 0x0100))	//灯亮01 06 00 01 01 00 D9 9A
				{
					LED_ON;
			//					Prints("I'm here:1\r\n");
				}
				if(((buf[2]<<8|buf[3])== 0x0001)&&(buf[4] == 0x06))					//延时01 06 00 01 06 0A 5B AD 延时10
				{
					LED_ON;
					Delay_nMS((Uint16)buf[5]*10);	//u8未定义，改为u16了
					LED_OFF;
					Delay_nMS(100);
					LED_ON;
					Delay_nMS(100);
					LED_OFF;
			//					Prints("I'm here:2\r\n");
				}
				if(((buf[2]<<8|buf[3])== 0x0001)&&((buf[4]<<8|buf[5]) == 0x0200))	//灯灭01 06 00 01 02 00 D9 6A																							//01 06 00 01 02 00 D9 6A
				{
					LED_OFF;
			//					Prints("I'm here:3\r\n");
				}
			    modbus_reg[startaddr] = code;
			    MODBUS_Send(buf,length);//
			    Write_EN = 1;

				break;
			default:
				break;
		}
	}
}

/******************************************************************************
								MODBUS 01功能码
*******************************************************************************/
static void modbus_01_solve(void)
{
//	Prints("I'm here");
	static Uint16 bit_num = 0;
	static Uint32 reg_start_addr = 0;
	static Uint16 i;
	static Uint16 crc_temp = 0;

	reg_start_addr = (((Uint32)COM1_RxBuff[2]<<16)|COM1_RxBuff[3]);
	bit_num =(((Uint32)COM1_RxBuff[4]<<16)|COM1_RxBuff[5])/0x0008;//
//	MODBUS_Send(bit_num,2);
//	MODBUS_Send(COM1_RxBuff[2],1);
//	MODBUS_Send(COM1_RxBuff[3],1);
//	MODBUS_Send(reg_start_addr,1);

//	Prints("\r\n   \r\n");

	if((reg_start_addr + bit_num) > REG_LAST_ADDR) //寄存器地址加数量超出范围
	{
		COM1_TxBuff[0] = COM1_RxBuff[0];      //ID
		COM1_TxBuff[1] = COM1_RxBuff[1];      //
		COM1_TxBuff[2] = 0x02;                //
		MODBUS_Send(COM1_TxBuff, 3);
//    Prints("I'm here");

		return;
	}


	COM1_TxBuff[0] = COM1_RxBuff[0];      //ID
	COM1_TxBuff[1] = COM1_RxBuff[1];      //
	COM1_TxBuff[2] = bit_num;           //
	COM1_TxBuff[3] = Gu16_modbus_bits_outputIO[reg_start_addr+1];
//	for(i = 0; i < 1; i++)				//bit_num
//	{
//		COM1_TxBuff[3 + i] = ( Gu16_modbus_bits_outputIO[reg_start_addr + i]);//256--左移8位
////		COM1_TxBuff[4 + i*2] = ( Gu32_modbus_outputIO[reg_start_addr + i] % 65536 );
//	}

	crc_temp = get_crc16( COM1_TxBuff, bit_num+3 );

	COM1_TxBuff[bit_num+3] = (crc_temp>>8) & 0xFF;
	COM1_TxBuff[bit_num+4] =  crc_temp & 0xFF;
	MODBUS_Send(COM1_TxBuff,bit_num+5);//bit_num+5
}

/******************************************************************************
								MODBUS 03功能码
*******************************************************************************/
static void modbus_03_solve(void)
{
	static Uint32 reg_num = 0;
	static Uint32 reg_start_addr = 0;
	static Uint16  i;
	static Uint16 crc_temp = 0;

	reg_start_addr = ((Uint32)COM1_RxBuff[2]<<16)|COM1_RxBuff[3];
	reg_num = ((Uint32)COM1_RxBuff[4]<<16)|COM1_RxBuff[5];

	if((reg_start_addr + reg_num) > REG_LAST_ADDR)  //寄存器地址加数量超出范围
	{
		COM1_TxBuff[0] = COM1_RxBuff[0];      //ID
		COM1_TxBuff[1] = COM1_RxBuff[1];     //
		COM1_TxBuff[2] = 0x02;                    //
    MODBUS_Send(COM1_TxBuff, 3);
		return;
	}


	COM1_TxBuff[0] = COM1_RxBuff[0];      //ID
	COM1_TxBuff[1] = COM1_RxBuff[1];     //
	COM1_TxBuff[2] = reg_num*2;               //
	for(i = 0; i < (reg_num*2); i++)
	{
		COM1_TxBuff[3 + i*2] = ( Gu32_modbus_outputIO[reg_start_addr + i] / 65536 );//256--左移8位
		COM1_TxBuff[4 + i*2] = ( Gu32_modbus_outputIO[reg_start_addr + i] % 65536 );
	}

	crc_temp = get_crc16( COM1_TxBuff, reg_num*2+3 );

	COM1_TxBuff[reg_num*2+3] = (crc_temp>>8) & 0xFF;
	COM1_TxBuff[reg_num*2+4] =  crc_temp & 0xFF;
	MODBUS_Send(COM1_TxBuff,reg_num*2+5);
}

/******************************************************************************
								MODBUS 05功能码
*******************************************************************************/
static void modbus_05_solve(void)
{
	static Uint32 reg_start_addr = 0;
	static Uint16 crc_temp = 0;
	static Uint32 reg_num = 0;

	reg_start_addr = ((Uint32)COM1_RxBuff[2]<<16)|COM1_RxBuff[3];//0x0001
	Gu32_modbus_outputIO[reg_start_addr]=((Uint32)COM1_RxBuff[4]<<16)|COM1_RxBuff[5];//高字节在前 Gu32_modbus_outputIO[0x0001]=ff00
//	Gu32_modbus_outputIO[reg_start_addr]|=((Uint32)COM1_RxBuff[5]);//低字节在后
	Gu16_modbus_bits_outputIO[reg_start_addr]=0x00;
	if((( Gu32_modbus_outputIO[reg_start_addr] / 65536 )==0xFF) && (Gu32_modbus_outputIO[reg_start_addr] % 65536==0x00))//Gu32_modbus_outputIO[reg_start_addr] = 0xFF00
	{
		Gu16_modbus_bits_outputIO[reg_start_addr]=0x02;
	}
	else if((( Gu32_modbus_outputIO[reg_start_addr] / 65536 )==0x00) && (Gu32_modbus_outputIO[reg_start_addr] % 65536==0x00))//Gu32_modbus_outputIO[reg_start_addr]= 0x0000
	{
		Gu16_modbus_bits_outputIO[reg_start_addr]=0x00;
	}
	else
		return;
	COM1_TxBuff[0]=COM1_RxBuff[0];
	COM1_TxBuff[1]=COM1_RxBuff[1];
	COM1_TxBuff[2]=COM1_RxBuff[2];
	COM1_TxBuff[3]=COM1_RxBuff[3];
	COM1_TxBuff[4]=COM1_RxBuff[4];
	COM1_TxBuff[5]=COM1_RxBuff[5];
//	COM1_TxBuff[4] = ( Gu32_modbus_outputIO[reg_start_addr] / 65536 );//256--左移8位
//	COM1_TxBuff[5] = ( Gu32_modbus_outputIO[reg_start_addr] % 65536 );
//	COM1_TxBuff[4] = Gu16_modbus_bits_outputIO[reg_start_addr];
//	COM1_TxBuff[5] = Gu16_modbus_bits_outputIO[reg_start_addr];

	crc_temp=get_crc16(COM1_RxBuff,6);
	COM1_TxBuff[6]=(crc_temp>>8)&0xFF;
	COM1_TxBuff[7]=(crc_temp)&0xFF;
//	MODBUS_Send(buf,reg_num+5);
}

/******************************************************************************
								MODBUS 06功能码
*******************************************************************************/
static void modbus_06_solve(void)
{
	static Uint32 reg_start_addr = 0;
	static Uint16 crc_temp = 0;

	reg_start_addr = ((Uint32)COM1_RxBuff[2]<<16)|COM1_RxBuff[3];
	Gu32_modbus_outputIO[reg_start_addr]=((Uint32)COM1_RxBuff[4]<<16)|COM1_RxBuff[5];//高字节在前
//	Gu32_modbus_outputIO[reg_start_addr]|=((Uint32)COM1_RxBuff[5]);//低字节在后

	COM1_TxBuff[0]=COM1_RxBuff[0];
	COM1_TxBuff[1]=COM1_RxBuff[1];
	COM1_TxBuff[2]=COM1_RxBuff[2];
	COM1_TxBuff[3]=COM1_RxBuff[3];
	COM1_TxBuff[4]=COM1_RxBuff[4];
	COM1_TxBuff[5]=COM1_RxBuff[5];
	COM1_TxBuff[4] = ( Gu32_modbus_outputIO[reg_start_addr] / 65536 );//256--左移8位
	COM1_TxBuff[5] = ( Gu32_modbus_outputIO[reg_start_addr] % 65536 );

	crc_temp=get_crc16(COM1_RxBuff,6);
	COM1_TxBuff[6]=(crc_temp>>8)&0xFF;
	COM1_TxBuff[7]=(crc_temp)&0xFF;
}

/******************************************************************************
								MODBUS 10功能码
*******************************************************************************/
//static void modhus_10_solve(void)
//{
//	startaddr = buf[2]*256 + buf[3];
//	number = buf[4]*256 + buf[5];
//	if ((startaddr+number) > MODBUS_LENGTH)
//	{
//		break;
//	}
//	for (code = 0;code < number;code++)
//	{
//		modbus_reg[startaddr + code] = buf[6+code*2]*256 + buf[7+code*2];
//	}
//	MODBUS_10_Return(startaddr,number);
////							SYS_DEBUG_AP_FLAG=10;
//
//}

/******************************************************************************
								MODBUS 入口
*******************************************************************************/
void MODBUS_event()
{
//	Prints(usart1_rx_length);
//	Prints(time_usart1);
//	Delay_nMS(1000);
	if(usart1_rx_length&&(time_usart1 >= (10-Flash_Usart_BaudRate)*5))//
	{
		MODBUS(COM1_RxBuff,usart1_rx_length,1);
		usart1_rx_length = 0;
		time_usart1 = 0;
	}
}

/******************************************************************************
								MODBUS 信号
*******************************************************************************/
void MODBUS_datain()
{
	modbus_reg[0]=Flash_Usart_BaudRate;///0x0007
	modbus_reg[1]=Flash_Modbus_ADDR;  ///Modbus ID?
	modbus_reg[2]=0x1111;
	modbus_reg[3]=0x2222;
	modbus_reg[4]=0x3333;
	///.....
	///modbus_reg[n]=.....;
}

/******************************************************************************
						    	03功能码数据回传
*******************************************************************************/
void MODBUS_03_Return(unsigned int startaddr,unsigned int number)
{
    unsigned char send_length;
    unsigned char send_buf[205];
    unsigned char i;
    unsigned int crc;

    send_length = 0;
    send_buf[send_length++] = Flash_Modbus_ADDR;
    send_buf[send_length++] = 0x03;        //03???
    send_buf[send_length++] = number*2;	  //
    for (i = 0;i < number;i++)
    {
        send_buf[send_length++] = modbus_reg[startaddr+i]/256;
        send_buf[send_length++] = modbus_reg[startaddr+i]%256;
    }
    crc = CRC_Calculate(send_buf,send_length);
    send_buf[send_length++] = crc/256;
    send_buf[send_length++] = crc%256;
    MODBUS_Send(send_buf,send_length);
}

///******************************************************************************
//							MODBUS10功能码数据回传
//*******************************************************************************/
void MODBUS_10_Return(unsigned int addr,unsigned int code)
{
    unsigned char send_buf[8];
	unsigned int crc;
	send_buf[0] = Flash_Modbus_ADDR;
	send_buf[1] = 16;
	send_buf[2] = (addr / 256);
	send_buf[3] = (addr % 256);
	send_buf[4] = (code / 256);
	send_buf[5] = (code % 256);
	crc = CRC_Calculate(send_buf,6);
	send_buf[6] = (crc / 256);
    send_buf[7] = (crc % 256);
	MODBUS_Send(send_buf,8);
}
