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
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: keys.c
**创   建   人: cole3
**最后修改日期: 2003年8月3日
**描        述: ext1源代码。
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: cole3
** 版  本: V0.50
** 日　期: 2013年4月6日
** 描　述: 原始版本
********************************************************************************************************/
#include "keys.h"




static K_MAP code k_map[] = {
//	 type			raw_val	ir_val
	{ONE, 			0x01,	0x00000001},
	{TWO, 			0x02,	0x00000002},
	{THREE, 		0x03,	0x00000003},
	{MAX_K_TYPE,	0,		0}
};

static uint32 k_val;
#if 0
static K_TYPE keys_raw_find_type(uint8 val)
{
	K_MAP *p = k_map;
	
	while (p->type != MAX_K_TYPE)
	{
		if (p->raw_val == val)
		{
			return p->type;
		}
		p++;
	}
}

static K_TYPE keys_ir_find_type(uint32 val)
{
	K_MAP *p = k_map;
	
	while (p->type != MAX_K_TYPE)
	{
		if (p->ir_val == val)
		{
			return p->type;
		}
		p++;
	}
}
#endif
static uint8 keys_raw_get_value(void)
{
	return KEY_RAW_PORT;
}

static uint8 keys_ir_get_low_cnt(uint8 timeout)
{
	uint8 cnt;

	for (cnt=0; !KEY_IR_PORT && cnt<timeout; cnt++)
	{
		udelay(100);
	}
	
	return cnt;
}

static uint8 keys_ir_get_high_cnt(uint8 timeout)
{
	uint8 cnt;

	for (cnt=0; KEY_IR_PORT && cnt<timeout; cnt++)
	{
		udelay(100);
	}
	
	return cnt;
}

static uint32 keys_ir_get_value(void)
{
	uint8 cnt;
	uint8 i;
	uint32 val = 0;
	
	cnt = keys_ir_get_low_cnt(95);
	if ((cnt <= 85) || (cnt >= 95))  // low level 8500~9500us
		return 0;
	cnt = keys_ir_get_high_cnt(50);
	if ((cnt <= 40) || (cnt >= 50))  // high level 4000~5000us
		return 0;

	for (i=0; i<32; i++) //32 bit
	{
		cnt = keys_ir_get_low_cnt(80);
		if ((cnt <= 20) || (cnt >= 80)) // low level 200~800us
			return 0;

		cnt = keys_ir_get_high_cnt(200);
		if ((cnt <= 20) || (cnt >= 200)) // high level 200~2000us
			return 0;

		val >>= 1;
		if (cnt > 11) // 1120us
			val |= 0x80000000;
	}

	return val;
}

static void keys_raw_io_init(uint8 id)
{
	UNUSED(id);
}

static void keys_ir_io_init(uint8 id)
{
	UNUSED(id);
}




static int8 keys_open(uint8 id)
{
	UNUSED(id);
	keys_raw_io_init(id);
	keys_ir_io_init(id);
	
	OSSemCreate(KEYSEM, 0);
	
	return 0;
}

static void keys_close(uint8 id)
{
	UNUSED(id);
}

static int8 keys_ioctl(uint8 id, uint8 cmd, uint16 args)
{
	UNUSED(id);
	
	if (GET_CMD_DIR(cmd) == CMD_GET)
	{
		switch (cmd)
		{
			case KEY_PEND:
				OSSemPend(KEYSEM, 0);
			case KEY_GET:
				*(uint8 *)args = k_val;
				k_val = 0;
				break;
			default: 
				break;
		}
	}

	return 0;
}

#pragma disable                                        /* 除非最高优先级中断，否则，必须加上这一句                 */
void key_raw_irq(void) interrupt 0
{
    OS_INT_ENTER();
	k_val = keys_raw_get_value();
	OSSemPost(KEYSEM);
    OSIntExit();
}

#pragma disable                                        /* 除非最高优先级中断，否则，必须加上这一句                 */
void key_ir_irq(void) interrupt 2
{
    OS_INT_ENTER();
	k_val = keys_ir_get_value();
	OSSemPost(KEYSEM);
    OSIntExit();
}


const struct file_operations keys_fops = {
	keys_open,
	keys_close,
	keys_ioctl
};

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
