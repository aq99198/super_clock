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
**��   ��   ��: ext1.c
**��   ��   ��: ������
**����޸�����: 2003��8��3��
**��        ��: ext1Դ���롣
**
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: ������
** ��  ��: V0.50
** �ա���: 2002��2��22��
** �衡��: ԭʼ�汾
**
**------------------------------------------------------------------------------------------------------
** �޸���: ������
** ��  ��: V1.11.0
** �ա���: 2003��12��2��
** �衡��: ɾ��TaskD����
**
**------------------------------------------------------------------------------------------------------
** �޸���: ������
** ��  ��: V1.12.0
** �ա���: 2003��12��2��
** �衡��: �ޱ仯
**
**------------------------------------------------------------------------------------------------------
** �޸���: ������
** ��  ��: V1.20.0
** �ա���: 2003��8��3��
** �衡��: �����°汾Ҫ���޸�
**
**--------------��ǰ�汾�޶�------------------------------------------------------------------------------
** �޸���: 
** �ա���:
** �衡��:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include <stdio.h>
#include "config.h"
#include "drivers.h"

#define KEY_Q_SIZE	16
static uint8 OS_Q_MEM_SEL KeyQ[KEY_Q_SIZE];



void TaskA(void)
{
    while (1)
    {
		printf("TaskA\r\n");
        OSWait(K_TMO,5);
    } 
}

void TaskB(void)
{
    while (1)
    {
		printf("TaskB\r\n");
        OSWait(K_TMO,10);
    }
}

static void timer_notify0(void)
{
	printf("timer test 0.\r\n");
}

static void timer_notify1(void)
{
	//TIMER_OBJ timer;

	printf("timer test 1.\r\n");

	//timer.time_ms = 4000; // ms
	//timer.function = timer_notify1;
	//timer_init(&timer);
}

void TaskC(void)
{
    TIME_VAL t;
    
    OPEN(DS1302, 0);
    
    while (1)
    {
		IOCTL(DS1302, 0, DS1302_GET_TIME, (uint16)&t);
        printf("%d-%d-%d %d:%d:%d\r\n", 
            t.year,
            t.month,
            t.date,
            t.hour,
            t.min,
            t.sec);
        
        OSWait(K_TMO,10);
    }
}

void TaskD(void)
{
	//TIMER_OBJ timer;

	//timer.time_ms = 10000; // ms
	//timer.function = timer_notify1;
	//timer_init(&timer);

    while (1)
    {
		IOCTL(LED, 3, LED_TRIGGER, NULL);
        OSWait(K_TMO,20);
    }
}

void TaskE(void)
{
    while (1)
    {
		printf("system tick = %d\r\n", get_system_timer_value());
	
		IOCTL(LED, 4, LED_TRIGGER, NULL);
        OSWait(K_TMO,4);
    }
}

void TaskF(void)
{
    while (1)
    {
		IOCTL(LED, 5, LED_TRIGGER, NULL);
        OSWait(K_TMO,3);
    }
}

void TaskG(void)
{
	uint8 key_val = 0;

    while (1)
    {
		OSQPend(&key_val, KeyQ, 0);
		switch (key_val)
		{
			case 1: 
				printf("key val 1\n");
				break;
			case 2:
				printf("key val 2\n");
				break;
			case 3:
				printf("key val 3\n");
				break;
			default:
				break;
		}
    }
}

void TaskH(void)
{
	uint8 key_val = 0, old_key_val = 0;

	OSQCreate(KeyQ, KEY_Q_SIZE);

    while (1)
    {
    	old_key_val = 0;
		IOCTL(KEY, 0, KEY_PEND, (uint16)&key_val);
        OSWait(K_TMO, 1);
		IOCTL(KEY, 0, KEY_GET, (uint16)&old_key_val);
		if (key_val == old_key_val)
		{
			OSQPost(KeyQ, key_val);
		}
    }
}


void main(void)
{
    OSInit();
	systick_init();

	OPEN(LED, ALL_ID);
	OPEN(UART, 0);
	OPEN(KEY, 0);

    OSTaskCreate(TaskA, NULL, 0);
    OSTaskCreate(TaskB, NULL, 1);
    OSTaskCreate(TaskC, NULL, 2);
#if 0
    OSTaskCreate(TaskD, NULL, 3);
    OSTaskCreate(TaskE, NULL, 4);
    OSTaskCreate(TaskF, NULL, 5);
    OSTaskCreate(TaskG, NULL, 6);
    OSTaskCreate(TaskH, NULL, 7);
#endif
    while(1)
    {
        PCON = PCON | 0x01;                     /* CPU��������״̬ */
    }
}

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
