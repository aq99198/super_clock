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
**��   ��   ��: ext2.c
**��   ��   ��: ������
**����޸�����: 2002��12��30��
**��        ��: ext2Դ���롣
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
** �衡��: ɾ��TaskF����
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
#include "display.h"

void TaskA(void);
void TaskB(void);
void TaskC(void);
void TaskD(void);
void TaskE(void);

                            void main(void)
{
    OSInit();

	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK); 
    PC_DispStr(15,  0, "Small RTOS(51), the Real-Time Kernel(For Keil c51)", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
    PC_DispStr(35,  1, "ChenMingJi", DISP_FGND_WHITE);
    PC_DispStr(35,  3, "EXAMPLE #2", DISP_FGND_WHITE);

    OSTaskCreate(TaskA, NULL, 0);
    OSTaskCreate(TaskB, NULL, 1);
    OSTaskCreate(TaskC, NULL, 2);
    OSTaskCreate(TaskD, NULL, 3);

	TMOD = (TMOD & 0XF0) | 0X01;
	TL0 = 0x0;
	TH0 = 0x0;
	TR0 = 1;
	ET0 = 1;
	TF0 = 0;
    while(1)
    {
        PCON = PCON | 0x01;                     /* CPU��������״̬ */
    }
}


                            void TaskA(void)
{
    uint8 i;

    for (i = 0; i < 30; i++)
    {
        PC_DispChar(10, 5, (i % 10) + '0', DISP_FGND_LIGHT_GRAY);
        OSWait(K_TMO, 10);
    }
    OSTaskDel(2);
    for (i = 0; i < 10; i++)
    {
        PC_DispChar(10, 5, (i % 10) + '0', DISP_FGND_LIGHT_GRAY);
        OSWait(K_TMO, 10);
    }
    OSTaskCreate(TaskC, NULL, 2);
    i = 0;
    while (1)
    {
        
        PC_DispChar(10, 5, i + '0', DISP_FGND_LIGHT_GRAY);
        OSWait(K_TMO, 10);
        i++;
        i = i % 10;
    }    
}

                            void TaskB(void)
{
    uint8 i;
    for (i = 0; i < 10; i++)
    {
        PC_DispChar(20, 10, (i % 10) + '0', DISP_FGND_LIGHT_GRAY);
        OSWait(K_TMO, 15);
    }
    OSTaskDel(OSRunningTaskID());
}


                            void TaskC(void)
{
    uint8 i;
    
    i = 0;
    while (1)
    {
        
        PC_DispChar(10, 10, i + '0', DISP_FGND_LIGHT_GRAY);
        OSWait(K_TMO, 5);
        i++;
        i = i % 10;
    }    
}

                            void TaskD(void)
{
    uint8 i;

    for (i = 0; i < 30; i++)
    {
        PC_DispChar(20, 5, (i % 10) + '0', DISP_FGND_LIGHT_GRAY);
        OSWait(K_TMO, 20);
    }
    OSTaskDel(0);
    
    i = 0;
    while (1)
    {
        PC_DispChar(20, 5, i + '0', DISP_FGND_LIGHT_GRAY);
        OSWait(K_TMO, 20);
        i++;
        i = i % 10;
    }    
}