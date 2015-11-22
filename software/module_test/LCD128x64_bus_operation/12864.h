#ifndef _12864_H__
#define _12864_H__			

#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int	   //宏义

//#define CONFIG_IO_OPERATION

#ifdef CONFIG_IO_OPERATION
sbit RS = P2^1;  
sbit RW = P2^0;
sbit TE = P3^5; 
sbit WE = P3^6; 
sbit RE = P3^7; 
sbit CS1 = P2^7;   
sbit CS2 = P2^6;   
sbit RST = P3^4;
#define BUS  P0

void io_init()
{
	P0M0 = 0;
	P0M1 = 0;
}

void chekbusy12864(void)
{
  uchar dat;

P0M1 = 0xFF;
  WE = 1;
  CS1 = 1;
  CS2 = 0;
  RS=0;       //指令模式 
  RW=1;       //读数据
  do {
   BUS = 0x00;			 //
   RE = 0;
   TE = 1;
   dat = BUS & 0x80;
   RE = 1;
   TE = 0;
  } while(dat!=0x00);
  P1 = 3;
P0M1 = 0;
}

void cmd_w12864(uchar cmd)
{
  chekbusy12864();
  RS=0;	            //指令模式
  RW=0;             //写模式
  RE=0;
  TE = 1;
  BUS=cmd;
  RE=1;
  TE = 0;
}

void dat_w12864(uchar dat)
{
  chekbusy12864();
  RS=1;
  RW=0;
  RE=0;
  TE = 1;
  BUS=dat;
  RE=1;
  TE = 0;
}

#else
sbit PTEST = P3^5;

#define WCMD_ADDR	0x0000
#define RCMD_ADDR	0x0100
#define WDAT_ADDR	0x0200
#define RDAT_ADDR	0x0300
#define CS1			0x8000
#define CS2			0x4000

void io_init()
{
	P0M0 = 0;
	P0M1 = 0;
}

 /*12864判忙 */
void chekbusy12864(void)
{
  uchar dat;

//P0M1 = 0xFF;
  do {
    dat = *(unsigned char xdata *)0x0500;
	PTEST = 0;
//    P1 = dat;
  } while (dat & 0x80);
  PTEST = 1;
//P0M1 = 0;
}

/******写命令*****/
void cmd_w12864(uchar cmd)
{
  chekbusy12864();
  *(unsigned char xdata *)WCMD_ADDR = cmd;
}
/****写数据******/
void dat_w12864(uchar dat)
{
  chekbusy12864();
  *(unsigned char xdata *)WDAT_ADDR = dat;
}
#endif

/*****清屏*****/
void clear12864(void)
{
  uchar page, row;
  for(page=0xb8; page<0xc0; page++)
  {
    cmd_w12864(page);
	cmd_w12864(0x40);
	for(row=0; row<64; row++)
	{
	   dat_w12864(0xA3);
	}
  }
}
/***初始化****/
void init12864(void)
{
  io_init();

  chekbusy12864();
  cmd_w12864(0xc0);	  //从第零行开始显示
  cmd_w12864(0x3f);	  //LCD显示RAM中的内容
}
#if 0
/**8X8字符的显示**/
void font8(unsigned char x, unsigned char y, unsigned char code *ch)
{
  unsigned char i;
  unsigned int cs; 
  
  cs = (x < 64) ? CS2 : CS1;
  *(unsigned char xdata *)(WCMD_ADDR | cs) = 0x40 | x;
  *(unsigned char xdata *)(WCMD_ADDR | cs) = 0xb8 | y;
  //*(unsigned char xdata *)0x0400 = 0x40 | x;
  //*(unsigned char xdata *)0x0400 = 0xb8 | x;

  for(i=0;i<8;i++)
  {
    *(unsigned char xdata *)(WDAT_ADDR | cs) = ch[i];
    //*(unsigned char xdata *)0x0600 = ch;
  }
}
#endif
