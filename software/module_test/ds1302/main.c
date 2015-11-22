/*
 * DS1302 ͻ����ʽ����������д
 *
*/

#include "STC12C5A60S2.H"
#include <intrins.h>



typedef unsigned char uint8;
typedef unsigned int uint16;

#define SCK_BIT		4
#define SDA_BIT		5
#define RST_BIT		6


sbit SCK = P4_IO(SCK_BIT);	//ʱ��	
sbit SDA = P4_IO(SDA_BIT);	//����	
sbit RST = P4_IO(RST_BIT);  //DS1302��λ(Ƭѡ)


#define BCD2HEX(n)	((((n) & 0xf0) >> 4) * 10) + ((n) & 0xf)
#define HEX2BCD(n)	(((n) / 10) << 4) + ((n) % 10)


#define DS1302_W_ADDR 0xBE
#define DS1302_R_ADDR 0xBF


uint8 time[7]={50,59,23,15,8,7,10};//���ʱ��������10-08-15 23:59:50


void delay(uint16 n)
{
	while (n--);
}


/**
 * дһ���ֽ�
*/
void write_ds1302_byte(uint8 dat) 
{
	uint8 i;
	
	for (i=0;i<8;i++) 
	{ 
		SDA = dat & 0x01;
		SCK = 1;
		dat >>= 1;
		SCK = 0;
	}
}

/**
 * ��һ���ֽ�
*/
uint8 read_ds1302_byte(void) 
{
	uint8 i, dat=0;
	
	for (i=0;i<8;i++)
	{	
		dat >>= 1;
		if (SDA)
			dat |= 0x80;
		SCK = 1;
		SCK = 0;
	} 

	return dat;
}

void reset_ds1302(void)
{
	RST = 0;
	SCK = 0;
	RST = 1;
}

/**
 * ���д����
*/
void clear_ds1302_WP(void) 
{
	reset_ds1302();
	RST = 1;
	write_ds1302_byte(0x8E);
	write_ds1302_byte(0);
	SDA = 0;
	RST = 0;
}

/**
 * ����д����
*/
void set_ds1302_WP(void) 
{
	reset_ds1302();
	RST = 1;
	write_ds1302_byte(0x8E);
	write_ds1302_byte(0x80);
	SDA = 0;
	RST = 0;
}


/**
 * �趨ʱ������
*/
void set_time(uint8 *timedata)
{
	uint8 i, tmp;
	
	clear_ds1302_WP();
	
	reset_ds1302();
	RST = 1;
	write_ds1302_byte(DS1302_W_ADDR);
	for (i=0; i<7; i++)
	{
		tmp = HEX2BCD(((uint8 *)timedata)[i]);
		write_ds1302_byte(tmp);
	}
	write_ds1302_byte(0);
	SDA = 0;
	RST = 0;

	set_ds1302_WP();
}

/**
 * ��ʱ�����ݣ�BCD��ʽ��
*/
void read_time(uint8 *timedata)
{
	uint8 i;

	clear_ds1302_WP();

	reset_ds1302();
	RST = 1;
	write_ds1302_byte(DS1302_R_ADDR);
	P1M0 = 0;
	P1M1 = 2;
	for (i=0; i<7; i++)
	{
		timedata[i] = read_ds1302_byte();
		timedata[i] = BCD2HEX(timedata[i]);
	}
	P1M0 = 0;
	P1M1 = 0;
	SDA = 0;
	RST = 0;
	
	set_ds1302_WP();
}



/**
 * UART��ʼ��
 * �����ʣ�9600
*/
void uart_init(void)
{
    TMOD = 0x21;        // ��ʱ��1�����ڷ�ʽ2���Զ���װ��
    SCON = 0x50;        // 10λuart�������н���

    TH1 = 0xFD;
    TL1 = 0xFD;

    TR1 = 1;
}

/**
 * UART����һ�ֽ�
*/
void UART_Send_Byte(uint8 dat)
{
	SBUF = dat;
	while (TI == 0);
	TI = 0;
}

/**
 * ������ת����ASC�벢ͨ��UART���ͳ�ȥ
*/
void UART_Send_Dat(uint8 dat)
{
	UART_Send_Byte(dat/10 + '0');
	UART_Send_Byte(dat%10 + '0');
}

main()    
{
	P1M0 = 0;
	P1M1 = 0;

	SET_P4_OUT_IO(RST_BIT);
	RST = 0;

	SET_P4_OUT_IO(SDA_BIT);
	SDA = 1;

	SET_P4_OUT_IO(SCK_BIT);
	SCK = 0;

	uart_init();
 	//set_time(&time);    //�趨ʱ��ֵ

	while(1)
	{

		read_time(&time);   //���ʱ��������
  	
		UART_Send_Dat(time[6]);
		UART_Send_Byte('-');
		UART_Send_Dat(time[4]);
		UART_Send_Byte('-');
		UART_Send_Dat(time[3]);
		UART_Send_Byte(' ');

		UART_Send_Dat(time[2]);
		UART_Send_Byte(':');
		UART_Send_Dat(time[1]);
		UART_Send_Byte(':');
		UART_Send_Dat(time[0]);
		UART_Send_Byte('\r');
		UART_Send_Byte('\n');
		
		delay(60000);
		delay(60000);
		delay(60000);
		delay(60000);
		delay(60000);
		delay(60000);
		delay(60000);
		delay(60000);
		delay(60000);
	}
}
