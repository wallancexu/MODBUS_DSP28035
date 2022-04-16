#include "Delay.h"

//Delay_Init()
//{
//
//}
void Delay_nMS(Uint16 nms)
{
	Uint16 i = 0;
	for(i = 0; i < nms; i ++)
	{
		Delay_nUS(1000);
	}
}

