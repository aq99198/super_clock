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
**��   ��   ��: core.c
**��   ��   ��: cole3
**����޸�����: 2003��8��3��
**��        ��: driver core source code
**
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: cole3
** ��  ��: V0.50
** �ա���: 2013��4��6��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#ifdef EZ_STDIO
#include <stdarg.h>
#endif
#include <stdio.h>

#include "drivers.h"
#include "null.h"
#include "leds.h"
#include "uart.h"
#include "keys.h"
#include "DS1302.h"

const struct file_operations *const devices[DEVICE_MAX_NUM] = {
	&null_fops,
	&leds_fops,
	&uart_fops,
	&null_fops,
	&null_fops,
	&ds1302_fops,
	&null_fops,
	&null_fops,
	&null_fops
	//Add a new device...
};




/*
 * system tick
 */
uint8 systick_init(void)
{
	uint16 timer_val = 0;

	timer_val = 0x10000 - (SYS_CLK / OS_TICKS_PER_SEC / 12 / TICK_TIMER_SHARING);
	if (timer_val > 0xFFFF)
	{
		return FALSE;
	}

	TMOD = (TMOD & 0XF0) | 0x01;
	TL0 = timer_val & 0xFF;
	TH0 = (timer_val >> 8) & 0xFF;
	TR0 = 1;
	ET0 = 1;

	return TRUE;
}

static TIMER_MEM_SEL uint16 tick = 0;

void UserTickTimer(void)
{
	tick++;
}

uint16 get_system_timer_value(void)
{
	return tick;
}


#ifdef EN_HW_TIMER
/*
 * hardware timer
*/
static TIMER_MEM_SEL TIMER_OBJ timer_obj[TIMER_MAX] = {0};

static void hw_timer_init(void)
{
	if (TR2 == 1)
	{
		return;
	}
	ET2 = 0;
	TH2 = 0xFC; //1ms
	TL2 = 0x66;
	RCAP2H = TH2;
	RCAP2L = TL2;
	TR2 = 1;
}

static void hw_timer_deinit(void)
{
	ET2 = 0;
	TR2 = 0;
	TF2 = 0;
}

static void hw_timer_enable_interrupt(void)
{
	ET2 = 1;
}
static void hw_timer_disable_interrupt(void)
{
	ET2 = 0;
	TF2 = 0;
}


int8 timer_init(TIMER_OBJ *timer)
{
	uint8 i = 0;

	if (timer == NULL)
	{
		printf("timer == NULL\r\n");
		return -1;
	}
	if ((timer->time_ms == 0) || (timer->function == NULL))
	{
		printf("time/function invaild\r\n");
		return -2;
	}
	for (i=0; i<TIMER_MAX; i++)
	{
		if (timer_obj[i].time_ms == 0)
		{
			break;
		}
	}
	if (i == TIMER_MAX)
	{
		printf("i == TIMER_MAX\r\n");
		return -3;
	}
	// disable timer interrupt
	hw_timer_disable_interrupt();
	// add timer to array
	timer_obj[i].time_ms = timer->time_ms;
	timer_obj[i].function = timer->function;
	// reenable timer
	hw_timer_init();
	hw_timer_enable_interrupt();

	return 0;
}

void timer2_isr() interrupt 5
{
	uint8 i = 0;
	bool deinit_flag = TRUE;
	
	hw_timer_disable_interrupt();

	for (i=0; i<TIMER_MAX; i++)
	{
		if (timer_obj[i].time_ms == 0)
		{
			continue;
		}
		deinit_flag = FALSE;
		timer_obj[i].time_ms--;
		if (timer_obj[i].time_ms == 0)
		{
			void (const *function)(void);
			
			function = timer_obj[i].function;
			timer_obj[i].function = NULL;
			function();
		}
	}

	if (deinit_flag)
	{
		hw_timer_deinit();
	}
	else
	{
		hw_timer_enable_interrupt();
	}
}
#endif //EN_HW_TIMER

uint16 read_T0_val(void)
{
	uint8 th0 = TH0;
	uint8 tl0 = TL0;

	while (th0 != TH0)
	{
		th0 = TH0;
		tl0 = TL0;
	}

	return (uint16)th0 << 8 | tl0;
}

void udelay(uint16 us)
{
    uint16 cur = 0, stop = 0;
#if SYS_CLK == 22118400UL
	uint16 cnt = us << 1;
#else
	uint16 cnt = us;
#endif
	
    cur = read_T0_val();
    stop = cur + cnt;
    
    while (stop < cur)
    {
        cur = read_T0_val();
    }
    while (stop > cur)
    {
        cur = read_T0_val();
    }
}

void mdelay(uint16 ms)
{
	while (ms--)
	{
		udelay(1000);
	}
}


#ifdef EZ_STDIO
/*
 * easy printf for saving momory
*/
int ez_printf(char *fmt, ...)
{
	va_list vp;
	
	va_start(vp, fmt);

	while (*fmt)
	{
        if (*fmt == '%')  
        {
            switch (*(++fmt))  
            {  
				case 'c':
					{
						int ival = 0;
						
						ival = va_arg(vp, int);
						putchar((char)ival);
					}
					break;
				case 's':
					{
						char *ptr = NULL;
						
						ptr = va_arg(vp, char*);
						while (*ptr)
						{
							putchar(*ptr++);
						}
					}
					break;
				case 'd': 
					{
						int ival = 0;
						unsigned int div_val = 0; 
						
						ival = va_arg(vp, int);
						if (ival < 0)
						{
							putchar('-');
							ival = -ival;
						}
						
						div_val = 10000;
						while ((div_val > 1) && (div_val > ival))
						{
							div_val /= 10;
						}

						while (div_val)
						{
							putchar(ival/div_val + '0');
							ival %= div_val;
							div_val /= 10;
						}
					}
					break;
				//case 'b':
				//case 'B':
				//case 'f':
				case '%':
					putchar('%');
					break;
				default:break;
			}
		}
		else
		{
			putchar(*fmt++);
		}
	}

	va_end(vp);

	return 0;
}
#endif

