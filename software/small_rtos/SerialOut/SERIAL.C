/*********************************************************************************************************
**				                               Small RTOS(51)
**                                   The Real-Time Kernel(For Keil c51)
**
**                                  (c) Copyright 2002-2003, chenmingji
**                                           All Rights Reserved
**
**                                                  V1.12.0
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: serial.c
**��   ��   ��: ������
**����޸�����: 2002��12��30��
**��        ��: Ϊ�ź������ƵĲ��Դ��롣
**
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: ������
** ��  ��: V1.10
** �ա���: 2002��9��1��
** �衡��: ԭʼ�汾
**
**------------------------------------------------------------------------------------------------------
** �޸���: ������
** ��  ��: V1.11.0
** �ա���: 2003��12��2��
** �衡��: ɾ��TaskE����
**
**------------------------------------------------------------------------------------------------------
** �޸���: ������
** ��  ��: V1.12.0
** �ա���: 2003��12��30��
** �衡��: �ޱ仯
**
**--------------��ǰ�汾�޶�------------------------------------------------------------------------------
** �޸���: 
** �ա���:
** �衡��:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "config.h"


		void InitSerial(void)
{
	SCON = 0x50;
	PCON = 0x80;
	TMOD = TMOD & 0x0f;
	TMOD = TMOD | 0x20;
	TH1 = 0xf3;                   //com is 4800 b/s
	TL1 = 0xf3;
	TR1 = 1;
	ES = 0;

}

void TaskA(void);
void TaskB(void);
void TaskC(void);
void TaskD(void);

                            void main(void)
{
    OSInit();
    OSTaskCreate(TaskA, NULL, 0);
    OSTaskCreate(TaskB, NULL, 1);
    OSTaskCreate(TaskC, NULL, 2);
    OSTaskCreate(TaskD, NULL, 3);

    while(1)
    {
        PCON = PCON | 0x01;                     /* CPU��������״̬ */
    }
}

                            void putch(uint8 Data)
{
    uint8 i,j;

    SBUF = Data;
    i = 2048 / 256;
    j = 2048 % 256;
    do
    {
        do
        {
        } while (--j !=0);
    } while (--i != 0);
    
}


                            void TaskA(void)
{
	InitSerial();
	TMOD = (TMOD & 0XF0) | 0X01;
	TL0 = 0x0;
	TH0 = 0x0;
	TR0 = 1;
	ET0 = 1;
	TF0 = 0;
    EA = 1;
    
	OSSemCreate(0,1);
    while (1)
    {
        OSSemPend(0,0);
        putch('A');
        putch('0');
        putch('1');
        putch('2');
        putch('3');
        putch('\n');
        OSSemPost(0);
        OSWait(K_TMO,TL0);
    } 
}

                            void TaskB(void)
{
    while (1)
    {
        OSSemPend(0,0);
        putch('B');
        putch('0');
        putch('1');
        putch('2');
        putch('3');
        putch('\n');
        OSSemPost(0);
        OSWait(K_TMO,TL0);
    } 
}

                            void TaskC(void)
{
    while (1)
    {
        OSSemPend(0,0);
        putch('C');
        putch('0');
        putch('1');
        putch('2');
        putch('3');
        putch('\n');
        OSSemPost(0);
        OSWait(K_TMO,TL0);
    } 
}

                            void TaskD(void)
{
    uint8 i;
    
    while (1)
    {
        OSSemPend(0,0);
        for (i = 0; i < 100; i++)
        {
            putch('D');
            putch('E');
            putch('F');
            putch('G');
            putch(' ');
        }
        OSSemPost(0);
    } 
}