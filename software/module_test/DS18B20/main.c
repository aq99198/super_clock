/*
 * DS18B20 
*/

#include "STC12C5A60S2.H"
#include <intrins.h> 

#define SYS_CLK 22118400UL
#define STC_1T

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef char int8;
typedef int int16;
typedef bit bool;

#define DQ_BIT	1
sbit DQ  = P1^1;

#define SET_P1_INPUT(b)	do { \
	P1M0 &= ~(1<<(b)); \
	P1M1 |= 1<<(b); \
} while (0)

#define SET_P1_OUTPUT(b)	do { \
	P1M0 &= ~(1<<(b)); \
	P1M1 &= ~(1<<(b)); \
} while (0)


void UART_Send_Dat(uint8 dat);

uint8 timer_init(void)
{
	uint16 timer_val = 0;

	TMOD &= ~0x0F;
	TMOD |= 0x01;
	TL0 = timer_val & 0xFF;
	TH0 = (timer_val >> 8) & 0xFF;
	TR0 = 1;
	
	return 0;
}

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

static void ds1302_init(void)
{
	//io input
	SET_P1_OUTPUT(DQ_BIT);
	DQ = 1;
}

static void ds1302_reset(void)
{
	bool flag = 1;

	while (flag)
	{
		while (flag)
	 	{
			SET_P1_OUTPUT(DQ_BIT);
 			DQ = 1;
			udelay(16);
 			DQ = 0;
 			udelay(550); // 550us
 			DQ = 1; 
			SET_P1_INPUT(DQ_BIT);
 			udelay(66);  // 66us
			flag = DQ;
   		}
		udelay(500);    // 500us
		flag = ~DQ;
	}
	SET_P1_OUTPUT(DQ_BIT);
	DQ=1;
}

static void ds1302_write_byte(uint8 val)
{
	uint8 i;

	SET_P1_OUTPUT(DQ_BIT);
	for (i=0; i<8; i++)
	{
		DQ = 1;
		udelay(1);
		DQ = 0;
		udelay(4); //4us
		DQ = val & 0x01; 
		udelay(66); //66us
		val >>= 1;
	}
	DQ = 1;
	udelay(10);  
}

static uint8 ds1302_read_byte(void)
{
	uint8 i, value=0;

	for (i=0; i<8; i++)
	{
		SET_P1_OUTPUT(DQ_BIT);
		DQ=1;
		udelay(1);
		value >>= 1;
		DQ = 0;
		udelay(4);   //4us
		DQ = 1;
		SET_P1_INPUT(DQ_BIT);
		udelay(4);   //4us 
		if (DQ)
			value |= 0x80;
		udelay(66);  //66us
	}
	SET_P1_OUTPUT(DQ_BIT);
	DQ=1;

	return value;
}

static void ds1302_start(void)
{
	ds1302_reset();
	ds1302_write_byte(0xCC); // Skip ROM cmd
	ds1302_write_byte(0x44); // convert cmd
}

static int16 ds1302_read_temp(void)
{
	uint8 temp_data[2]; // 读出温度暂放
	int16 temp;

	ds1302_reset();
	ds1302_write_byte(0xCC); // Skip ROM cmd
	ds1302_write_byte(0xBE); // read
	temp_data[0] = ds1302_read_byte();  // low 8 bits
	temp_data[1] = ds1302_read_byte();  // high 8 bits

	temp = temp_data[1];
	temp <<= 8;
	temp |= temp_data[0];
	temp >>= 4;

	return temp;
}


/**
 * UART初始化
 * 波特率：9600
*/
void uart_init(void)
{
    TMOD = 0x21;        // 定时器1工作在方式2（自动重装）
    SCON = 0x50;        // 10位uart，允许串行接受

    TH1 = 0xFD;
    TL1 = 0xFD;

    TR1 = 1;
}

/**
 * UART发送一字节
*/
void UART_Send_Byte(uint8 dat)
{
	SBUF = dat;
	while (TI == 0);
	TI = 0;
}

/**
 * 将数据转换成ASC码并通过UART发送出去
*/
void UART_Send_Dat(uint8 dat)
{
	UART_Send_Byte(dat/10 + '0');
	UART_Send_Byte(dat%10 + '0');
}


main()
{
	int16 ans;
	
	timer_init();
	uart_init();
	ds1302_init();
	
	while (1)
	{
		ds1302_start();

		mdelay(1000); // 延时1秒

		ans=ds1302_read_temp();
	
		if (ans < 0)
		{
			UART_Send_Byte('-');
			ans = -ans;
		}
	
		UART_Send_Dat(ans);
		UART_Send_Byte('\r');
		UART_Send_Byte('\n');
	}

}

