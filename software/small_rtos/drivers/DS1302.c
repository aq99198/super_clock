#include <stdio.h>
#include "DS1302.h"



#define SCK_BIT		4
#define SDA_BIT		5
#define RST_BIT		6

sbit SCK = P4_IO(SCK_BIT);	//时钟	
sbit SDA = P4_IO(SDA_BIT);	//数据	
sbit RST = P4_IO(RST_BIT);  //DS1302复位(片选)


#define BCD2HEX(n)	((((n) & 0xf0) >> 4) * 10) + ((n) & 0xf)
#define HEX2BCD(n)	(((n) / 10) << 4) + ((n) % 10)


#define DS1302_W_ADDR 0xBE
#define DS1302_R_ADDR 0xBF


static void ds1302_init(void)
{
	SET_P4_OUT_IO(RST_BIT);
	RST = 0;

	SET_P4_OUT_IO(SDA_BIT);
	SDA = 1;

	SET_P4_OUT_IO(SCK_BIT);
	SCK = 0;
}

void ds1302_reset(void)
{
	RST = 0;
	SCK = 0;
	RST = 1;
}

void ds1302_write_byte(uint8 dat) 
{
	uint8 i;
	
	for (i=0; i<8; i++) 
	{ 
		SDA = dat & 0x01;
		SCK = 1;
		dat >>= 1;
		SCK = 0;
	}
}

uint8 ds1302_read_byte(void) 
{
	uint8 i, dat=0;
	
	for (i=0; i<8; i++)
	{	
		dat >>= 1;
		if (SDA)
			dat |= 0x80;
		SCK = 1;
		SCK = 0;
	} 

	return dat;
}

void ds1302_clear_wp(void) 
{
	ds1302_reset();
	RST = 1;
	ds1302_write_byte(0x8E);
	ds1302_write_byte(0);
	SDA = 0;
	RST = 0;
}

void ds1302_set_wp(void) 
{
	ds1302_reset();
	RST = 1;
	ds1302_write_byte(0x8E);
	ds1302_write_byte(0x80);
	SDA = 0;
	RST = 0;
}

void ds1302_set_time(TIME_VAL *time)
{
	uint8 i, tmp;
	
	ds1302_clear_wp();
	
	ds1302_reset();
	RST = 1;
	ds1302_write_byte(DS1302_W_ADDR);
	for (i=0; i<7; i++)
	{
		tmp = HEX2BCD(((uint8 *)time)[i]);
		ds1302_write_byte(tmp);
	}
	ds1302_write_byte(0);
	SDA = 0;
	RST = 0;

	ds1302_set_wp();
}

void ds1302_read_time(TIME_VAL *time)
{
	uint8 i, tmp;

	ds1302_clear_wp();

	ds1302_reset();
	RST = 1;
	ds1302_write_byte(DS1302_R_ADDR);
	for (i=0; i<7; i++)
	{
		tmp = ds1302_read_byte();
		((uint8 *)time)[i] = BCD2HEX(tmp);
        //printf("%d\r\n", tmp);
	}
	SDA = 0;
	RST = 0;
	
	ds1302_set_wp();
}


static int8 ds1302_open(uint8 id)
{
	UNUSED(id);
	ds1302_init();
	
	return 0;
}

static void ds1302_close(uint8 id)
{
	UNUSED(id);
}

static int8 ds1302_ioctl(uint8 id, uint8 cmd, uint16 args)
{
	TIME_VAL *p_time = (TIME_VAL *)args;

	if (GET_CMD_DIR(cmd) == CMD_SET)
	{
		switch (cmd)
		{
			case DS1302_SET_TIME:
				ds1302_set_time(p_time);
				break;
			default: 
				break;
		}
	}
	else // CMD_GET
	{
		switch (cmd)
		{
			case DS1302_GET_TIME:
				ds1302_read_time(p_time);
				break;
			default:
				break;
		}
	}
	
	UNUSED(id);
	
	return 0;
}


const struct file_operations ds1302_fops = {
	ds1302_open,
	ds1302_close,
	ds1302_ioctl
};

