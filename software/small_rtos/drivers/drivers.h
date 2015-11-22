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
**文   件   名: drivers.h
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
#ifndef _DRIVERS_H_
#define _DRIVERS_H_


#include "config.h"


// dev
#define NUL         0
#define LED         1
#define UART        2
#define BEEP        3
#define DS18B20     4
#define DS1302      5
#define LCD12864    6
#define LCD1602     7
#define KEY         8
#define DEVICE_MAX_NUM 9

// id
#define ALL_ID			0xFF

// method
#define	OPEN(dev, id)				devices[dev]->open(id)
#define	CLOSE(dev, id)				devices[dev]->close(id)
#define	IOCTL(dev, id, cmd, val)	devices[dev]->ioctl(id, cmd, val)


// struct
struct file_operations {
	int8 (*open)(uint8 id);
	void (*close)(uint8 id);
	int8 (*ioctl)(uint8 id, uint8 cmd, uint16 args);
};

extern const struct file_operations *const devices[DEVICE_MAX_NUM];


// ioctl cmd
#define IOCTL_DIR			(7)
#define CMD_SET				(0<<IOCTL_DIR)
#define CMD_GET				(1<<IOCTL_DIR)
#define GET_CMD_DIR(x)		((1<<IOCTL_DIR) & (x))

// null

// led
#define LED_ON				CMD_SET|0	/* turn on led */
#define LED_OFF				CMD_SET|1   /* trun off led */
#define LED_TRIGGER			CMD_SET|2	/* trigger led */
#define	LED_STATE			CMD_GET|0	/* query led state */

// uart
#define UART_SET_BAUDRATE	CMD_SET|0	/* set uart baud rate */
#define UART_SET_CHAR		CMD_SET|1	/* send char by uart */
#define UART_SET_STR		CMD_SET|2	/* send string by uart */
#define UART_SET_NUM		CMD_SET|3	/* send string by uart */
#define UART_GET_BAUDRATE	CMD_GET|0	/* get uart baud rate */
#define UART_GET_CHAR		CMD_GET|1	/* receive char by uart (block)*/
#define UART_GET_STR		CMD_GET|2	/* receive string by uart (block)*/

// key
#define KEY_GET				CMD_GET|0	/* get key value  */
#define KEY_PEND			CMD_GET|1	/* pending key event  */
// key type
typedef enum
{
    ONE,
	TWO,
	THREE,
	MAX_K_TYPE
}K_TYPE;

// beep
#define BEEP_SET_FREQ		CMD_SET|0	/* set beep frequence */
#define BEEP_OUTPUT         CMD_SET|1   /* output beep */
#define BEEP_MODE           CMD_SET|2   /* sound output mode */
#define BEEP_GET_STATE      CMD_GET|0   /* get beep state */

// DS18B20
#define DS18B20_GET         CMD_GET|0   /* get ds18b20 temp */

// DS1302
typedef struct {
	uint8 year;
	uint8 day;
	uint8 month;
	uint8 date;
	uint8 hour;
	uint8 min;
	uint8 sec;
} TIME_VAL;

#define DS1302_SET_TIME     CMD_SET|0   /* set ds1302 time */
#define DS1302_SET_RAM      CMD_SET|1   /* set ds1302 ram */
#define DS1302_GET_TIME     CMD_GET|0   /* get ds1302 time */
#define DS1302_GET_RAM      CMD_GET|1   /* get ds1302 ram */

// LCD1602
#define LCD1602_CLEAR       
#define LCD1602_SET_STR
#define LCD1602_GET_STATE

// LCD12864
#define LCD12864_CLEAR
#define LCD12864_SET_STR
#define LCD12864_SET_FONT
#define LCD12864_SET_LINE
#define LCD12864_SET_RECT
#define LCD12864_SET_CIRCLE
#define LCD12864_SET_
#define LCD12864_GET_STATE


// define semaphore
#define KEYSEM          0



// delay
void udelay(uint16 us);
void mdelay(uint16 ms);

// timer
#define TIMER_MEM_SEL	xdata
#define TIMER_MAX		2
typedef struct
{
    uint16 time_ms;	// ms
    void (const *function)(void);	// interrupt
}TIMER_OBJ;
extern uint8 systick_init(void); //systick
uint16 get_system_timer_value(void);
extern void udelay(uint16 us);
#ifdef EN_HW_TIMER
extern int8 timer_init(TIMER_OBJ *timer);
#endif

//ez stdio
#ifdef EZ_STDIO
#define printf	ez_printf
int ez_printf(char *fmt, ...);	
#endif

#endif //_DRIVERS_H_
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

