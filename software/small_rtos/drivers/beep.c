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
**��   ��   ��: beep.c
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
#include "beep.h"


struct beep_dev {
	uint8 freq;
	uint8 pin;
	uint8 on;
};

static struct beep_dev beep_dev[MAX_BEEP];


#ifdef PWM_PIN_ENABLE
static bool beep_IO_init(uint8 id)
{
	return TRUE;
}

static bool beep_set_freq(uint8 id, uint8 freq)
{
	return TRUE;
}

static bool beep_output(uint8 id, bool on)
{
	return TRUE;
}
#else
static bool beep_IO_init(uint8 id)
{
	UNUSED(id);
	return TRUE;
}

static bool beep_set_freq(uint8 id, uint8 freq)
{
	UNUSED(id);
	UNUSED(freq);
	return TRUE;
}

static bool beep_output(uint8 id, bool on)
{
	UNUSED(id);
	UNUSED(on);
	return TRUE;
}
#endif

static int8 beep_open(uint8 id)
{
	if (id >= MAX_BEEP)
	{
		return -1;
	}
	
	if (!beep_IO_init(id))
	{
		return -2;
	}

	beep_dev[id].freq = 0;
	beep_dev[id].on = FALSE;

	return 0;
}

static void beep_close(uint8 id)
{
	if (id >= MAX_BEEP)
	{
		return;
	}

	beep_open(id);

	beep_dev[id].freq = 0;
	beep_dev[id].on = FALSE;
}

static int8 beep_ioctl(uint8 id, uint8 cmd, uint16 args)
{
	uint8 *para = (uint8 *)args;

	if (GET_CMD_DIR(cmd) == CMD_SET)
	{
		switch (cmd)
		{
			case BEEP_SET_FREQ:
				if (!beep_set_freq(id, *para))
				{
					return -2;
				}
				beep_dev[id].freq = *para;
				break;
			case BEEP_OUTPUT:
				if(!beep_output(id, (bool)*para))
				{
					return -2;
				}
				beep_dev[id].on = (bool)*para;
				break;
			case BEEP_MODE:
				
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
			case BEEP_GET_STATE:
				*para = (uint8)beep_dev[id].on;
				break;
			default: 
				return -1;
		}
	}
	
	return 0;
}


const struct file_operations beep_fops = {
	beep_open,
	beep_close,
	beep_ioctl
};

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
