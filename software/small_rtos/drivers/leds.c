/*********************************************************************************************************
**				                               Small RTOS(51)
**                                   The Real-Time Kernel(For Keil c51)
**
**                                  (c) Copyright 2002-2003, chenmingji
**                                           All Rights Reserved
**
**                                                  V1.20.0
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: leds.c
**��   ��   ��: cole3
**����޸�����: 2003��8��3��
**��        ��: ext1Դ���롣
**
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: cole3
** ��  ��: V0.50
** �ա���: 2013��4��6��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#include "leds.h"

static int8 leds_open(uint8 id)
{
	UNUSED(id);
	ENLED = 0;
	ADDR3 = 1;
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;

	return 0;
}

static void leds_close(uint8 id)
{
	UNUSED(id);
	ENLED = 0;
	ADDR3 = 0;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
}

static int8 leds_ioctl(uint8 id, uint8 cmd, uint16 args)
{
	uint8 *para = (uint8 *)args;

	if (GET_CMD_DIR(cmd) == CMD_SET)
	{
		switch (cmd)
		{
			case LED_ON:
				LED_PORT &= ~(1<<id);
				break;
			case LED_OFF:
				LED_PORT |= (1<<id);
				break;
			case LED_TRIGGER:
				LED_PORT ^= (1<<id);
				break;
			default: 
				return -1;
		}
	}
	else // CMD_GET
	{
		if (para == NULL)
		{
			return -2;
		}
		switch (cmd)
		{
			case LED_STATE:
				if (LED_PORT & (1<<id))
				{
					*para = LED_OFF;
				}
				else
				{
					*para = LED_ON;
				}
				break;
			default: 
				return -1;
		}
	}

	return 0;
}


const struct file_operations leds_fops = {
	leds_open,
	leds_close,
	leds_ioctl
};

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
