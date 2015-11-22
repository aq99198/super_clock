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
**文   件   名: uart.c
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
#include "uart.h"



static uint8 xdata SerialCanSend = TRUE;
static uint8 OS_Q_MEM_SEL SerialOutData[UART_SEND_Q_SIZE];
static uint8 OS_Q_MEM_SEL SerialInData[UART_RCV_Q_SIZE];

// stdio redefine
char uart_send_char(char ch)
{
    OS_ENTER_CRITICAL();
    if (SerialCanSend == TRUE)
    {
        SerialCanSend = FALSE;
        SBUF = ch;
    }
    else
    {
        OSQPost(SerialOutData, ch);
    }
    OS_EXIT_CRITICAL();
	
    return ch;
}

char uart_rcv_char(void)
{
	uint8 ch = '0';

	OSQPend(&ch, SerialInData, 0);

    return ch;
}

int8 uart_send_str(uint8 *ptr)
{
	while (*ptr)
	{
		uart_send_char(*ptr++);
	}
	return 0;
}

static int8 uart_set_baudrate(uint32 baudrate)
{
	uint8 start_val = 0;
#ifdef UART_1T_MODE
    uint8 rep = 1;
#else
    uint8 rep = 12;
#endif

	if (SYS_CLK / rep / 32 / baudrate > 0xFF)
	{
		return -1;
	}

	start_val = SYS_CLK / rep / 32 / baudrate;
	if (start_val == 0)
	{
		return -1;
	}
	TR1 = 0;
	TH1 = 0x100 - start_val;
    TL1 = TH1;
	TR1 = 1;
	
	return 0;
}

static uint16 uart_get_baudrate(void)
{
    return (SYS_CLK / 12 / 32 / (0x100 - TH1));
}

static int8 uart_open(uint8 id)
{
    unsigned char u;

	if (id >= MAX_UART && id != ALL_ID)
	{
		return -1;
	}

    TMOD = TMOD & 0x0F | 0x20;    // 定时器1工作在方式2（自动重装）
    SCON = 0x50;                  // 10位uart，允许串行接受            
#ifdef UART_1T_MODE
    AUXR |= 1<<6;   // enable 1T mode
#endif
	uart_set_baudrate(UART_DEFAULT_BAUD);
    u = SBUF;
	// init intarrupt
	OSQCreate(SerialOutData, UART_SEND_Q_SIZE);  // init uart send Q
	OSQCreate(SerialInData, UART_RCV_Q_SIZE);  // init uart receive Q
	
	ES = 1;

	return 0;
}

static void uart_close(uint8 id)
{
	if (id >= MAX_UART && id != ALL_ID)
	{
		return;
	}
	TR1 = 0;
}

static int8 uart_ioctl(uint8 id, uint8 cmd, uint16 args)
{
	uint8 *para = (uint8 *)args;
	int8 ret = 0;

	UNUSED(id);
	if (GET_CMD_DIR(cmd) == CMD_SET)
	{
		switch (cmd)
		{
			case UART_SET_BAUDRATE:
				ret = uart_set_baudrate(args);
				break;
			case UART_SET_CHAR:
				uart_send_char((uint8)args);
				break;
			case UART_SET_STR:
				{
					uint8 const *para = (uint8 const *)args;
					if (para == NULL)
					{
						ret = -1;
						break;
					}
					uart_send_str(para);
				}
				break;
			default: break;
		}
	}
	else // CMD_GET
	{
		switch (cmd)
		{
			case UART_GET_BAUDRATE:
				*para = uart_get_baudrate();
				break;
			case UART_GET_CHAR:
				*para = uart_rcv_char();
				break;
			default: break;
		}
	}
	
	return ret;
}

#pragma disable
void uart0_irq(void) interrupt 4
{
	if (TI == 1)
	{
		uint8 temp;

		TI = 0;
		if (OSQAccept(&temp, SerialOutData) == OS_Q_OK)
		{
		    SBUF = temp;
		}
		else
		{
		    SerialCanSend = TRUE;
		}
	}
	if (RI == 1)
	{
        OS_INT_ENTER();
		RI = 0;
        OSQPost(SerialInData, SBUF);
        OSIntExit();
		return;
	}
}


const struct file_operations uart_fops = {
	uart_open,
	uart_close,
	uart_ioctl
};


// stdio redefine
char putchar(char ch)
{
	return uart_send_char(ch);
}

char _getkey(void)
{
	return uart_rcv_char();
}

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
