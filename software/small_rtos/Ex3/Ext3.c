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
**文   件   名: ext3.c
**创   建   人: 陈明计
**最后修改日期: 2003年8月3日
**描        述: ext3源代码。
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: V0.50
** 日　期: 2002年2月22日
** 描　述: 原始版本
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: V1.11.0
** 日　期: 2003年12月2日
** 描　述: 删除TaskC代码
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: V1.12.0
** 日　期: 2003年12月30日
** 描　述: 无变化
**
**------------------------------------------------------------------------------------------------------
** 修改人: 陈明计
** 版  本: V1.20.0
** 日　期: 2003年8月3日
** 描　述: 根据新版本要求修改
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "config.h"
#include "display.h"

void TaskA(void);
void TaskB(void);

                            void main(void)
{
    OSInit();

	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK); 
    PC_DispStr(15,  0, "Small RTOS(51), the Real-Time Kernel(For Keil c51)", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
    PC_DispStr(35,  1, "ChenMingJi", DISP_FGND_WHITE);
    PC_DispStr(35,  3, "EXAMPLE #3", DISP_FGND_WHITE);

	TMOD = (TMOD & 0XF0) | 0X01;
	TL0 = 0x0;
	TH0 = 0x0;
	TR0 = 1;
	ET0 = 1;
	TF0 = 0;

    OSTaskCreate(TaskA, NULL, 0);
    OSTaskCreate(TaskB, NULL, 1);
    OSTaskCreate(TaskB, NULL, 2);
    OSTaskCreate(TaskB, NULL, 3);
    OSTaskCreate(TaskB, NULL, 4);
    OSTaskCreate(TaskB, NULL, 5);
    OSTaskCreate(TaskB, NULL, 6);
    OSTaskCreate(TaskB, NULL, 7);
    OSTaskCreate(TaskB, NULL, 8);
    OSTaskCreate(TaskB, NULL, 9);

    while(1)
    {
        PCON = PCON | 0x01;                     /* CPU进入休眠状态 */
    }
}

uint8 ShowChar;


                            void TaskA(void)
{
    uint8 x,y;
    
    while (1)
    {
        OSWait(K_SIG,0);
        x = random(80);
        y = random(20);
        PC_DispChar(x, y + 5, ShowChar, DISP_FGND_LIGHT_GRAY);    
    }    
}


                            void TaskB(void)
{
    while (1)
    {
        ShowChar = OSRunningTaskID()+'0';
        OSSendSignal(0);
        OSWait(K_TMO,1);
    }    
}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
